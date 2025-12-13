#include "yamitracker.h"
#include "ui_yamitracker.h"
#include <QDesktopServices>
#include <QUrl>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QBuffer>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <math.h>

// Глобальная переменная для управления громкостью баса
double bassVolumeMultiplier = 2.0; // Увеличение громкости баса в 2 раза

// Инструменты басового типа для усиления громкости
QSet<int> bassInstruments = {
    28,  // FngrBass (Фингер-бас)
    33,  // FngrBass (Фингер-бас) - другой тип
    39,  // FngrBass (Фингер-бас) - еще тип
    40,  // SlapBas (Слэп-бас)
    38,  // DstGtr (Дисторшн-гитара) - тоже можно усилить
    29,  // Ovrdrive (Овердрайв-гитара)
    37   // Ovrdrive (Овердрайв-гитара) - другой тип
};

void SerialReader::run()
{
    while (!shouldStop) {
        int fd = open(currentDevice.toUtf8().constData(), O_RDWR | O_NOCTTY);
        if (fd < 0) {
            emit error(QString("Cannot open %1").arg(currentDevice));
            emit connectionStatusChanged(false);
            sleep(2);
            continue;
        }

        emit connectionStatusChanged(true);
        
        QString manufacturer = "Unknown";
        QString model = "Unknown";
        
        if (currentDevice.contains("yamaha", Qt::CaseInsensitive) || 
            currentDevice.contains("psr", Qt::CaseInsensitive)) {
            manufacturer = "Yamaha";
            model = "PSR Series";
        } else if (currentDevice.contains("dmmidi")) {
            manufacturer = "Yamaha";
            model = "PSR-E333 (Direct MIDI)";
        }
        
        emit deviceInfoReceived(manufacturer, model);

        struct termios tty;
        tcgetattr(fd, &tty);
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);
        tty.c_cflag |= (CLOCAL | CREAD | CS8);
        tty.c_cc[VMIN] = 1;
        tcsetattr(fd, TCSANOW, &tty);

        unsigned char byte;
        while (!shouldStop && read(fd, &byte, 1) > 0) {
            if (byte == 0xF8 || byte == 0xFE) {
                continue;
            }

            if (byte & 0x80) {
                if ((byte & 0xF0) == 0x90) {
                    unsigned char note, velocity;
                    if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                        QString hexNote = QString("%1").arg(note, 2, 16, QChar('0')).toUpper();

                        if (velocity == 0)
                            emit noteOffReceived(hexNote);
                        else
                            emit noteOnReceived(hexNote, velocity);
                    }
                }
                else if ((byte & 0xF0) == 0x80) {
                    unsigned char note, velocity;
                    if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                        QString hexNote = QString("%1").arg(note, 2, 16, QChar('0')).toUpper();
                        emit noteOffReceived(hexNote);
                    }
                }
            }
        }

        close(fd);
        emit connectionStatusChanged(false);
        
        if (!shouldStop) {
            emit error(QString("Connection lost to %1. Attempting to reconnect...").arg(currentDevice));
            sleep(2);
        }
    }
}

