#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>

// Системные заголовки Linux для работы с последовательным портом
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>

// Структура для отслеживания активной ноты
struct ActiveNote {
    int noteNumber;
    qint64 startTime;
    bool isHeldByPedal; // true, если клавиша отпущена, но педаль держит звук
};

class MidiPedalTester : public QObject {
    Q_OBJECT

public:
    MidiPedalTester(QObject *parent = nullptr) : QObject(parent) {
        fd = -1;
        shouldStop = false;
        sustainPedalPressed = false;

        // Карта названий нот для красивого вывода
        noteNames = {
            {0, "C-1"}, {1, "C#-1"}, {2, "D-1"}, {3, "D#-1"}, {4, "E-1"}, {5, "F-1"},
            {6, "F#-1"}, {7, "G-1"}, {8, "G#-1"}, {9, "A-1"}, {10, "A#-1"}, {11, "B-1"},
            {12, "C0"}, {13, "C#0"}, {14, "D0"}, {15, "D#0"}, {16, "E0"}, {17, "F0"},
            {18, "F#0"}, {19, "G0"}, {20, "G#0"}, {21, "A0"}, {22, "A#0"}, {23, "B0"},
            {24, "C1"}, {25, "C#1"}, {26, "D1"}, {27, "D#1"}, {28, "E1"}, {29, "F1"},
            {30, "F#1"}, {31, "G1"}, {32, "G#1"}, {33, "A1"}, {34, "A#1"}, {35, "B1"},
            {36, "C2"}, {37, "C#2"}, {38, "D2"}, {39, "D#2"}, {40, "E2"}, {41, "F2"},
            {42, "F#2"}, {43, "G2"}, {44, "G#2"}, {45, "A2"}, {46, "A#2"}, {47, "B2"},
            {48, "C3"}, {49, "C#3"}, {50, "D3"}, {51, "D#3"}, {52, "E3"}, {53, "F3"},
            {54, "F#3"}, {55, "G3"}, {56, "G#3"}, {57, "A3"}, {58, "A#3"}, {59, "B3"},
            {60, "C4"}, {61, "C#4"}, {62, "D4"}, {63, "D#4"}, {64, "E4"}, {65, "F4"},
            {66, "F#4"}, {67, "G4"}, {68, "G#4"}, {69, "A4"}, {70, "A#4"}, {71, "B4"},
            {72, "C5"}, {73, "C#5"}, {74, "D5"}, {75, "D#5"}, {76, "E5"}, {77, "F5"},
            {78, "F#5"}, {79, "G5"}, {80, "G#5"}, {81, "A5"}, {82, "A#5"}, {83, "B5"},
            {84, "C6"}
        };
    }

    ~MidiPedalTester() {
        stop();
    }

    void start(const QString &devicePath) {
        device = devicePath;
        QTimer::singleShot(100, this, &MidiPedalTester::runLoop);
    }

    void stop() {
        shouldStop = true;
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }

private:
    int fd;
    QString device;
    bool shouldStop;
    bool sustainPedalPressed;

    // Хранит активные ноты: Key = номер ноты, Value = структура состояния
    QMap<int, ActiveNote> activeNotes;

    // Множество нот, которые ждут затухания (отпущены пальцем, но держатся педалью)
    QSet<int> notesWaitingForDecay;

    QMap<int, QString> noteNames;

    QString getNoteName(int note) {
        return noteNames.value(note, QString("Note %1").arg(note));
    }

    void openPort() {
        if (fd >= 0) return;

        fd = open(device.toUtf8().constData(), O_RDWR | O_NOCTTY);
        if (fd < 0) {
            qWarning() << "Не удалось открыть порт:" << device
                       << ". Проверьте права доступа (sudo chmod 666" << device << ") или подключение.";
            return;
        }

        struct termios tty;
        tcgetattr(fd, &tty);
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);
        tty.c_cflag |= (CLOCAL | CREAD | CS8);
        tty.c_cc[VMIN] = 1;
        tcsetattr(fd, TCSANOW, &tty);

