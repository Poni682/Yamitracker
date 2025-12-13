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
#include "simplemidiwriter.h"
#include "yamimedia.h"
#include "mididevicedetector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Yamitracker; }
QT_END_NAMESPACE

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
    void onError(const QString &message);
    void onPlayClicked();
    void onStopClicked();
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
    SimpleMidiWriter *midiWriter;
    yami::YamiFile *yamiFile;
    double recordingStartTime;
    QHash<QString, yami::Note*> activeNotes;
    MidiDeviceDetector *deviceDetector;
    QLabel *deviceLabel;
    QPushButton *refreshDevicesButton;

    void initializeKeyButtons();
    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();
    void startSerialReader();
    void saveAllFormats(const QString &baseFilename);
    void initializeDeviceDetection();
    void autoDetectAndConnect();
};

#endif