Yamitracker::Yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Yamitracker)
    , serialReader(new SerialReader(this))
    , reconnectTimer(new QTimer(this))
    , isConnected(false)
    , wavWriter(new SimpleWavWriter(this))
    , recordingStartTime(0)
    , brrConverter(new BrrConverter(this))
    , audioPlayerProcess(new QProcess(this))
    , playbackSpeedSlider(nullptr)
    , bassVolumeSlider(nullptr)
    , speedLabel(nullptr)
    , volumeLabel(nullptr)
    , audioOutput(nullptr)
    , audioBuffer(nullptr)
    , playbackTimer(new QTimer(this))
    , isPlayingBack(false)
    , currentPlaybackTrack(-1)
    , currentTrack(0)
{
    ui->setupUi(this);
    
    deviceLabel = ui->deviceLabel;
    refreshDevicesButton = ui->refreshDevicesButton;
    midiFilesList = ui->midiFilesList;
    instrumentComboBox = ui->instrumentComboBox;
    trackComboBox = ui->trackComboBox;

    // Ищем элементы в UI
    playbackSpeedSlider = findChild<QSlider*>("playbackSpeedSlider");
    bassVolumeSlider = findChild<QSlider*>("bassVolumeSlider");
    speedLabel = findChild<QLabel*>("speedLabel");
    volumeLabel = findChild<QLabel*>("volumeLabel");

    // Настраиваем слайдеры, если они найдены
    if (playbackSpeedSlider) {
        playbackSpeedSlider->setRange(50, 200); // 50% - 200%
        playbackSpeedSlider->setValue(100); // 100% по умолчанию
        playbackSpeedSlider->setTickInterval(10);
        playbackSpeedSlider->setTickPosition(QSlider::TicksBelow);
    }
    
    if (bassVolumeSlider) {
        bassVolumeSlider->setRange(100, 400); // 100% - 400%
        bassVolumeSlider->setValue(200); // 200% по умолчанию
        bassVolumeSlider->setTickInterval(50);
        bassVolumeSlider->setTickPosition(QSlider::TicksBelow);
    }
    
    // Инициализируем дорожки
    initializeTracks();
    initializeTrackControls();
    
    // Обновляем метки
    updateSpeedLabel();
    updateVolumeLabel();
    updateTrackInfo();
    
    // Подключаем сигналы
    if (playbackSpeedSlider) {
        connect(playbackSpeedSlider, &QSlider::valueChanged, this, &Yamitracker::onPlaybackSpeedChanged);
    }
    if (bassVolumeSlider) {
        connect(bassVolumeSlider, &QSlider::valueChanged, this, &Yamitracker::onBassVolumeChanged);
    }
    
    // Заполняем комбобокс инструментами
    QList<int> instrumentIds = instruments.keys();
    std::sort(instrumentIds.begin(), instrumentIds.end());
    for (int id : instrumentIds) {
        QString instrumentName = instruments[id];
        // Добавляем пометку для басовых инструментов
        if (bassInstruments.contains(id)) {
            instrumentName += " [БАС - усилено]";
        }
        instrumentComboBox->addItem(QString("%1: %2").arg(id).arg(instrumentName), id);
    }
    
    initializeKeyButtons();
    initializeDeviceDetection();

    // Подключаем сигналы серийного порта
    connect(serialReader, &SerialReader::noteOnReceived, this, &Yamitracker::onNoteOnReceived);
    connect(serialReader, &SerialReader::noteOffReceived, this, &Yamitracker::onNoteOffReceived);
    connect(serialReader, &SerialReader::error, this, &Yamitracker::onError);
    connect(serialReader, &SerialReader::connectionStatusChanged, this, &Yamitracker::onConnectionStatusChanged);
    connect(serialReader, &SerialReader::deviceInfoReceived, this, &Yamitracker::onDeviceInfoReceived);
    
    // Подключаем кнопки управления записью
    connect(ui->playButton, &QPushButton::clicked, this, &Yamitracker::onPlayClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &Yamitracker::onPauseClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &Yamitracker::onStopClicked);
    connect(ui->convertButton, &QPushButton::clicked, this, &Yamitracker::onConvertToBrrClicked);
    connect(refreshDevicesButton, &QPushButton::clicked, this, &Yamitracker::onRefreshDevicesClicked);
    connect(ui->loadMidiButton, &QPushButton::clicked, this, &Yamitracker::onLoadMidiFileClicked);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &Yamitracker::onSelectFileForConversion);
    connect(ui->playFileButton, &QPushButton::clicked, this, &Yamitracker::onSelectFileForPlayback);
    connect(ui->saveRecordingButton, &QPushButton::clicked, this, &Yamitracker::onSaveRecordingClicked);
    connect(ui->playMidiButton, &QPushButton::clicked, this, &Yamitracker::onPlayMidiFileClicked);
    connect(ui->recordFromMidiButton, &QPushButton::clicked, this, &Yamitracker::onRecordFromMidiFile);
    connect(midiFilesList, &QListWidget::itemSelectionChanged, this, &Yamitracker::onSelectedMidiFileChanged);
    connect(ui->saveMusicButton, &QPushButton::clicked, this, &Yamitracker::onSaveMusicFileClicked);
    connect(ui->addTrackButton, &QPushButton::clicked, this, &Yamitracker::onAddTrackClicked);
    connect(ui->removeTrackButton, &QPushButton::clicked, this, &Yamitracker::onRemoveTrackClicked);
    connect(ui->clearTrackButton, &QPushButton::clicked, this, &Yamitracker::onClearTrackClicked);
    connect(trackComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Yamitracker::onCurrentTrackChanged);
    
    // Подключаем новые кнопки
    connect(ui->playTrackButton, &QPushButton::clicked, this, &Yamitracker::onPlayTrackClicked);
    connect(ui->playAllButton, &QPushButton::clicked, this, &Yamitracker::onPlayAllTracksClicked);
    connect(ui->stopPlaybackButton, &QPushButton::clicked, this, &Yamitracker::onStopPlaybackClicked);
    connect(ui->loadWavButton, &QPushButton::clicked, this, &Yamitracker::onLoadWavToTrackClicked);
    connect(ui->copyTrackButton, &QPushButton::clicked, this, &Yamitracker::onCopyTrackClicked);
    connect(ui->mergeTrackButton, &QPushButton::clicked, this, &Yamitracker::onMergeTracksClicked);
    
    // Подключаем сигналы от wavWriter
    connect(wavWriter, &SimpleWavWriter::trackUpdated, this, [this](int track) {
        if (track == currentTrack) {
            updateTrackInfo();
        }
    });
    
    connect(wavWriter, &SimpleWavWriter::recordingStatusChanged, this, [this](bool recording) {
        if (recording) {
            ui->statusLabel->setText("Запись активна");
            ui->playButton->setEnabled(false);
            ui->stopButton->setEnabled(true);
            ui->pauseButton->setEnabled(true);
        } else {
            ui->statusLabel->setText("Запись остановлена");
            ui->playButton->setEnabled(true);
            ui->stopButton->setEnabled(false);
            ui->pauseButton->setEnabled(false);
            ui->pauseButton->setText("⏸ Пауза");
        }
    });
    
    connect(reconnectTimer, &QTimer::timeout, this, &Yamitracker::attemptReconnect);
    reconnectTimer->start(5000);
    
    connect(audioPlayerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode);
                if (exitStatus == QProcess::NormalExit) {
                    ui->statusLabel->setText("Воспроизведение завершено");
                }
            });
    
    // Подключаем таймер воспроизведения
    connect(playbackTimer, &QTimer::timeout, this, &Yamitracker::onPlaybackFinished);
    
    // ОБНОВЛЕННЫЙ СТИЛЬ - черный текст на белом фоне для панели управления дорожками
    // Остальные элементы остаются с белым текстом на темном фоне
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; }"
        "QLabel { color: #ffffff; font-size: 14px; font-weight: bold; }"
        "QGroupBox { color: #ffffff; font-weight: bold; border: 2px solid #555; border-radius: 5px; margin-top: 10px; }"
        "QGroupBox::title { color: #ffffff; subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        "QPushButton { background-color: #4a4a4a; color: #ffffff; border: 1px solid #555; border-radius: 3px; padding: 5px; font-weight: bold; }"
        "QPushButton:hover { background-color: #5a5a5a; }"
        "QPushButton:pressed { background-color: #3a3a3a; }"
        "QPushButton[whiteKey=true] { background-color: white; color: black; border: 1px solid #ccc; font-size: 8pt; }"
        "QPushButton[whiteKey=true]:pressed { background-color: #ff4444; color: white; }"
        "QPushButton[blackKey=true] { background-color: black; color: white; font-size: 7pt; }"
        "QPushButton[blackKey=true]:pressed { background-color: #ff4444; }"
        "QProgressBar { border: 1px solid #555; background: #333; color: white; }"
        "QProgressBar::chunk { background: #4CAF50; }"
        "QListWidget { background-color: #333; color: #ffffff; border: 1px solid #555; selection-background-color: #555; }"
        "QComboBox { background-color: #333; color: #ffffff; border: 1px solid #555; padding: 5px; }"
        "QComboBox QAbstractItemView { background-color: #333; color: #ffffff; selection-background-color: #555; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid white; }"
        "QSlider::groove:horizontal { border: 1px solid #555; height: 8px; background: #333; margin: 2px 0; }"
        "QSlider::handle:horizontal { background: #4CAF50; border: 1px solid #5f5f5f; width: 18px; margin: -2px 0; border-radius: 3px; }"
        "QLineEdit { background-color: #ffffff; color: #000000; border: 1px solid #555; padding: 3px; }"  // Изменено: белый фон, черный текст
        "QCheckBox { color: #ffffff; }"
        "QCheckBox::indicator { width: 16px; height: 16px; }"
        // Стили для панели управления дорожками
        "QScrollArea { background-color: white; }"
        "QScrollArea QGroupBox { background-color: white; color: #000000; border: 2px solid #aaa; }"
        "QScrollArea QGroupBox::title { color: #000000; }"
        "QScrollArea QLabel { color: #000000; }"
        "QScrollArea QComboBox { background-color: white; color: #000000; border: 1px solid #aaa; }"
        "QScrollArea QComboBox QAbstractItemView { background-color: white; color: #000000; selection-background-color: #ddd; }"
        "QScrollArea QPushButton { background-color: #e0e0e0; color: #000000; border: 1px solid #aaa; }"
        "QScrollArea QPushButton:hover { background-color: #d0d0d0; }"
        "QScrollArea QPushButton:pressed { background-color: #b0b0b0; }"
        "QScrollArea QSlider::groove:horizontal { border: 1px solid #aaa; height: 8px; background: #ddd; }"
        "QScrollArea QSlider::handle:horizontal { background: #4CAF50; border: 1px solid #5f5f5f; }"
    );

    QTimer::singleShot(1000, this, &Yamitracker::autoDetectAndConnect);
    setWindowTitle("Yamaha PSR-E333 Monitor - Multitrack Recording");
    
    // Сканируем MIDI файлы при запуске
    scanMidiFiles();
    
    // Информация о басовых инструментах
    qDebug() << "Бас-инструменты усилены в" << bassVolumeMultiplier << "раз";
    qDebug() << "Бас-инструменты: FngrBass(28,33,39), SlapBas(40), Ovrdrive(29,37), DstGtr(38)";
}

void Yamitracker::initializeTracks()
{
    trackComboBox->clear();
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {  // Используем MAX_TRACKS
        QString trackName = wavWriter->getTrackName(i);
        trackComboBox->addItem(trackName, i);
    }
    trackComboBox->setCurrentIndex(0);
    currentTrack = 0;
}

