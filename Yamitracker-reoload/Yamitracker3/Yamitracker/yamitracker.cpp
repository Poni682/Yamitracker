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
#include <QComboBox>
#include <QSlider>
#include <QLayout>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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
                else if ((byte & 0xF0) == 0xB0) {
                    unsigned char controller, value;
                    if (read(fd, &controller, 1) > 0 && read(fd, &value, 1) > 0) {
                        emit controlChangeReceived(controller, value);
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
{
    ui->setupUi(this);
    
    deviceLabel = ui->deviceLabel;
    refreshDevicesButton = ui->refreshDevicesButton;
    
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
    
    connect(reconnectTimer, &QTimer::timeout, this, &Yamitracker::attemptReconnect);
    reconnectTimer->start(5000); // Попытка переподключения каждые 5 секунд
    
    connect(audioPlayerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode);
                if (exitStatus == QProcess::NormalExit) {
                    ui->statusLabel->setText("Воспроизведение завершено");
                    ui->statusLabel->setStyleSheet("color: white;");
                }
            });
    
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; color: white; }"
        "QLabel { color: white; font-size: 14px; }"
        "QPushButton { border: none; }"
        "QPushButton[whiteKey=true] { background-color: white; color: black; border: 1px solid #ccc; font-size: 8pt; }"
        "QPushButton[whiteKey=true]:pressed { background-color: #ff4444; }"
        "QPushButton[blackKey=true] { background-color: black; color: white; font-size: 7pt; }"
        "QPushButton[blackKey=true]:pressed { background-color: #ff4444; }"
        "QProgressBar { border: 1px solid #555; background: #333; }"
        "QProgressBar::chunk { background: #4CAF50; }"
    );

    QTimer::singleShot(1000, this, &Yamitracker::autoDetectAndConnect);
    setWindowTitle("Yamaha PSR-E333 Monitor");
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
            // Сохраняем полную информацию о ноте
            NoteInfo noteInfo;
            noteInfo.note = noteNumber;
            noteInfo.velocity = velocity;
            noteInfo.startTime = currentTime;
            noteInfo.endTime = 0; // Будет установлено при note off
            noteInfo.isActive = true;
            
            activeNotes[data] = noteInfo;
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

    if (wavWriter->isRecording() && activeNotes.contains(data)) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        
        NoteInfo noteInfo = activeNotes[data];
        noteInfo.endTime = currentTime;
        noteInfo.isActive = false;
        
        // Сохраняем записанную ноту
        recordedNotes.append(noteInfo);
        
        // Удаляем из активных нот
        activeNotes.remove(data);
        
        // Добавляем ноту в WAV writer с правильными данными
        wavWriter->addNote(noteInfo.startTime, noteInfo.note, noteInfo.velocity,
                          noteInfo.endTime - noteInfo.startTime);
    }
}

void Yamitracker::onPlayClicked()
{
    ui->statusLabel->setText("Запись...");
    ui->statusLabel->setStyleSheet("color: blue;");
    
    // Очищаем предыдущие данные
    activeNotes.clear();
    recordedNotes.clear();
    currentlyPressedKeys.clear();
    clearAllHighlights();
    
    // Начинаем запись
    if (wavWriter->startRecording(16000)) {
        recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        qDebug() << "Started WAV recording at" << recordingStartTime;
    }
}

void Yamitracker::onStopClicked()
{
    ui->statusLabel->setText("Остановлено");
    ui->statusLabel->setStyleSheet("color: white;");
    
    clearAllHighlights();
    currentlyPressedKeys.clear();
    ui->keysInfoLabel->setText("Нажато клавиш: 0");
    ui->volumeBar->setValue(0);
    
    if (wavWriter->isRecording()) {
        // Сохраняем все активные ноты (которые не были отпущены)
        for (auto it = activeNotes.begin(); it != activeNotes.end(); ++it) {
            NoteInfo noteInfo = it.value();
            noteInfo.endTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
            noteInfo.isActive = false;
            recordedNotes.append(noteInfo);
        }
        activeNotes.clear();
        
        wavWriter->stopRecording();
        
        // Сохраняем WAV файл
        QString filename = "recording_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".wav";
        saveWavFile(filename);
    }
}