        qDebug() << "Подключено к:" << device;
        qDebug() << "--- Начало мониторинга MIDI (Нажмите Ctrl+C для выхода) ---";
        qDebug() << "Формат: [Статус] Нота - Описание";
    }

    void runLoop() {
        openPort();
        if (fd < 0) {
            QTimer::singleShot(2000, this, &MidiPedalTester::runLoop); // Попытка reconnect
            return;
        }

        unsigned char byte;
        unsigned char lastStatus = 0;

        while (!shouldStop) {
            // Используем select для таймаута, чтобы можно было проверить флаг shouldStop
            fd_set set;
            struct timeval timeout;
            FD_ZERO(&set);
            FD_SET(fd, &set);
            timeout.tv_sec = 0;
            timeout.tv_usec = 10000; // 10ms timeout

            int rv = select(fd + 1, &set, NULL, NULL, &timeout);
            if (rv > 0) {
                if (read(fd, &byte, 1) > 0) {
                    processMidiByte(byte, lastStatus);
                }
            } else if (rv == 0) {
                // Таймаут, ничего не пришло
            } else {
                // Ошибка select
                break;
            }

            if (shouldStop) break;
        }

        if (!shouldStop) {
            QTimer::singleShot(10, this, &MidiPedalTester::runLoop);
        }
    }

    void processMidiByte(unsigned char byte, unsigned char &lastStatus) {
        if (byte == 0xF8 || byte == 0xFE) return; // Игнорируем Timing Clock

        if (byte & 0x80) {
            lastStatus = byte;
        } else {
            if (lastStatus == 0) return;
        }

        unsigned char status = lastStatus;
        unsigned char type = status & 0xF0;

        // Обработка Note On (0x90 - 0x9F)
        if (type == 0x90) {
            unsigned char note, velocity;
            if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                handleNoteOn(note, velocity);
            }
        }
        // Обработка Note Off (0x80 - 0x8F)
        else if (type == 0x80) {
            unsigned char note, velocity;
            if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                handleNoteOff(note);
            }
        }
        // Обработка Control Change (0xB0 - 0xBF)
        else if (type == 0xB0) {
            unsigned char controller, value;
            if (read(fd, &controller, 1) > 0 && read(fd, &value, 1) > 0) {
                handleControlChange(controller, value);
            }
        }
    }

    void handleNoteOn(int note, int velocity) {
        if (velocity == 0) {
            handleNoteOff(note);
            return;
        }

        QString name = getNoteName(note);
        QString pedalMark = sustainPedalPressed ? "(П)" : "";
        QString logLine = QString("[0x90] %1 (%2) - Клавиша %3 нажата %4")
                              .arg(note, 2, 16, QChar('0')).toUpper()
                              .arg(velocity)
                              .arg(name)
                              .arg(pedalMark);

        qDebug() << logLine;

        // Добавляем в активные ноты
        ActiveNote an;
        an.noteNumber = note;
        an.startTime = QDateTime::currentMSecsSinceEpoch();
        an.isHeldByPedal = false;
        activeNotes[note] = an;

        // Если эта нота была в ожидании затухания, убираем её оттуда (повторное нажатие)
        notesWaitingForDecay.remove(note);
    }

    void handleNoteOff(int note) {
        QString name = getNoteName(note);

        if (sustainPedalPressed) {
            // Педаль нажата: звук продолжается, нота переходит в режим ожидания
            notesWaitingForDecay.insert(note);

            // Обновляем статус активной ноты
            if (activeNotes.contains(note)) {
                activeNotes[note].isHeldByPedal = true;
            }

            qDebug() << QString("[0x80] %1 - Клавиша %2 отпущена (П) [Удерживается педалью]")
                            .arg(note, 2, 16, QChar('0')).toUpper()
                            .arg(name);
        } else {
            // Педаль отпущена: звук прекращается сразу
            finalizeNoteDecay(note, false);

            qDebug() << QString("[0x80] %1 - Клавиша %2 отпущена")
                            .arg(note, 2, 16, QChar('0')).toUpper()
                            .arg(name);
        }
    }

    void handleControlChange(int controller, int value) {
        // Контроллер 64 - Sustain Pedal
        if (controller == 64) {
            bool pressed = (value >= 64);

            if (pressed != sustainPedalPressed) {
                sustainPedalPressed = pressed;

                if (!pressed) {
                    // Педаль только что отпущена!
                    // Все ноты, которые ждали затухания, должны затухнуть сейчас
                    qDebug() << "--- ПЕДАЛЬ ОТПУЩЕНА ---";

                    // Копируем множество, так как будем модифицировать его в цикле
                    QSet<int> toDecay = notesWaitingForDecay;
                    for (int note : toDecay) {
                        finalizeNoteDecay(note, true);
                    }
                    notesWaitingForDecay.clear();
                } else {
                    qDebug() << "--- ПЕДАЛЬ НАЖАТА ---";
                }
            }
        }
    }

    void finalizeNoteDecay(int note, bool byPedalRelease) {
        QString name = getNoteName(note);
        QString suffix = byPedalRelease ? "(П)" : "";

        qDebug() << QString("[DECAY] %1 - Клавиша %2 затухла %3")
                        .arg(note, 2, 16, QChar('0')).toUpper()
                        .arg(name)
                        .arg(suffix);

        if (activeNotes.contains(note)) {
            qint64 duration = QDateTime::currentMSecsSinceEpoch() - activeNotes[note].startTime;
            qDebug() << "   Длительность звучания:" << duration << "мс";
            activeNotes.remove(note);
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString device = "/dev/dmmidi1";
    if (argc > 1) {
        device = argv[1];
    }

    qDebug() << "Yamaha PSR-E333 Pedal Tester";
    qDebug() << "Используемое устройство:" << device;


    MidiPedalTester tester;

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&tester]() {
        tester.stop();
    });

    tester.start(device);

    return app.exec();
}
#include "moc_pedal_tester.cpp"