void Yamitracker::initializeTrackControls()
{
    // Получаем ScrollArea для управления дорожками
    QScrollArea* scrollArea = findChild<QScrollArea*>("scrollArea");
    if (!scrollArea) return;
    
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* tracksLayout = new QVBoxLayout(scrollContent);
    
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        QGroupBox* trackGroup = new QGroupBox(QString("Дорожка %1").arg(i + 1), scrollContent);
        trackGroup->setProperty("trackIndex", i);
        
        QHBoxLayout* trackLayout = new QHBoxLayout(trackGroup);
        
        // Название дорожки
        QLineEdit* nameEdit = new QLineEdit(wavWriter->getTrackName(i), trackGroup);
        nameEdit->setProperty("trackIndex", i);
        nameEdit->setObjectName(QString("trackNameEdit_%1").arg(i));
        nameEdit->setMaximumWidth(150);
        connect(nameEdit, &QLineEdit::textChanged, this, &Yamitracker::onTrackNameChanged);
        trackNameEdits.append(nameEdit);
        
        // Инструмент дорожки
        QComboBox* instrumentBox = new QComboBox(trackGroup);
        instrumentBox->setProperty("trackIndex", i);
        instrumentBox->setObjectName(QString("trackInstrumentBox_%1").arg(i));
        instrumentBox->setMaximumWidth(150);
        
        QList<int> instrumentIds = instruments.keys();
        std::sort(instrumentIds.begin(), instrumentIds.end());
        for (int id : instrumentIds) {
            QString instrumentName = instruments[id];
            if (bassInstruments.contains(id)) {
                instrumentName += " [БАС]";
            }
            instrumentBox->addItem(instrumentName, id);
        }
        
        // Устанавливаем текущий инструмент
        int currentInstrument = wavWriter->getTrackInstrument(i);
        int index = instrumentBox->findData(currentInstrument);
        if (index >= 0) {
            instrumentBox->setCurrentIndex(index);
        }
        
        connect(instrumentBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &Yamitracker::onTrackInstrumentChanged);
        trackInstrumentComboBoxes.append(instrumentBox);
        
        // Громкость
        QSlider* volumeSlider = new QSlider(Qt::Horizontal, trackGroup);
        volumeSlider->setProperty("trackIndex", i);
        volumeSlider->setObjectName(QString("trackVolumeSlider_%1").arg(i));
        volumeSlider->setRange(0, 200);
        volumeSlider->setValue(static_cast<int>(wavWriter->getTrackVolume(i) * 100));
        volumeSlider->setMaximumWidth(100);
        connect(volumeSlider, &QSlider::valueChanged, this, &Yamitracker::onTrackVolumeChanged);
        trackVolumeSliders.append(volumeSlider);
        
        QLabel* volumeLabel = new QLabel(QString("%1%").arg(volumeSlider->value()), trackGroup);
        volumeLabel->setProperty("trackIndex", i);
        volumeLabel->setObjectName(QString("trackVolumeLabel_%1").arg(i));
        volumeLabel->setMaximumWidth(40);
        
        // Кнопка Mute
        QPushButton* muteButton = new QPushButton(wavWriter->isTrackMuted(i) ? "🔇" : "🔊", trackGroup);
        muteButton->setProperty("trackIndex", i);
        muteButton->setObjectName(QString("trackMuteButton_%1").arg(i));
        muteButton->setMaximumWidth(40);
        connect(muteButton, &QPushButton::clicked, this, &Yamitracker::onMuteTrackClicked);
        trackMuteButtons.append(muteButton);
        
        // Кнопка Solo
        QPushButton* soloButton = new QPushButton("🎧", trackGroup);
        soloButton->setProperty("trackIndex", i);
        soloButton->setObjectName(QString("trackSoloButton_%1").arg(i));
        soloButton->setMaximumWidth(40);
        connect(soloButton, &QPushButton::clicked, this, &Yamitracker::onSoloTrackClicked);
        trackSoloButtons.append(soloButton);
        
        // Информация о дорожке
        QLabel* infoLabel = new QLabel(QString("%1 нот").arg(wavWriter->getEventCount(i)), trackGroup);
        infoLabel->setProperty("trackIndex", i);
        infoLabel->setObjectName(QString("trackInfoLabel_%1").arg(i));
        
        // Кнопка выбора для одновременного воспроизведения
        QCheckBox* selectCheck = new QCheckBox("Воспр.", trackGroup);
        selectCheck->setProperty("trackIndex", i);
        selectCheck->setObjectName(QString("trackSelectCheck_%1").arg(i));
        selectCheck->setChecked(true);
        
        // Добавляем элементы в layout
        trackLayout->addWidget(new QLabel("Название:", trackGroup));
        trackLayout->addWidget(nameEdit);
        trackLayout->addWidget(new QLabel("Инструмент:", trackGroup));
        trackLayout->addWidget(instrumentBox);
        trackLayout->addWidget(new QLabel("Громкость:", trackGroup));
        trackLayout->addWidget(volumeSlider);
        trackLayout->addWidget(volumeLabel);
        trackLayout->addWidget(muteButton);
        trackLayout->addWidget(soloButton);
        trackLayout->addWidget(selectCheck);
        trackLayout->addWidget(infoLabel);
        
        tracksLayout->addWidget(trackGroup);
    }
    
    // Добавляем растягивающийся спейсер в конец
    tracksLayout->addStretch();
    
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
}

void Yamitracker::updateTrackControls()
{
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        if (i < trackNameEdits.size()) {
            trackNameEdits[i]->setText(wavWriter->getTrackName(i));
        }
        
        if (i < trackInstrumentComboBoxes.size()) {
            int currentInstrument = wavWriter->getTrackInstrument(i);
            int index = trackInstrumentComboBoxes[i]->findData(currentInstrument);
            if (index >= 0) {
                trackInstrumentComboBoxes[i]->setCurrentIndex(index);
            }
        }
        
        if (i < trackVolumeSliders.size()) {
            trackVolumeSliders[i]->setValue(static_cast<int>(wavWriter->getTrackVolume(i) * 100));
        }
        
        if (i < trackMuteButtons.size()) {
            trackMuteButtons[i]->setText(wavWriter->isTrackMuted(i) ? "🔇" : "🔊");
        }
        
        if (i < trackSoloButtons.size()) {
            // Обновляем текст кнопки solo при необходимости
        }
        
        // Обновляем информацию о дорожке
        QLabel* infoLabel = findChild<QLabel*>(QString("trackInfoLabel_%1").arg(i));
        if (infoLabel) {
            infoLabel->setText(QString("%1 нот").arg(wavWriter->getEventCount(i)));
        }
        
        // Обновляем метку громкости
        QLabel* volumeLabel = findChild<QLabel*>(QString("trackVolumeLabel_%1").arg(i));
        if (volumeLabel) {
            volumeLabel->setText(QString("%1%").arg(trackVolumeSliders[i]->value()));
        }
    }
}

void Yamitracker::updateTrackInfo()
{
    int eventCount = wavWriter->getEventCount(currentTrack);
    QString trackName = wavWriter->getTrackName(currentTrack);
    ui->trackInfoLabel->setText(QString("%1: %2 нот").arg(trackName).arg(eventCount));
}

void Yamitracker::onCurrentTrackChanged(int index)
{
    currentTrack = trackComboBox->itemData(index).toInt();
    updateTrackInfo();
}

void Yamitracker::onPlaybackSpeedChanged(int value)
{
    double speedPercent = value / 100.0;
    wavWriter->setPlaybackSpeed(speedPercent);
    updateSpeedLabel();
}

void Yamitracker::onBassVolumeChanged(int value)
{
    double volumeMultiplier = value / 100.0;
    wavWriter->setBassVolumeMultiplier(volumeMultiplier);
    bassVolumeMultiplier = volumeMultiplier;
    updateVolumeLabel();
}

void Yamitracker::updateSpeedLabel()
{
    if (speedLabel && playbackSpeedSlider) {
        int speed = playbackSpeedSlider->value();
        speedLabel->setText(QString("Скорость: %1%").arg(speed));
    }
}

void Yamitracker::updateVolumeLabel()
{
    if (volumeLabel && bassVolumeSlider) {
        int volume = bassVolumeSlider->value();
        volumeLabel->setText(QString("Бас: %1%").arg(volume));
    }
}

void Yamitracker::onSaveMusicFileClicked()
{
    QString defaultName = "music_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".s";
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить музыкальный файл", defaultName, "ASM файлы (*.s);;Все файлы (*)");
    
    if (!filename.isEmpty()) {
        saveMusicFile(filename);
    }
}

void Yamitracker::saveMusicFile(const QString &filename)
{
    if (wavWriter->saveToMusicFile(filename)) {
        ui->statusLabel->setText(QString("Музыкальный файл сохранен: %1").arg(filename));
        
        // Показываем информацию о сохраненном файле
        QFileInfo fileInfo(filename);
        QString info = QString("Музыкальный файл создан успешно!\n"
                             "Файл: %1\n"
                             "Размер: %2 байт\n"
                             "Формат: Pently-style assembler")
                       .arg(filename)
                       .arg(fileInfo.size());
        QMessageBox::information(this, "Файл сохранен", info);
        
        // Открываем файл для просмотра
        QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
    } else {
        ui->statusLabel->setText("Ошибка сохранения музыкального файла");
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить музыкальный файл");
    }
}

void Yamitracker::onAddTrackClicked()
{
    // Создаем новую дорожку
    int newTrack = -1;
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        if (wavWriter->getEventCount(i) == 0 && wavWriter->getTrackName(i).startsWith("Дорожка")) {
            newTrack = i;
            break;
        }
    }
    
    if (newTrack == -1) {
        QMessageBox::warning(this, "Ошибка", "Достигнуто максимальное количество дорожек");
        return;
    }
    
    // Устанавливаем название новой дорожки
    QString newName = QString("Дорожка %1").arg(newTrack + 1);
    wavWriter->setTrackName(newTrack, newName);
    
    // Переключаемся на новую дорожку
    trackComboBox->setCurrentIndex(newTrack);
    currentTrack = newTrack;
    
    ui->statusLabel->setText(QString("Создана новая дорожка %1").arg(newTrack + 1));
    updateTrackControls();
    updateTrackInfo();
}

