#include "yamitracker.h"
#include "ui_yamitracker.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>

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
    , wavWriter(new SimpleWavWriter())
    , recordingStartTime(0)
    , brrConverter(new BrrConverter(this))
    , audioPlayerProcess(new QProcess(this))
    , playbackSpeedSlider(nullptr)
    , bassVolumeSlider(nullptr)
    , speedLabel(nullptr)
    , volumeLabel(nullptr)
{
    ui->setupUi(this);
    
    deviceLabel = ui->deviceLabel;
    refreshDevicesButton = ui->refreshDevicesButton;
    midiFilesList = ui->midiFilesList;
    instrumentComboBox = ui->instrumentComboBox;

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
    
    // Обновляем метки
    updateSpeedLabel();
    updateVolumeLabel();
    
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

    connect(serialReader, &SerialReader::noteOnReceived, this, &Yamitracker::onNoteOnReceived);
    connect(serialReader, &SerialReader::noteOffReceived, this, &Yamitracker::onNoteOffReceived);
    connect(serialReader, &SerialReader::error, this, &Yamitracker::onError);
    connect(serialReader, &SerialReader::connectionStatusChanged, this, &Yamitracker::onConnectionStatusChanged);
    connect(serialReader, &SerialReader::deviceInfoReceived, this, &Yamitracker::onDeviceInfoReceived);
    connect(ui->playButton, &QPushButton::clicked, this, &Yamitracker::onPlayClicked);
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
    
    connect(reconnectTimer, &QTimer::timeout, this, &Yamitracker::attemptReconnect);
    reconnectTimer->start(5000);
    
    connect(audioPlayerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode);
                if (exitStatus == QProcess::NormalExit) {
                    ui->statusLabel->setText("Воспроизведение завершено");
                }
            });
    
    // ОБНОВЛЕННЫЙ СТИЛЬ - белый текст на темном фоне для лучшей читаемости
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
    );

    QTimer::singleShot(1000, this, &Yamitracker::autoDetectAndConnect);
    setWindowTitle("Yamaha PSR-E333 Monitor - Bass Enhanced");
    
    // Сканируем MIDI файлы при запуске
    scanMidiFiles();
    
    // Информация о басовых инструментах
    qDebug() << "Бас-инструменты усилены в" << bassVolumeMultiplier << "раз";
    qDebug() << "Бас-инструменты: FngrBass(28,33,39), SlapBas(40), Ovrdrive(29,37), DstGtr(38)";
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
    
    // Начинаем запись
    if (wavWriter->startRecording(16000)) {
        recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        qDebug() << "Started WAV recording from MIDI file, instrument:" << currentInstrument;
        
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
        wavWriter->addNote(0.0, 60, 100 * volumeMultiplier, 1.0, instrument); // C4
        wavWriter->addNote(0.5, 64, 80 * volumeMultiplier, 0.5, instrument);  // E4
        wavWriter->addNote(1.0, 67, 90 * volumeMultiplier, 1.0, instrument);  // G4
        wavWriter->addNote(2.0, 72, 100 * volumeMultiplier, 1.0, instrument); // C5
        break;
        
    case 2: // BritePno (Яркое пианино)
        wavWriter->addNote(0.0, 60, 100 * volumeMultiplier, 0.3, instrument); // C4
        wavWriter->addNote(0.1, 64, 90 * volumeMultiplier, 0.3, instrument);  // E4
        wavWriter->addNote(0.2, 67, 80 * volumeMultiplier, 0.3, instrument);  // G4
        wavWriter->addNote(0.3, 72, 100 * volumeMultiplier, 0.5, instrument); // C5
        break;
        
    case 3: // Harpsi (Клавесин)
    case 6:
        wavWriter->addNote(0.0, 60, 100 * volumeMultiplier, 0.1, instrument); // C4
        wavWriter->addNote(0.1, 62, 90 * volumeMultiplier, 0.1, instrument);  // D4
        wavWriter->addNote(0.2, 64, 100 * volumeMultiplier, 0.1, instrument); // E4
        wavWriter->addNote(0.3, 65, 90 * volumeMultiplier, 0.1, instrument);  // F4
        wavWriter->addNote(0.4, 67, 100 * volumeMultiplier, 0.2, instrument); // G4
        break;
        
    case 28: // FngrBass (Фингер-бас)
    case 33:
    case 39:
        wavWriter->addNote(0.0, 36, 120 * volumeMultiplier, 1.0, instrument); // C2 (бас)
        wavWriter->addNote(1.0, 40, 100 * volumeMultiplier, 0.5, instrument); // E2
        wavWriter->addNote(1.5, 43, 110 * volumeMultiplier, 1.0, instrument); // G2
        wavWriter->addNote(2.5, 48, 100 * volumeMultiplier, 0.5, instrument); // C3
        break;
        
    case 40: // SlapBas (Слэп-бас)
        wavWriter->addNote(0.0, 36, 150 * volumeMultiplier, 0.2, instrument); // C2 (щелчок)
        wavWriter->addNote(0.3, 40, 120 * volumeMultiplier, 0.3, instrument); // E2 (пул)
        wavWriter->addNote(0.7, 43, 140 * volumeMultiplier, 0.2, instrument); // G2 (щелчок)
        break;
        
    case 29: // Ovrdrive (Овердрайв-гитара)
    case 37:
        wavWriter->addNote(0.0, 40, 100 * volumeMultiplier, 1.5, instrument); // E2
        wavWriter->addNote(0.5, 43, 90 * volumeMultiplier, 1.0, instrument);  // G2
        wavWriter->addNote(1.0, 45, 100 * volumeMultiplier, 0.5, instrument); // A2
        wavWriter->addNote(1.5, 47, 110 * volumeMultiplier, 1.0, instrument); // B2
        break;
        
    case 48: // String (Струнные)
    case 49: // SlowStr (Медленные струнные)
        wavWriter->addNote(0.0, 60, 80 * volumeMultiplier, 2.0, instrument);  // C4
        wavWriter->addNote(0.5, 64, 70 * volumeMultiplier, 1.5, instrument);  // E4
        wavWriter->addNote(1.0, 67, 75 * volumeMultiplier, 2.0, instrument);  // G4
        break;
        
    case 81: // LeadSx (Саксофон)
        wavWriter->addNote(0.0, 65, 90 * volumeMultiplier, 1.5, instrument);  // F4
        wavWriter->addNote(0.3, 67, 85 * volumeMultiplier, 0.5, instrument);  // G4
        wavWriter->addNote(0.8, 69, 95 * volumeMultiplier, 2.0, instrument);  // A4
        break;
        
    case 95: // Bright (Яркий звук)
    case 96:
    case 100:
        wavWriter->addNote(0.0, 72, 100 * volumeMultiplier, 0.2, instrument); // C5
        wavWriter->addNote(0.2, 76, 90 * volumeMultiplier, 0.2, instrument);  // E5
        wavWriter->addNote(0.4, 79, 100 * volumeMultiplier, 0.2, instrument); // G5
        wavWriter->addNote(0.6, 84, 110 * volumeMultiplier, 0.3, instrument); // C6
        break;
        
    default:
        // Стандартная мелодия для неизвестных инструментов
        wavWriter->addNote(0.0, 60, 100 * volumeMultiplier, 1.0, instrument); // C4
        wavWriter->addNote(1.0, 64, 80 * volumeMultiplier, 1.0, instrument);  // E4
        wavWriter->addNote(2.0, 67, 90 * volumeMultiplier, 1.0, instrument);  // G4
        break;
    }
}