void Yamitracker::attemptReconnect()
{
    if (!isConnected) {
        qDebug() << "Устройство отключено, пытаюсь переподключиться...";
        
        if (serialReader && serialReader->isRunning()) {
            serialReader->stop();
            serialReader->wait();
        }
        
        // Небольшая задержка перед повторной попыткой
        QTimer::singleShot(1000, this, [this]() {
            ui->statusLabel->setText("Попытка переподключения...");
            ui->statusLabel->setStyleSheet("color: orange;");
            
            autoDetectAndConnect();
        });
    }
}
    : QMainWindow(parent)
    , ui(new Ui::Yamitracker)
    , serialReader(new SerialReader(this))
    , reconnectTimer(new QTimer(this))
    , isConnected(false)
    , wavWriter(new SimpleWavWriter())
    , recordingStartTime(0)
    , brrConverter(new BrrConverter(this))
    , audioPlayerProcess(new QProcess(this))
    , currentInstrument(0)
    , reverbLevel(0.0f)
    , chorusLevel(0.0f)
    , synthesizer(new AudioSynthesizer(44100))
{
    recordingStartTime = 0;
    isRecording = false;

    ui->setupUi(this);
    
    deviceLabel = ui->deviceLabel;
    refreshDevicesButton = ui->refreshDevicesButton;
    
    // Создаем кнопку экспорта нот
    exportNotesButton = new QPushButton("Экспорт нот", this);
    ui->controlLayout->addWidget(exportNotesButton);
    
    // Создаем комбо-бокс для выбора инструмента
    QLabel *instrumentLabel = new QLabel("Инструмент:", this);
    instrumentCombo = new QComboBox(this);
    ui->controlLayout->addWidget(instrumentLabel);
    ui->controlLayout->addWidget(instrumentCombo);
    
    // Создаем слайдеры для эффектов
    QLabel *reverbLabel = new QLabel("Реверб:", this);
    reverbSlider = new QSlider(Qt::Horizontal, this);
    reverbSlider->setRange(0, 100);
    reverbSlider->setValue(0);
    ui->controlLayout->addWidget(reverbLabel);
    ui->controlLayout->addWidget(reverbSlider);
    
    QLabel *chorusLabel = new QLabel("Хорус:", this);
    chorusSlider = new QSlider(Qt::Horizontal, this);
    chorusSlider->setRange(0, 100);
    chorusSlider->setValue(0);
    ui->controlLayout->addWidget(chorusLabel);
    ui->controlLayout->addWidget(chorusSlider);
    
    initializeKeyButtons();
    initializeDeviceDetection();
    initializeInstruments();

    connect(serialReader, &SerialReader::noteOnReceived, this, &Yamitracker::onNoteOnReceived);
    connect(serialReader, &SerialReader::noteOffReceived, this, &Yamitracker::onNoteOffReceived);
    connect(serialReader, &SerialReader::controlChangeReceived, this, &Yamitracker::onControlChangeReceived);
    connect(serialReader, &SerialReader::error, this, &Yamitracker::onError);
    connect(serialReader, &SerialReader::connectionStatusChanged, this, &Yamitracker::onConnectionStatusChanged);
    connect(serialReader, &SerialReader::deviceInfoReceived, this, &Yamitracker::onDeviceInfoReceived);
    connect(ui->playButton, &QPushButton::clicked, this, &Yamitracker::onPlayClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &Yamitracker::onStopClicked);
    connect(ui->convertButton, &QPushButton::clicked, this, &Yamitracker::onConvertToBrrClicked);
    connect(exportNotesButton, &QPushButton::clicked, this, &Yamitracker::onExportNotesClicked);
    connect(refreshDevicesButton, &QPushButton::clicked, this, &Yamitracker::onRefreshDevicesClicked);
    connect(instrumentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Yamitracker::onInstrumentChanged);
    connect(reverbSlider, &QSlider::valueChanged, this, &Yamitracker::onReverbChanged);
    connect(chorusSlider, &QSlider::valueChanged, this, &Yamitracker::onChorusChanged);
    
    connect(reconnectTimer, &QTimer::timeout, this, &Yamitracker::attemptReconnect);
    reconnectTimer->start(5000);
    
    connect(audioPlayerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode);
                if (exitStatus == QProcess::NormalExit) {
                    ui->statusLabel->setText("Воспроизведение завершено");
                    ui->statusLabel->setStyleSheet("color: white;");
                }
            });
    
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; color: white; }"
        "QLabel { color: white; font-size: 14px; }"
        "QPushButton { border: none; }"
        "QPushButton[whiteKey=true] { background-color: white; color: black; border: 1px solid #ccc; font-size: 8pt; }"
        "QPushButton[whiteKey=true]:pressed { background-color: #ff4444; }"
        "QPushButton[blackKey=true] { background-color: black; color: white; font-size: 7pt; }"
        "QPushButton[blackKey=true]:pressed { background-color: #ff4444; }"
        "QProgressBar { border: 1px solid #555; background: #333; }"
        "QProgressBar::chunk { background: #4CAF50; }"
        "QComboBox { background-color: #444; color: white; border: 1px solid #666; }"
        "QSlider::groove:horizontal { background: #555; height: 6px; }"
        "QSlider::handle:horizontal { background: #4CAF50; width: 18px; margin: -6px 0; }"
    );

    QTimer::singleShot(1000, this, &Yamitracker::autoDetectAndConnect);
    setWindowTitle("Yamaha PSR-E333 Monitor");
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
    delete synthesizer;
    delete ui;
}

