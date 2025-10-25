#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QHash>

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
    }
    
    void stop() 
    {
        shouldStop = true;
    }

protected:
    void run() override;

private:
    bool shouldStop;

signals:
    void noteOnReceived(const QString &note, int velocity);
    void noteOffReceived(const QString &note);
    void error(const QString &message);
    void connectionStatusChanged(bool connected);
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

private:
    Ui::Yamitracker *ui;
    SerialReader *serialReader;
    QHash<QString, QPushButton*> keyButtons;
    QSet<QString> currentlyPressedKeys;
    QTimer *reconnectTimer;
    bool isConnected;

    void initializeKeyButtons();
    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();
    void startSerialReader();
};

#endif // YAMITRACKER_H