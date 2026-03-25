#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QHash>
#include <QDateTime>
#include <QLabel>
#include <QProcess>
#include "simplewavwriter.h"
#include "brrconverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Yamitracker; }
QT_END_NAMESPACE

// Структура для хранения информации о ноте (ПЕРЕМЕСТИТЕ В НАЧАЛО ФАЙЛА)
struct NoteInfo {
    int note;           // MIDI номер ноты
    int velocity;       // Громкость (0-127)
    double startTime;   // Время начала
    double endTime;     // Время окончания
    bool isActive;      // Активна ли нота
};

class SerialReader : public QThread
{
    Q_OBJECT
public:
    explicit SerialReader(QObject *parent = nullptr) : QThread(parent) 
    {
        shouldStop = false;
        currentDevice = "/dev/dmmidi1";
    }
    
    void stop() 
    {
        shouldStop = true;
    }
    
    void setDevice(const QString& device) {
        currentDevice = device;
    }
    
    QString getCurrentDevice() const {
        return currentDevice;
    }

protected:
    void run() override;

private:
    bool shouldStop;
    QString currentDevice;

signals:
    void noteOnReceived(const QString &note, int velocity);
    void noteOffReceived(const QString &note);
    void error(const QString &message);
    void connectionStatusChanged(bool connected);
    void deviceInfoReceived(const QString &manufacturer, const QString &model);
    void controlChangeReceived(int controller, int value); // ТОЛЬКО ОДИН РАЗ!
};

class Yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    Yamitracker(QWidget *parent = nullptr);
    ~Yamitracker();

private slots:
    void onNoteOnReceived(const QString &note, int velocity);
    void onNoteOffReceived(const QString &note);
    void onControlChangeReceived(int controller, int value);
    void onError(const QString &message);
    void onPlayClicked();
    void onStopClicked();
    void onConvertToBrrClicked();
    void onExportNotesClicked();
    void clearStatusMessage();
    void onConnectionStatusChanged(bool connected);
    void attemptReconnect();
    void onDeviceInfoReceived(const QString &manufacturer, const QString &model);
    void onRefreshDevicesClicked();

private:
    Ui::Yamitracker *ui;
    SerialReader *serialReader;
    QHash<QString, QPushButton*> keyButtons;
    QSet<QString> currentlyPressedKeys;
    QTimer *reconnectTimer;
    bool isConnected;
    SimpleWavWriter *wavWriter;
    double recordingStartTime;
    QHash<QString, NoteInfo> activeNotes;
    QVector<NoteInfo> recordedNotes;
    QLabel *deviceLabel;
    QPushButton *refreshDevicesButton;
    QPushButton *exportNotesButton;
    BrrConverter *brrConverter;
    QProcess *audioPlayerProcess;
    QString lastRecordedWavFile;

    void initializeKeyButtons();
    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();
    void startSerialReader();
    void saveWavFile(const QString &filename);
    void initializeDeviceDetection();
    void autoDetectAndConnect();
    void convertToBrr(const QString &wavFile, const QString &brrFile);
    void playWavExternal(const QString &wavFile);
    void exportNotesToFile(const QString &filename);
    QString noteToFrequencyHex(int note);
};

#endif