void Yamitracker::initializeInstruments()
{
    // Инструменты Yamaha PSR-E333
    instrumentCombo->addItem("Фортепиано", 0);
    instrumentCombo->addItem("Электропиано", 1);
    instrumentCombo->addItem("Орган", 2);
    instrumentCombo->addItem("Клавесин", 3);
    instrumentCombo->addItem("Скрипка", 4);
    instrumentCombo->addItem("Виолончель", 5);
    instrumentCombo->addItem("Контрабас", 6);
    instrumentCombo->addItem("Флейта", 7);
    instrumentCombo->addItem("Кларнет", 8);
    instrumentCombo->addItem("Саксофон", 9);
    instrumentCombo->addItem("Труба", 10);
    instrumentCombo->addItem("Тромбон", 11);
    instrumentCombo->addItem("Гитара", 12);
    instrumentCombo->addItem("Бас-гитара", 13);
    instrumentCombo->addItem("Ситар", 14);
    instrumentCombo->addItem("Ударные", 15);
    instrumentCombo->addItem("Маракасы", 16);
    instrumentCombo->addItem("Пэд", 17);
    instrumentCombo->addItem("Синтезатор", 18);
    instrumentCombo->addItem("Струнные", 19);
    instrumentCombo->addItem("Хор", 20);
}

void Yamitracker::initializeDeviceDetection()
{
    deviceLabel->setText("Устройство: Поиск...");
    deviceLabel->setStyleSheet("color: orange;");
}

void Yamitracker::autoDetectAndConnect()
{
    QString device = "/dev/dmmidi1";
    serialReader->setDevice(device);
    deviceLabel->setText(QString("Устройство: %1").arg(device));
    deviceLabel->setStyleSheet("color: green;");
    qDebug() << "Using device:" << device;
    
    startSerialReader();
}

void Yamitracker::onDeviceInfoReceived(const QString &manufacturer, const QString &model)
{
    deviceLabel->setText(QString("Устройство: %1 %2").arg(manufacturer, model));
    deviceLabel->setStyleSheet("color: green;");
    
    setWindowTitle(QString("Yamitracker - %1 %2").arg(manufacturer, model));
}