Yamitracker::~Yamitracker()
{
    if (serialReader) {
        serialReader->stop();
        if (serialReader->isRunning()) {
            serialReader->terminate();
            serialReader->wait();
        }
    }
    delete wavWriter;
    delete ui;
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
            
            wavWriter->addNote(noteStartTime, noteNumber, noteVelocity, duration);
            
            noteStartTimes.remove(data);
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
    ui->statusLabel->setText("Ошибка: " + message);
    if (!message.contains("reconnect", Qt::CaseInsensitive)) {
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
    
    if (wavWriter->startRecording(16000)) {
        recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        qDebug() << "Started WAV recording from synthesizer";
        if (isBassInstrument) {
            qDebug() << "Bass instrument detected - volume will be increased";
        }
    }
}

void Yamitracker::onStopClicked()
{
    ui->statusLabel->setText("Остановлено");
    
    clearAllHighlights();
    currentlyPressedKeys.clear();
    ui->keysInfoLabel->setText("Нажато клавиш: 0");
    ui->volumeBar->setValue(0);
    
    if (wavWriter->isRecording()) {
        wavWriter->stopRecording();
        
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
    if (wavWriter->saveToFile(filename)) {
        lastRecordedWavFile = filename;
        ui->statusLabel->setText(QString("Сохранено: %1").arg(filename));
        
        // Активируем кнопку конвертации
        ui->convertButton->setEnabled(true);
        
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
        wavWriter->addNote(currentTime, note, velocity, duration);
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