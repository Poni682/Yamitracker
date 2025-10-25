#include "yamitracker.h"
#include "ui_yamitracker.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

void SerialReader::run()
{
    while (!shouldStop) {
        int fd = open("/dev/dmmidi1", O_RDWR | O_NOCTTY);
        if (fd < 0) {
            emit error("Cannot open /dev/dmmidi1");
            emit connectionStatusChanged(false);
            // Ждем перед повторной попыткой
            sleep(2);
            continue;
        }

        emit connectionStatusChanged(true);

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
            emit error("Connection lost. Attempting to reconnect...");
            sleep(2); // Ждем перед повторной попыткой подключения
        }
    }
}

Yamitracker::Yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Yamitracker)
    , serialReader(new SerialReader(this))
    , reconnectTimer(new QTimer(this))
    , isConnected(false)
{
    ui->setupUi(this);
    initializeKeyButtons();

    connect(serialReader, &SerialReader::noteOnReceived, this, &Yamitracker::onNoteOnReceived);
    connect(serialReader, &SerialReader::noteOffReceived, this, &Yamitracker::onNoteOffReceived);
    connect(serialReader, &SerialReader::error, this, &Yamitracker::onError);
    connect(serialReader, &SerialReader::connectionStatusChanged, this, &Yamitracker::onConnectionStatusChanged);
    connect(ui->playButton, &QPushButton::clicked, this, &Yamitracker::onPlayClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &Yamitracker::onStopClicked);
    
    // Таймер для периодической проверки соединения
    connect(reconnectTimer, &QTimer::timeout, this, &Yamitracker::attemptReconnect);
    reconnectTimer->start(5000); // Проверка каждые 5 секунд

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

    startSerialReader();
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
    delete ui;
}

void Yamitracker::startSerialReader()
{
    if (serialReader && !serialReader->isRunning()) {
        serialReader->start();
    }
}

void Yamitracker::initializeKeyButtons()
{
    // ... существующий код без изменений ...
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
    // ... существующий код без изменений ...
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
}

void Yamitracker::onNoteOffReceived(const QString &data)
{
    // ... существующий код без изменений ...
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
    // Не показываем MessageBox для ошибок подключения, чтобы не раздражать пользователя
    if (!message.contains("reconnect", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Ошибка", message);
    }
}

void Yamitracker::onPlayClicked()
{
    ui->statusLabel->setText("Воспроизведение...");
    ui->statusLabel->setStyleSheet("color: blue;");
}

void Yamitracker::onStopClicked()
{
    ui->statusLabel->setText("Остановлено");
    ui->statusLabel->setStyleSheet("color: white;");
    clearAllHighlights();
    currentlyPressedKeys.clear();
    ui->keysInfoLabel->setText("Нажато клавиш: 0");
    ui->volumeBar->setValue(0);
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