void Yamitracker::onRefreshDevicesClicked()
{
    deviceLabel->setText("Устройство: Обновление...");
    deviceLabel->setStyleSheet("color: orange;");
    
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

    bool ok;
    int noteNumber = data.toInt(&ok, 16);
    if (!ok) return;

    // Всегда добавляем ноту в синтезатор для воспроизведения
    synthesizer->addNote(noteNumber, velocity, currentInstrument);
    
    // Записываем ноту только если идет запись
    if (isRecording) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        
        // Сохраняем информацию о ноте для записи в WAV
        NoteInfo noteInfo;
        noteInfo.note = noteNumber;
        noteInfo.velocity = velocity;
        noteInfo.startTime = currentTime;
        noteInfo.endTime = 0; // Будет установлено при note off
        noteInfo.instrument = currentInstrument;
        noteInfo.isActive = true;
        
        activeNotes[data] = noteInfo;
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

    // Удаляем ноту из синтезатора
    bool ok;
    int noteNumber = data.toInt(&ok, 16);
    if (ok) {
        synthesizer->removeNote(noteNumber);
    }

    // Записываем окончание ноты если идет запись
    if (isRecording && activeNotes.contains(data)) {
        double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
        
        NoteInfo noteInfo = activeNotes[data];
        noteInfo.endTime = currentTime;
        noteInfo.isActive = false;
        
        // Сохраняем записанную ноту
        recordedNotes.append(noteInfo);
        
        // Удаляем из активных нот
        activeNotes.remove(data);
    }
}

void Yamitracker::onControlChangeReceived(int controller, int value)
{
    // Обработка контроллеров MIDI
    switch(controller) {
        case 1: // Modulation Wheel
            // Можно использовать для вибрато
            break;
        case 7: // Volume
            // Управление громкостью
            break;
        case 10: // Pan
            // Панорамирование
            break;
        case 91: // Reverb
            reverbLevel = value / 127.0f;
            synthesizer->setReverbLevel(reverbLevel);
            reverbSlider->setValue(value);
            break;
        case 93: // Chorus
            chorusLevel = value / 127.0f;
            synthesizer->setChorusLevel(chorusLevel);
            chorusSlider->setValue(value);
            break;
    }
}