void Yamitracker::onRemoveTrackClicked()
{
    if (wavWriter->getEventCount(currentTrack) == 0) {
        QMessageBox::warning(this, "Ошибка", "Дорожка уже пуста");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление дорожки", 
                                  QString("Вы уверены, что хотите удалить дорожку %1?\nВсе данные будут потеряны.")
                                  .arg(currentTrack + 1),
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        wavWriter->clearTrack(currentTrack);
        ui->statusLabel->setText(QString("Дорожка %1 очищена").arg(currentTrack + 1));
        updateTrackInfo();
        updateTrackControls();
    }
}

void Yamitracker::onClearTrackClicked()
{
    if (wavWriter->getEventCount(currentTrack) == 0) {
        ui->statusLabel->setText("Дорожка уже пуста");
        return;
    }
    
    wavWriter->clearTrack(currentTrack);
    ui->statusLabel->setText(QString("Дорожка %1 очищена").arg(currentTrack + 1));
    updateTrackInfo();
    updateTrackControls();
}

void Yamitracker::onPlayTrackClicked()
{
    playTrack(currentTrack);
}

void Yamitracker::onPlayAllTracksClicked()
{
    playAllTracks();
}

void Yamitracker::onStopPlaybackClicked()
{
    stopPlayback();
}

void Yamitracker::playTrack(int track)
{
    if (isPlayingBack) {
        stopPlayback();
    }
    
    QByteArray audioData = wavWriter->getTrackAudioData(track, false);
    if (audioData.isEmpty()) {
        ui->statusLabel->setText(QString("Дорожка %1 пуста").arg(track + 1));
        return;
    }
    
    // Настраиваем аудиовыход с высоким качеством
    QAudioFormat format;
    format.setSampleRate(44100); // Высокое качество
    format.setChannelCount(2);   // Стерео
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        format = info.nearestFormat(format);
    }
    
    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
    }
    if (audioBuffer) {
        delete audioBuffer;
    }
    
    audioOutput = new QAudioOutput(format, this);
    
    // Конвертируем моно в стерео для лучшего качества
    QByteArray stereoData = convertMonoToStereo(audioData);
    audioBuffer = new QBuffer(this);
    audioBuffer->setData(stereoData);
    audioBuffer->open(QIODevice::ReadOnly);
    
    connect(audioOutput, &QAudioOutput::stateChanged, this, &Yamitracker::handleAudioStateChanged);
    
    currentPlaybackTrack = track;
    isPlayingBack = true;
    ui->statusLabel->setText(QString("Воспроизведение дорожки %1...").arg(track + 1));
    
    audioOutput->start(audioBuffer);
    
    // Запускаем таймер для отслеживания завершения воспроизведения
    playbackTimer->start(100);
}

void Yamitracker::playAllTracks()
{
    if (isPlayingBack) {
        stopPlayback();
    }
    
    // Получаем выбранные дорожки для воспроизведения
    QVector<int> selectedTracks;
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        QCheckBox* checkBox = findChild<QCheckBox*>(QString("trackSelectCheck_%1").arg(i));
        if (checkBox && checkBox->isChecked() && !wavWriter->isTrackMuted(i)) {
            selectedTracks.append(i);
        }
    }
    
    if (selectedTracks.isEmpty()) {
        ui->statusLabel->setText("Нет выбранных дорожек для воспроизведения");
        return;
    }
    
    // Смешиваем выбранные дорожки
    QByteArray mixedData = mixSelectedTracks(selectedTracks);
    if (mixedData.isEmpty()) {
        ui->statusLabel->setText("Нет данных для воспроизведения");
        return;
    }
    
    // Настраиваем аудиовыход с высоким качеством
    QAudioFormat format;
    format.setSampleRate(44100); // Высокое качество
    format.setChannelCount(2);   // Стерео
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        format = info.nearestFormat(format);
    }
    
    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
    }
    if (audioBuffer) {
        delete audioBuffer;
    }
    
    audioOutput = new QAudioOutput(format, this);
    
    // Конвертируем моно в стерео для лучшего качества
    QByteArray stereoData = convertMonoToStereo(mixedData);
    audioBuffer = new QBuffer(this);
    audioBuffer->setData(stereoData);
    audioBuffer->open(QIODevice::ReadOnly);
    
    connect(audioOutput, &QAudioOutput::stateChanged, this, &Yamitracker::handleAudioStateChanged);
    
    currentPlaybackTrack = -1; // Все дорожки
    isPlayingBack = true;
    
    QString trackList;
    for (int track : selectedTracks) {
        if (!trackList.isEmpty()) trackList += ", ";
        trackList += QString::number(track + 1);
    }
    ui->statusLabel->setText(QString("Воспроизведение дорожек %1...").arg(trackList));
    
    audioOutput->start(audioBuffer);
    
    // Запускаем таймер для отслеживания завершения воспроизведения
    playbackTimer->start(100);
}

QByteArray Yamitracker::mixSelectedTracks(const QVector<int> &trackIndices)
{
    // Создаем временный WAV writer для микширования
    SimpleWavWriter tempWriter;
    
    // Копируем данные из выбранных дорожек
    for (int trackIndex : trackIndices) {
        // Копируем события из оригинальной дорожки
        // Здесь нужно добавить логику копирования событий
        // Временно используем getAllTracksAudioData
    }
    
    return wavWriter->getAllTracksAudioData();
}

QByteArray Yamitracker::convertMonoToStereo(const QByteArray &monoData)
{
    // Конвертируем моно 16-битные данные в стерео
    const qint16 *monoSamples = reinterpret_cast<const qint16*>(monoData.constData());
    int monoCount = monoData.size() / sizeof(qint16);
    
    QByteArray stereoData(monoCount * 2 * sizeof(qint16), 0);
    qint16 *stereoSamples = reinterpret_cast<qint16*>(stereoData.data());
    
    for (int i = 0; i < monoCount; i++) {
        stereoSamples[i * 2] = monoSamples[i];     // Левый канал
        stereoSamples[i * 2 + 1] = monoSamples[i]; // Правый канал
    }
    
    return stereoData;
}

void Yamitracker::stopPlayback()
{
    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
        audioOutput = nullptr;
    }
    
    if (audioBuffer) {
        audioBuffer->close();
        delete audioBuffer;
        audioBuffer = nullptr;
    }
    
    isPlayingBack = false;
    currentPlaybackTrack = -1;
    ui->statusLabel->setText("Воспроизведение остановлено");
    playbackTimer->stop();
}

void Yamitracker::handleAudioStateChanged(QAudio::State state)
{
    switch (state) {
    case QAudio::IdleState:
        // Воспроизведение завершено
        onPlaybackFinished();
        break;
    case QAudio::StoppedState:
        if (audioOutput && audioOutput->error() != QAudio::NoError) {
            ui->statusLabel->setText("Ошибка воспроизведения");
        }
        break;
    default:
        break;
    }
}

void Yamitracker::onPlaybackFinished()
{
    isPlayingBack = false;
    
    if (currentPlaybackTrack >= 0) {
        ui->statusLabel->setText(QString("Воспроизведение дорожки %1 завершено").arg(currentPlaybackTrack + 1));
    } else if (currentPlaybackTrack == -1) {
        ui->statusLabel->setText("Воспроизведение всех дорожек завершено");
    } else {
        ui->statusLabel->setText("Воспроизведение завершено");
    }
    
    currentPlaybackTrack = -1;
    playbackTimer->stop();
}

void Yamitracker::onLoadWavToTrackClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите WAV файл", "", "WAV файлы (*.wav)");
    
    if (!filename.isEmpty()) {
        if (wavWriter->loadWavToTrack(filename, currentTrack)) {
            ui->statusLabel->setText(QString("WAV файл загружен в дорожку %1").arg(currentTrack + 1));
            updateTrackInfo();
            updateTrackControls();
        } else {
            ui->statusLabel->setText("Ошибка загрузки WAV файла");
        }
    }
}