void Yamitracker::onConnectionStatusChanged(bool connected)
{
    isConnected = connected;
    if (connected) {
        ui->statusLabel->setText("Статус: Подключено к синтезатору");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        ui->statusLabel->setText("Статус: Отключено (переподключение...)");
        ui->statusLabel->setStyleSheet("color: red;");
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
    ui->statusLabel->setStyleSheet("color: orange;");
    if (!message.contains("reconnect", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Ошибка", message);
    }
}

void Yamitracker::onPlayClicked()
{
    ui->statusLabel->setText("Запись...");
    ui->statusLabel->setStyleSheet("color: blue;");
    
    // Очищаем предыдущие данные
    activeNotes.clear();
    recordedNotes.clear();
    currentlyPressedKeys.clear();
    clearAllHighlights();
    synthesizer->clearNotes();
    wavWriter->clear(); // Очищаем WAV writer
    
    // Начинаем запись
    recordingStartTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    isRecording = true; // Устанавливаем флаг записи
    
    qDebug() << "Started recording at time:" << recordingStartTime;
}

void Yamitracker::onStopClicked()
{
    ui->statusLabel->setText("Остановлено");
    ui->statusLabel->setStyleSheet("color: white;");
    
    clearAllHighlights();
    currentlyPressedKeys.clear();
    ui->keysInfoLabel->setText("Нажато клавиш: 0");
    ui->volumeBar->setValue(0);
    synthesizer->clearNotes();
    
    if (isRecording) {
        isRecording = false; // Снимаем флаг записи
        
        // Сохраняем все активные ноты
        for (auto it = activeNotes.begin(); it != activeNotes.end(); ++it) {
            NoteInfo noteInfo = it.value();
            noteInfo.endTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - recordingStartTime;
            noteInfo.isActive = false;
            recordedNotes.append(noteInfo);
        }
        activeNotes.clear();
        
        // Начинаем запись в WAV файл
        wavWriter->startRecording(44100);
        
        // Добавляем все записанные ноты в WAV writer
        for (const NoteInfo &noteInfo : recordedNotes) {
            if (noteInfo.endTime > noteInfo.startTime) {
                wavWriter->addNote(noteInfo.startTime, noteInfo.note, noteInfo.velocity,
                                  noteInfo.endTime - noteInfo.startTime, noteInfo.instrument);
            }
        }
        
        wavWriter->stopRecording();
        
        // Сохраняем WAV файл
        QString filename = "recording_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".wav";
        saveWavFile(filename);
    }
}

void Yamitracker::saveWavFile(const QString &filename)
{
    if (wavWriter->saveToFile(filename)) {
        lastRecordedWavFile = filename;
        ui->statusLabel->setText(QString("Сохранено: %1").arg(filename));
        ui->statusLabel->setStyleSheet("color: green;");
        
        // Активируем кнопки
        ui->convertButton->setEnabled(true);
        exportNotesButton->setEnabled(true);
        
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
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

void Yamitracker::onExportNotesClicked()
{
    if (recordedNotes.isEmpty()) {
        QMessageBox::information(this, "Экспорт нот", "Сначала запишите ноты");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить ноты",
        "notes_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".asm",
        "Assembly файлы (*.asm);;Текстовые файлы (*.txt);;Все файлы (*)");
    
    if (filename.isEmpty()) {
        return;
    }
    
    exportNotesToFile(filename);
}

void Yamitracker::exportNotesToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }
    
    QTextStream out(&file);
    
    // Заголовок
    out << "; ============================================\n";
    out << "; Файл нот, созданный Yamitracker\n";
    out << "; Дата: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";
    out << "; Всего нот: " << recordedNotes.size() << "\n";
    out << "; Формат: команда, freq_low, freq_high, длительность\n";
    out << "; Команды: $01 = нота, $02 = пауза, $FF = конец\n";
    out << "; Длительность: 1 = 1/60 секунды\n";
    out << "; ============================================\n\n";
    
    out << "Song_Data:\n";
    
    // Сортируем ноты по времени начала
    std::sort(recordedNotes.begin(), recordedNotes.end(),
        [](const NoteInfo& a, const NoteInfo& b) {
            return a.startTime < b.startTime;
        });
    
    double lastTime = 0;
    int noteIndex = 0;
    
    for (const NoteInfo& note : recordedNotes) {
        // Добавляем паузу между нотами, если нужно
        if (note.startTime > lastTime) {
            double pauseDuration = note.startTime - lastTime;
            int pauseTicks = static_cast<int>(pauseDuration * 60); // 60 тиков в секунду
            
            if (pauseTicks > 0) {
                out << QString("    .byte $02, $00, $00, $%1  ; Пауза\n")
                    .arg(pauseTicks, 2, 16, QChar('0')).toUpper();
            }
        }
        
        // Конвертируем ноту в частоту
        double frequency = 440.0 * pow(2.0, (note.note - 69) / 12.0);
        int freqInt = static_cast<int>(frequency * 256 / 32000); // Для 8-битного формата
        
        int freqLow = freqInt & 0xFF;
        int freqHigh = (freqInt >> 8) & 0xFF;
        
        // Длительность ноты
        double duration = note.endTime - note.startTime;
        int durationTicks = static_cast<int>(duration * 60);
        
        if (durationTicks < 1) durationTicks = 1;
        if (durationTicks > 255) durationTicks = 255;
        
        out << QString("    .byte $01, $%1, $%2, $%3  ; Нота %4 (MIDI %5)\n")
            .arg(freqLow, 2, 16, QChar('0')).toUpper()
            .arg(freqHigh, 2, 16, QChar('0')).toUpper()
            .arg(durationTicks, 2, 16, QChar('0')).toUpper()
            .arg(noteIndex + 1)
            .arg(note.note);
        
        lastTime = note.endTime;
        noteIndex++;
    }
    
    // Добавляем конечную команду
    out << "    .byte $FF, $00, $00, $00  ; Конец песни\n";
    
    file.close();
    
    QMessageBox::information(this, "Экспорт завершен",
        QString("Ноты успешно экспортированы в файл:\n%1").arg(filename));
    
    ui->statusLabel->setText("Ноты экспортированы");
    ui->statusLabel->setStyleSheet("color: green;");
}

void Yamitracker::onConvertToBrrClicked()
{
    if (lastRecordedWavFile.isEmpty()) {
        QMessageBox::information(this, "Конвертация", "Сначала запишите WAV файл");
        return;
    }

    if (!QFile::exists(lastRecordedWavFile)) {
        QMessageBox::warning(this, "Конвертация", "WAV файл не найден: " + lastRecordedWavFile);
        return;
    }

    // Создаем имя BRR файла
    QString baseName = QFileInfo(lastRecordedWavFile).completeBaseName();
    QString brrFile = baseName + ".brr";

    ui->statusLabel->setText("Конвертация WAV -> BRR...");
    ui->statusLabel->setStyleSheet("color: blue;");
    QApplication::processEvents();

    if (brrConverter->convertWavToBrr(lastRecordedWavFile, brrFile)) {
        ui->statusLabel->setText(QString("Конвертация завершена: %1").arg(brrFile));
        ui->statusLabel->setStyleSheet("color: green;");

        // Показываем информацию о BRR файле
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
        ui->statusLabel->setStyleSheet("color: red;");
        QMessageBox::warning(this, "Ошибка", "Не удалось преобразовать WAV в BRR формат");
    }
}

void Yamitracker::onInstrumentChanged(int index)
{
    currentInstrument = index;
    synthesizer->setInstrument(index);
    
    ui->statusLabel->setText(QString("Инструмент: %1").arg(instrumentCombo->itemText(index)));
    ui->statusLabel->setStyleSheet("color: blue;");
    
    QTimer::singleShot(2000, this, &Yamitracker::clearStatusMessage);
}

void Yamitracker::onReverbChanged(int value)
{
    reverbLevel = value / 100.0f;
    synthesizer->setReverbLevel(reverbLevel);
    updateAudioEffects();
}

void Yamitracker::onChorusChanged(int value)
{
    chorusLevel = value / 100.0f;
    synthesizer->setChorusLevel(chorusLevel);
    updateAudioEffects();
}

void Yamitracker::updateAudioEffects()
{
    ui->statusLabel->setText(QString("Эффекты: Реверб=%1%, Хорус=%2%")
        .arg(static_cast<int>(reverbLevel * 100))
        .arg(static_cast<int>(chorusLevel * 100)));
    ui->statusLabel->setStyleSheet("color: blue;");
    
    QTimer::singleShot(2000, this, &Yamitracker::clearStatusMessage);
}

QString Yamitracker::noteToFrequencyHex(int note)
{
    double frequency = 440.0 * pow(2.0, (note - 69) / 12.0);
    int freqInt = static_cast<int>(frequency * 256 / 32000);
    
    int freqLow = freqInt & 0xFF;
    int freqHigh = (freqInt >> 8) & 0xFF;
    
    return QString("$%1, $%2")
        .arg(freqLow, 2, 16, QChar('0')).toUpper()
        .arg(freqHigh, 2, 16, QChar('0')).toUpper();
}

void Yamitracker::playWavExternal(const QString &wavFile)
{
    QStringList players = {"aplay", "paplay", "play", "ffplay"};
    
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
    ui->statusLabel->setStyleSheet("color: blue;");
    
    audioPlayerProcess->start(player, QStringList() << wavFile);
    
    if (!audioPlayerProcess->waitForStarted(3000)) {
        ui->statusLabel->setText("Не удалось запустить аудиоплеер");
        ui->statusLabel->setStyleSheet("color: red;");
        QMessageBox::warning(this, "Ошибка", 
            QString("Не удалось запустить %1. Убедитесь, что он установлен.").arg(player));
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
    ui->statusLabel->setStyleSheet("color: white;");
}