void Yamitracker::onCopyTrackClicked()
{
    bool ok;
    int targetTrack = QInputDialog::getInt(this, "Копирование дорожки", 
                                          "Введите номер целевой дорожки (1-16):", 
                                          currentTrack + 1, 1, 16, 1, &ok);
    
    if (ok && targetTrack - 1 != currentTrack) {
        wavWriter->copyTrack(currentTrack, targetTrack - 1);
        ui->statusLabel->setText(QString("Дорожка %1 скопирована в дорожку %2")
                                  .arg(currentTrack + 1).arg(targetTrack));
        updateTrackInfo();
        updateTrackControls();
    }
}

void Yamitracker::onMergeTracksClicked()
{
    bool ok;
    int targetTrack = QInputDialog::getInt(this, "Объединение дорожек", 
                                          "Введите номер дорожки для объединения (1-16):", 
                                          currentTrack + 1, 1, 16, 1, &ok);
    
    if (ok && targetTrack - 1 != currentTrack) {
        wavWriter->mergeTrack(currentTrack, targetTrack - 1);
        ui->statusLabel->setText(QString("Дорожка %1 объединена с дорожкой %2")
                                  .arg(currentTrack + 1).arg(targetTrack));
        updateTrackInfo();
        updateTrackControls();
    }
}

void Yamitracker::onMuteTrackClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int track = button->property("trackIndex").toInt();
    bool muted = wavWriter->isTrackMuted(track);
    wavWriter->muteTrack(track, !muted);
    
    button->setText(!muted ? "🔇" : "🔊");
    ui->statusLabel->setText(QString("Дорожка %1 %2")
                              .arg(track + 1)
                              .arg(!muted ? "заглушена" : "включена"));
}

void Yamitracker::onSoloTrackClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int soloTrack = button->property("trackIndex").toInt();
    
    // Выключаем все дорожки кроме выбранной
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        wavWriter->muteTrack(i, i != soloTrack);
        if (i < trackMuteButtons.size()) {
            trackMuteButtons[i]->setText(i == soloTrack ? "🔊" : "🔇");
        }
    }
    
    ui->statusLabel->setText(QString("Только дорожка %1 воспроизводится").arg(soloTrack + 1));
}

void Yamitracker::onTrackVolumeChanged(int value)
{
    QSlider* slider = qobject_cast<QSlider*>(sender());
    if (!slider) return;
    
    int track = slider->property("trackIndex").toInt();
    wavWriter->setTrackVolume(track, value / 100.0);
    
    // Обновляем метку громкости
    QLabel* volumeLabel = findChild<QLabel*>(QString("trackVolumeLabel_%1").arg(track));
    if (volumeLabel) {
        volumeLabel->setText(QString("%1%").arg(value));
    }
}

void Yamitracker::onTrackNameChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (!edit) return;
    
    int track = edit->property("trackIndex").toInt();
    wavWriter->setTrackName(track, edit->text());
    
    // Обновляем комбобокс дорожек
    if (track == currentTrack) {
        trackComboBox->setItemText(trackComboBox->currentIndex(), edit->text());
    }
}

void Yamitracker::onTrackInstrumentChanged(int index)
{
    QComboBox* combo = qobject_cast<QComboBox*>(sender());
    if (!combo) return;
    
    int track = combo->property("trackIndex").toInt();
    int instrument = combo->itemData(index).toInt();
    wavWriter->setTrackInstrument(track, instrument);
    
    ui->statusLabel->setText(QString("Инструмент дорожки %1 изменен на %2")
                              .arg(track + 1)
                              .arg(instruments.value(instrument, "Неизвестный")));
}

void Yamitracker::initializeKeyButtons()
{
    QList<QPushButton*> whiteKeys = {
        ui->key_C1, ui->key_D1, ui->key_E1, ui->key_F1, ui->key_G1, ui->key_A1, ui->key_B1,
        ui->key_C2, ui->key_D2, ui->key_E2, ui->key_F2, ui->key_G2, ui->key_A2, ui->key_B2,
        ui->key_C3, ui->key_D3, ui->key_E3, ui->key_F3, ui->key_G3, ui->key_A3, ui->key_B3,
        ui->key_C4, ui->key_D4, ui->key_E4, ui->key_F4, ui->key_G4, ui->key_A4, ui->key_B4,
        ui->key_C5, ui->key_D5, ui->key_E5, ui->key_F5, ui->key_G5, ui->key_A5, ui->key_B5,
        ui->key_C6
    };

    for (QPushButton *key : whiteKeys) {
        key->setProperty("whiteKey", true);
        keyButtons[key->text()] = key;
    }

    QList<QPushButton*> blackKeys = {
        ui->key_Cs1, ui->key_Ds1, ui->key_Fs1, ui->key_Gs1, ui->key_As1,
        ui->key_Cs2, ui->key_Ds2, ui->key_Fs2, ui->key_Gs2, ui->key_As2,
        ui->key_Cs3, ui->key_Ds3, ui->key_Fs3, ui->key_Gs3, ui->key_As3,
        ui->key_Cs4, ui->key_Ds4, ui->key_Fs4, ui->key_Gs4, ui->key_As4,
        ui->key_Cs5, ui->key_Ds5, ui->key_Fs5, ui->key_Gs5, ui->key_As5
    };

    for (QPushButton *key : blackKeys) {
        key->setProperty("blackKey", true);
        keyButtons[key->text()] = key;
    }
}

QString Yamitracker::getInstrumentName(int program)
{
    return instruments.contains(program) ? instruments[program] : QString("Unknown (%1)").arg(program);
}

void Yamitracker::onNoteOnReceived(const QString &data, int velocity)
{
    QHash<QString, QString> noteMapping = {
        {"24","C1"},{"25","C#1"},{"26","D1"},{"27","D#1"},{"28","E1"},{"29","F1"},{"2A","F#1"},{"2B","G1"},{"2C","G#1"},{"2D","A1"},{"2E","A#1"},{"2F","B1"},
        {"30","C2"},{"31","C#2"},{"32","D2"},{"33","D#2"},{"34","E2"},{"35","F2"},{"36","F#2"},{"37","G2"},{"38","G#2"},{"39","A2"},{"3A","A#2"},{"3B","B2"},
        {"3C","C3"},{"3D","C#3"},{"3E","D3"},{"3F","D#3"},{"40","E3"},{"41","F3"},{"42","F#3"},{"43","G3"},{"44","G#3"},{"45","A3"},{"46","A#3"},{"47","B3"},
        {"48","C4"},{"49","C#4"},{"4A","D4"},{"4B","D#4"},{"4C","E4"},{"4D","F4"},{"4E","F#4"},{"4F","G4"},{"50","G#4"},{"51","A4"},{"52","A#4"},{"53","B4"},
        {"54","C5"},{"55","C#5"},{"56","D5"},{"57","D#5"},{"58","E5"},{"59","F5"},{"5A","F#5"},{"5B","G5"},{"5C","G#5"},{"5D","A5"},{"5E","A#5"},{"5F","B5"},
        {"60","C6"}
    };

    if (!noteMapping.contains(data)) return;

    QString note = noteMapping[data];
    currentlyPressedKeys.insert(note);
    highlightKey(note);

    ui->keysInfoLabel->setText(QString("Нажато клавиш: %1").arg(currentlyPressedKeys.size()));

    int volume = (velocity * 100) / 127;
    ui->volumeBar->setValue(volume);

    if (wavWriter->isRecording()) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        
        bool ok;
        int noteNumber = data.toInt(&ok, 16);
        if (ok) {
            noteStartTimes[data] = currentTime;
            
            // Записываем начало ноты в текущую дорожку
            int currentInstrument = instrumentComboBox->currentData().toInt();
            wavWriter->addNote(currentTrack, currentTime, noteNumber, velocity, 0.1, currentInstrument);
            
            // Обновляем информацию о дорожке
            updateTrackInfo();
            updateTrackControls();
        }
    }
}

void Yamitracker::onNoteOffReceived(const QString &data)
{
    QHash<QString, QString> noteMapping = {
        {"24","C1"},{"25","C#1"},{"26","D1"},{"27","D#1"},{"28","E1"},{"29","F1"},{"2A","F#1"},{"2B","G1"},{"2C","G#1"},{"2D","A1"},{"2E","A#1"},{"2F","B1"},
        {"30","C2"},{"31","C#2"},{"32","D2"},{"33","D#2"},{"34","E2"},{"35","F2"},{"36","F#2"},{"37","G2"},{"38","G#2"},{"39","A2"},{"3A","A#2"},{"3B","B2"},
        {"3C","C3"},{"3D","C#3"},{"3E","D3"},{"3F","D#3"},{"40","E3"},{"41","F3"},{"42","F#3"},{"43","G3"},{"44","G#3"},{"45","A3"},{"46","A#3"},{"47","B3"},
        {"48","C4"},{"49","C#4"},{"4A","D4"},{"4B","D#4"},{"4C","E4"},{"4D","F4"},{"4E","F#4"},{"4F","G4"},{"50","G#4"},{"51","A4"},{"52","A#4"},{"53","B4"},
        {"54","C5"},{"55","C#5"},{"56","D5"},{"57","D#5"},{"58","E5"},{"59","F5"},{"5A","F#5"},{"5B","G5"},{"5C","G#5"},{"5D","A5"},{"5E","A#5"},{"5F","B5"},
        {"60","C6"}
    };

    if (!noteMapping.contains(data)) return;

    QString note = noteMapping[data];
    currentlyPressedKeys.remove(note);
    clearKey(note);

    ui->keysInfoLabel->setText(QString("Нажато клавиш: %1").arg(currentlyPressedKeys.size()));

    if (currentlyPressedKeys.isEmpty()) {
        ui->volumeBar->setValue(0);
    }

    if (wavWriter->isRecording() && noteStartTimes.contains(data)) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        
        bool ok;
        int noteNumber = data.toInt(&ok, 16);
        if (ok) {
            double noteStartTime = noteStartTimes[data];
            double duration = currentTime - noteStartTime;
            
            int noteVelocity = 100;
            
            // Обновляем длительность ноты (в реальном приложении нужно обновлять существующую ноту)
            int currentInstrument = instrumentComboBox->currentData().toInt();
            wavWriter->addNote(currentTrack, noteStartTime, noteNumber, noteVelocity, duration, currentInstrument);
            
            noteStartTimes.remove(data);
            
            // Обновляем информацию о дорожке
            updateTrackInfo();
            updateTrackControls();
        }
    }
}

void Yamitracker::onConnectionStatusChanged(bool connected)
{
    isConnected = connected;
    if (connected) {
        ui->statusLabel->setText("Статус: Подключено к синтезатору");
    } else {
        ui->statusLabel->setText("Статус: Отключено (переподключение...)");
    }
}

void Yamitracker::attemptReconnect()
{
    if (!isConnected && (!serialReader || !serialReader->isRunning())) {
        qDebug() << "Attempting to reconnect...";
        startSerialReader();
    }
}

void Yamitracker::onError(const QString &message)
{
    // НЕ показываем всплывающее окно с ошибкой подключения
    // Просто обновляем статус в строке состояния
    ui->statusLabel->setText("Ошибка: " + message);
    
    // Показываем окно только для критических ошибок (не связанных с подключением)
    if (!message.contains("Cannot open", Qt::CaseInsensitive) && 
        !message.contains("Connection lost", Qt::CaseInsensitive) &&
        !message.contains("reconnect", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Ошибка", message);
    }
}

void Yamitracker::onPlayClicked()
{
    // Получаем выбранный инструмент
    int currentInstrument = instrumentComboBox->currentData().toInt();
    bool isBassInstrument = bassInstruments.contains(currentInstrument);
    
    if (isBassInstrument) {
        ui->statusLabel->setText(QString("Запись с синтезатора [БАС x%1]...").arg(bassVolumeMultiplier));
    } else {
        ui->statusLabel->setText("Запись с синтезатора...");
    }
    
    noteStartTimes.clear();
    currentlyPressedKeys.clear();
    clearAllHighlights();
    
    // Начинаем запись с высоким качеством
    if (wavWriter->startRecording(44100)) {
        recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        qDebug() << "Started WAV recording from synthesizer, track:" << currentTrack 
                 << "sample rate: 44100, instrument:" << currentInstrument;
        if (isBassInstrument) {
            qDebug() << "Bass instrument detected - volume will be increased";
        }
        
        // Активируем кнопку паузы
        ui->pauseButton->setEnabled(true);
    }
}

void Yamitracker::onPauseClicked()
{
    if (wavWriter->isRecording()) {
        if (wavWriter->isPaused()) {
            wavWriter->resumeRecording();
            ui->statusLabel->setText("Запись возобновлена...");
            ui->pauseButton->setText("⏸ Пауза");
        } else {
            wavWriter->pauseRecording();
            ui->statusLabel->setText("Запись приостановлена");
            ui->pauseButton->setText("▶ Продолжить");
        }
    }
}

void Yamitracker::onStopClicked()
{
    ui->statusLabel->setText("Запись остановлена");
    
    clearAllHighlights();
    currentlyPressedKeys.clear();
    ui->keysInfoLabel->setText("Нажато клавиш: 0");
    ui->volumeBar->setValue(0);
    
    if (wavWriter->isRecording()) {
        wavWriter->stopRecording();
        
        // Деактивируем кнопку паузы
        ui->pauseButton->setEnabled(false);
        ui->pauseButton->setText("⏸ Пауза");
        
        // Предлагаем сохранить запись
        onSaveRecordingClicked();
    }
    
    noteStartTimes.clear();
}

void Yamitracker::onSaveRecordingClicked()
{
    QString defaultName = "recording_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".wav";
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить запись", defaultName, "WAV файлы (*.wav)");
    
    if (!filename.isEmpty()) {
        saveWavFile(filename);
    }
}

void Yamitracker::saveWavFile(const QString &filename)
{
    // Проверяем, есть ли данные для сохранения
    bool hasData = false;
    for (int i = 0; i < SimpleWavWriter::MAX_TRACKS; i++) {
        if (wavWriter->getEventCount(i) > 0) {
            hasData = true;
            break;
        }
    }
    
    if (!hasData) {
        QMessageBox::warning(this, "Ошибка", 
            "Нет данных для сохранения. Запишите хотя бы одну ноту на любой дорожке.");
        return;
    }
    
    if (wavWriter->saveToFile(filename)) {
        lastRecordedWavFile = filename;
        ui->statusLabel->setText(QString("Сохранено: %1").arg(filename));
        
        // Показываем информацию о сохраненном файле
        QFileInfo fileInfo(filename);
        QString info = QString("WAV файл создан успешно!\n"
                             "Файл: %1\n"
                             "Размер: %2 байт\n"
                             "Формат: 44100 Гц, 16 бит, моно\n"
                             "Содержит все активные дорожки (микшированные)")
                       .arg(filename)
                       .arg(fileInfo.size());
        QMessageBox::information(this, "Файл сохранен", info);
        
        // Активируем кнопку конвертации
        ui->convertButton->setEnabled(true);
        ui->saveMusicButton->setEnabled(true);
        
        // Предлагаем воспроизвести файл
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Запись завершена",
            QString("WAV файл сохранен:\n%1\n\nВоспроизвести файл?").arg(filename),
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            playWavExternal(lastRecordedWavFile);
        }
    } else {
        ui->statusLabel->setText("Ошибка сохранения WAV файла");
        QMessageBox::warning(this, "Ошибка", 
            "Не удалось сохранить WAV файл. Возможно:\n"
            "1. Нет данных для записи\n"
            "2. Недостаточно прав для записи\n"
            "3. Диск переполнен");
    }
}

void Yamitracker::onConvertToBrrClicked()
{
    if (lastRecordedWavFile.isEmpty() || !QFile::exists(lastRecordedWavFile)) {
        onSelectFileForConversion();
        return;
    }

    QString baseName = QFileInfo(lastRecordedWavFile).completeBaseName();
    QString brrFile = QFileDialog::getSaveFileName(this, "Сохранить BRR файл", baseName + ".brr", "BRR файлы (*.brr)");
    
    if (brrFile.isEmpty()) return;

    ui->statusLabel->setText("Конвертация WAV -> BRR...");
    QApplication::processEvents();

    if (brrConverter->convertWavToBrr(lastRecordedWavFile, brrFile)) {
        ui->statusLabel->setText(QString("Конвертация завершена: %1").arg(brrFile));

        QFileInfo brrInfo(brrFile);
        if (brrInfo.exists()) {
            QString info = QString("BRR файл создан успешно!\n"
                                 "Файл: %1\n"
                                 "Размер: %2 байт\n"
                                 "Формат: Nintendo SNES ADPCM")
                           .arg(brrFile)
                           .arg(brrInfo.size());
            QMessageBox::information(this, "Конвертация завершена", info);
        }
    } else {
        ui->statusLabel->setText("Ошибка конвертации WAV -> BRR");
        QMessageBox::warning(this, "Ошибка", "Не удалось преобразовать WAV в BRR формат");
    }
}

void Yamitracker::onSelectFileForConversion()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите WAV файл для конвертации", "", "WAV файлы (*.wav)");
    
    if (!filename.isEmpty()) {
        lastRecordedWavFile = filename;
        onConvertToBrrClicked();
    }
}

void Yamitracker::onSelectFileForPlayback()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите WAV файл для воспроизведения", "", "WAV файлы (*.wav)");
    
    if (!filename.isEmpty()) {
        playWavExternal(filename);
    }
}

void Yamitracker::playWavExternal(const QString &wavFile)
{
    QStringList players = {
        "aplay", "paplay", "play", "ffplay"
    };
    
    QString player;
    
    for (const QString &p : players) {
        QProcess checkProcess;
        checkProcess.start("which", QStringList() << p);
        checkProcess.waitForFinished();
        if (checkProcess.exitCode() == 0) {
            player = p;
            break;
        }
    }
    
    if (player.isEmpty()) {
        QMessageBox::warning(this, "Воспроизведение", 
            "Не найден аудиоплеер. Установите один из:\n"
            "aplay, paplay, sox или ffmpeg");
        return;
    }
    
    ui->statusLabel->setText("Воспроизведение WAV файла...");
    
    audioPlayerProcess->start(player, QStringList() << wavFile);
    
    if (!audioPlayerProcess->waitForStarted(3000)) {
        ui->statusLabel->setText("Не удалось запустить аудиоплеер");
        QMessageBox::warning(this, "Ошибка", 
            QString("Не удалось запустить %1. Убедитесь, что он установлен.").arg(player));
    }
}

void Yamitracker::scanMidiFiles()
{
    QDir currentDir = QDir::current();
    QStringList filters;
    filters << "*.txt";
    QStringList files = currentDir.entryList(filters, QDir::Files);
    
    midiFilesList->clear();
    for (const QString &file : files) {
        // Проверяем, является ли файл MIDI логом
        if (file.contains("midi_log_") || 
            file.contains("CrandPno") || 
            file.contains("BritePno") || 
            file.contains("Harpsi") ||
            file.contains("FngrBass") ||
            file.contains("Ovrdrive") ||
            file.contains("Bright")) {
            
            QFileInfo fileInfo(file);
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
            item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
            midiFilesList->addItem(item);
        }
    }
    
    if (midiFilesList->count() == 0) {
        QListWidgetItem *item = new QListWidgetItem("MIDI файлы не найдены");
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        midiFilesList->addItem(item);
    }
}

void Yamitracker::onLoadMidiFileClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите MIDI лог файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    
    if (!filename.isEmpty()) {
        loadMidiFile(filename);
    }
}

void Yamitracker::loadMidiFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл: " + filename);
        return;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    currentMidiFilePath = filename;
    parseMidiLog(content);
    
    ui->statusLabel->setText(QString("Загружен файл: %1").arg(QFileInfo(filename).fileName()));
}

void Yamitracker::parseMidiLog(const QString &content)
{
    QStringList lines = content.split('\n');
    int program = 1; // По умолчанию
    
    for (const QString &line : lines) {
        if (line.contains("PROGRAM_CHANGE: Program=")) {
            // Извлекаем номер программы
            QRegularExpression re("Program=(\\d+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                program = match.captured(1).toInt();
                // Обновляем выбранный инструмент в комбобоксе
                int index = instrumentComboBox->findData(program);
                if (index >= 0) {
                    instrumentComboBox->setCurrentIndex(index);
                }
            }
        }
    }
    
    // Показываем информацию о загруженном файле
    QString info = QString("Файл загружен\nИнструмент: %1\nСтрок: %2")
                  .arg(getInstrumentName(program))
                  .arg(lines.count());
    
    QMessageBox::information(this, "MIDI файл загружен", info);
}

void Yamitracker::onPlayMidiFileClicked()
{
    if (currentMidiFilePath.isEmpty()) {
        QMessageBox::information(this, "Воспроизведение", "Сначала загрузите MIDI файл");
        return;
    }
    
    // Проверяем, является ли инструмент басовым
    int currentInstrument = instrumentComboBox->currentData().toInt();
    bool isBassInstrument = bassInstruments.contains(currentInstrument);
    
    if (isBassInstrument) {
        ui->statusLabel->setText(QString("Симуляция MIDI файла [БАС x%1]...").arg(bassVolumeMultiplier));
        QApplication::processEvents();
        
        QMessageBox::information(this, "Симуляция MIDI (БАС)", 
            QString("Симуляция воспроизведения MIDI файла с БАС-инструментом:\n"
                   "Файл: %1\n"
                   "Инструмент: %2\n"
                   "Громкость усилена в %3 раза\n\n"
                   "В реальной версии бас будет звучать громче.")
            .arg(QFileInfo(currentMidiFilePath).fileName())
            .arg(getInstrumentName(currentInstrument))
            .arg(bassVolumeMultiplier));
    } else {
        ui->statusLabel->setText("Симуляция MIDI файла...");
        QApplication::processEvents();
        
        QMessageBox::information(this, "Симуляция MIDI", 
            QString("Симуляция воспроизведения MIDI файла:\n%1\n\nВ реальной версии здесь будет воспроизведение нот.")
            .arg(QFileInfo(currentMidiFilePath).fileName()));
    }
    
    ui->statusLabel->setText("Симуляция завершена");
}

void Yamitracker::simulateNote(int note, int velocity, double duration)
{
    if (wavWriter->isRecording()) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        wavWriter->addNote(currentTrack, currentTime, note, velocity, duration);
    }
}

void Yamitracker::onSelectedMidiFileChanged()
{
    QList<QListWidgetItem*> selectedItems = midiFilesList->selectedItems();
    if (!selectedItems.isEmpty()) {
        QString filePath = selectedItems.first()->data(Qt::UserRole).toString();
        loadMidiFile(filePath);
    }
}

void Yamitracker::onRecordFromMidiFile()
{
    if (currentMidiFilePath.isEmpty()) {
        QMessageBox::information(this, "Запись", "Сначала загрузите MIDI файл");
        return;
    }
    
    // Получаем выбранный инструмент
    int currentInstrument = instrumentComboBox->currentData().toInt();
    bool isBassInstrument = bassInstruments.contains(currentInstrument);
    
    if (isBassInstrument) {
        ui->statusLabel->setText(QString("Запись из MIDI файла [БАС x%1]...").arg(bassVolumeMultiplier));
    } else {
        ui->statusLabel->setText(QString("Запись из MIDI файла [Инструмент: %1]...").arg(getInstrumentName(currentInstrument)));
    }
    
    // Начинаем запись с высоким качеством
    if (wavWriter->startRecording(44100)) {
        recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        qDebug() << "Started WAV recording from MIDI file, instrument:" << currentInstrument << "track:" << currentTrack;
        
        // Генерируем мелодию в зависимости от инструмента
        generateInstrumentMelody(currentInstrument);
        
        wavWriter->stopRecording();
        
        // Предлагаем сохранить
        onSaveRecordingClicked();
    }
}

void Yamitracker::generateInstrumentMelody(int instrument)
{
    double volumeMultiplier = bassInstruments.contains(instrument) ? bassVolumeMultiplier : 1.0;
    
    // Разные мелодии для разных инструментов
    switch (instrument) {
    case 1: // CrandPno (Гранд-пианино)
        wavWriter->addNote(currentTrack, 0.0, 60, 100 * volumeMultiplier, 1.0, instrument); // C4
        wavWriter->addNote(currentTrack, 0.5, 64, 80 * volumeMultiplier, 0.5, instrument);  // E4
        wavWriter->addNote(currentTrack, 1.0, 67, 90 * volumeMultiplier, 1.0, instrument);  // G4
        wavWriter->addNote(currentTrack, 2.0, 72, 100 * volumeMultiplier, 1.0, instrument); // C5
        break;
        
    case 2: // BritePno (Яркое пианино)
        wavWriter->addNote(currentTrack, 0.0, 60, 100 * volumeMultiplier, 0.3, instrument); // C4
        wavWriter->addNote(currentTrack, 0.1, 64, 90 * volumeMultiplier, 0.3, instrument);  // E4
        wavWriter->addNote(currentTrack, 0.2, 67, 80 * volumeMultiplier, 0.3, instrument);  // G4
        wavWriter->addNote(currentTrack, 0.3, 72, 100 * volumeMultiplier, 0.5, instrument); // C5
        break;
        
    case 3: // Harpsi (Клавесин)
    case 6:
        wavWriter->addNote(currentTrack, 0.0, 60, 100 * volumeMultiplier, 0.1, instrument); // C4
        wavWriter->addNote(currentTrack, 0.1, 62, 90 * volumeMultiplier, 0.1, instrument);  // D4
        wavWriter->addNote(currentTrack, 0.2, 64, 100 * volumeMultiplier, 0.1, instrument); // E4
        wavWriter->addNote(currentTrack, 0.3, 65, 90 * volumeMultiplier, 0.1, instrument);  // F4
        wavWriter->addNote(currentTrack, 0.4, 67, 100 * volumeMultiplier, 0.2, instrument); // G4
        break;
        
    case 28: // FngrBass (Фингер-бас)
    case 33:
    case 39:
        wavWriter->addNote(currentTrack, 0.0, 36, 120 * volumeMultiplier, 1.0, instrument); // C2 (бас)
        wavWriter->addNote(currentTrack, 1.0, 40, 100 * volumeMultiplier, 0.5, instrument); // E2
        wavWriter->addNote(currentTrack, 1.5, 43, 110 * volumeMultiplier, 1.0, instrument); // G2
        wavWriter->addNote(currentTrack, 2.5, 48, 100 * volumeMultiplier, 0.5, instrument); // C3
        break;
        
    case 40: // SlapBas (Слэп-бас)
        wavWriter->addNote(currentTrack, 0.0, 36, 150 * volumeMultiplier, 0.2, instrument); // C2 (щелчок)
        wavWriter->addNote(currentTrack, 0.3, 40, 120 * volumeMultiplier, 0.3, instrument); // E2 (пул)
        wavWriter->addNote(currentTrack, 0.7, 43, 140 * volumeMultiplier, 0.2, instrument); // G2 (щелчок)
        break;
        
    case 29: // Ovrdrive (Овердрайв-гитара)
    case 37:
        wavWriter->addNote(currentTrack, 0.0, 40, 100 * volumeMultiplier, 1.5, instrument); // E2
        wavWriter->addNote(currentTrack, 0.5, 43, 90 * volumeMultiplier, 1.0, instrument);  // G2
        wavWriter->addNote(currentTrack, 1.0, 45, 100 * volumeMultiplier, 0.5, instrument); // A2
        wavWriter->addNote(currentTrack, 1.5, 47, 110 * volumeMultiplier, 1.0, instrument); // B2
        break;
        
    case 48: // String (Струнные)
    case 49: // SlowStr (Медленные струнные)
        wavWriter->addNote(currentTrack, 0.0, 60, 80 * volumeMultiplier, 2.0, instrument);  // C4
        wavWriter->addNote(currentTrack, 0.5, 64, 70 * volumeMultiplier, 1.5, instrument);  // E4
        wavWriter->addNote(currentTrack, 1.0, 67, 75 * volumeMultiplier, 2.0, instrument);  // G4
        break;
        
    case 81: // LeadSx (Саксофон)
        wavWriter->addNote(currentTrack, 0.0, 65, 90 * volumeMultiplier, 1.5, instrument);  // F4
        wavWriter->addNote(currentTrack, 0.3, 67, 85 * volumeMultiplier, 0.5, instrument);  // G4
        wavWriter->addNote(currentTrack, 0.8, 69, 95 * volumeMultiplier, 2.0, instrument);  // A4
        break;
        
    case 95: // Bright (Яркий звук)
    case 96:
    case 100:
        wavWriter->addNote(currentTrack, 0.0, 72, 100 * volumeMultiplier, 0.2, instrument); // C5
        wavWriter->addNote(currentTrack, 0.2, 76, 90 * volumeMultiplier, 0.2, instrument);  // E5
        wavWriter->addNote(currentTrack, 0.4, 79, 100 * volumeMultiplier, 0.2, instrument); // G5
        wavWriter->addNote(currentTrack, 0.6, 84, 110 * volumeMultiplier, 0.3, instrument); // C6
        break;
        
    default:
        // Стандартная мелодия для неизвестных инструментов
        wavWriter->addNote(currentTrack, 0.0, 60, 100 * volumeMultiplier, 1.0, instrument); // C4
        wavWriter->addNote(currentTrack, 1.0, 64, 80 * volumeMultiplier, 1.0, instrument);  // E4
        wavWriter->addNote(currentTrack, 2.0, 67, 90 * volumeMultiplier, 1.0, instrument);  // G4
        break;
    }
}

void Yamitracker::highlightKey(const QString &note)
{
    if (keyButtons.contains(note)) {
        QPushButton *key = keyButtons[note];
        key->setStyleSheet("background-color: #ff4444; color: white; border: 2px solid red;");
    }
}

void Yamitracker::clearKey(const QString &note)
{
    if (!keyButtons.contains(note)) return;
    QPushButton *key = keyButtons[note];
    if (key->property("whiteKey").toBool())
        key->setStyleSheet("background-color: white; color: black; border: 1px solid #ccc;");
    else
        key->setStyleSheet("background-color: black; color: white;");
}

void Yamitracker::clearAllHighlights()
{
    for (auto i = keyButtons.begin(); i != keyButtons.end(); ++i) {
        clearKey(i.key());
    }
}

void Yamitracker::clearStatusMessage()
{
    ui->statusLabel->setText("Статус: Готов");
}

void Yamitracker::initializeDeviceDetection()
{
    deviceLabel->setText("Устройство: Поиск...");
}

void Yamitracker::autoDetectAndConnect()
{
    QString device = "/dev/dmmidi1";
    serialReader->setDevice(device);
    deviceLabel->setText(QString("Устройство: %1").arg(device));
    qDebug() << "Using device:" << device;
    
    startSerialReader();
}

void Yamitracker::onDeviceInfoReceived(const QString &manufacturer, const QString &model)
{
    deviceLabel->setText(QString("Устройство: %1 %2").arg(manufacturer, model));
    
    setWindowTitle(QString("Yamitracker - %1 %2").arg(manufacturer, model));
}

void Yamitracker::onRefreshDevicesClicked()
{
    deviceLabel->setText("Устройство: Обновление...");
    
    autoDetectAndConnect();
}

void Yamitracker::startSerialReader()
{
    if (serialReader && !serialReader->isRunning()) {
        serialReader->start();
    }
}

// void Yamitracker::convertToBrr(const QString &wavFile, const QString &brrFile)
// {
//     // Реализация через brrConverter
//     if (brrConverter->convertWavToBrr(wavFile, brrFile)) {
//         ui->statusLabel->setText(QString("Конвертация завершена: %1").arg(brrFile));
//     } else {
//         ui->statusLabel->setText("Ошибка конвертации");
//     }
// }

// Метод onAudioOutputNotify - если он не используется, можно его удалить или оставить пустым
// void Yamitracker::onAudioOutputNotify()
// {
//     // Этот метод может быть пустым, если не используется
//     // Он был добавлен в сигналы, но может не иметь реализации
// }

Yamitracker::~Yamitracker()
{
    stopPlayback();
    
    if (serialReader) {
        serialReader->stop();
        if (serialReader->isRunning()) {
            serialReader->terminate();
            serialReader->wait();
        }
    }
    
    delete ui;
}