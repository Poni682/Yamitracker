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
#include <QComboBox>
#include <QListWidget>
#include <QFileDialog>
#include <QSlider>
#include <QSpinBox>
#include <QAudioOutput>
#include <QBuffer>
#include "simplewavwriter.h"
#include "brrconverter.h"

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
    void onPauseClicked();
    void onStopClicked();
    void onConvertToBrrClicked();
    void clearStatusMessage();
    void onConnectionStatusChanged(bool connected);
    void attemptReconnect();
    void onDeviceInfoReceived(const QString &manufacturer, const QString &model);
    void onRefreshDevicesClicked();
    void onLoadMidiFileClicked();
    void onSelectFileForConversion();
    void onSelectFileForPlayback();
    void onSaveRecordingClicked();
    void onPlayMidiFileClicked();
    void onSelectedMidiFileChanged();
    void onRecordFromMidiFile();
    void onPlaybackSpeedChanged(int value);
    void onBassVolumeChanged(int value);
    void updateSpeedLabel();
    void updateVolumeLabel();
    void onCurrentTrackChanged(int index);
    void onSaveMusicFileClicked();
    void onAddTrackClicked();
    void onRemoveTrackClicked();
    void onClearTrackClicked();
    void onPlayTrackClicked();
    void onPlayAllTracksClicked();
    void onStopPlaybackClicked();
    void onLoadWavToTrackClicked();
    void onMergeTracksClicked();
    void onCopyTrackClicked();
    void onMuteTrackClicked();
    void onSoloTrackClicked();
    void onTrackVolumeChanged(int value);
    void onTrackNameChanged();
    void onTrackInstrumentChanged(int index);
    
    // Аудио обработчики
    void handleAudioStateChanged(QAudio::State state);
    void onAudioOutputNotify();
    void onPlaybackFinished();

private:
    Ui::Yamitracker *ui;
    SerialReader *serialReader;
    QHash<QString, QPushButton*> keyButtons;
    QSet<QString> currentlyPressedKeys;
    QTimer *reconnectTimer;
    bool isConnected;
    SimpleWavWriter *wavWriter;
    double recordingStartTime;
    QHash<QString, double> noteStartTimes;
    QLabel *deviceLabel;
    QPushButton *refreshDevicesButton;
    BrrConverter *brrConverter;
    QProcess *audioPlayerProcess;
    QString lastRecordedWavFile;
    QListWidget *midiFilesList;
    QComboBox *instrumentComboBox;
    QString currentMidiFilePath;
    QSlider *playbackSpeedSlider;
    QSlider *bassVolumeSlider;
    QLabel *speedLabel;
    QLabel *volumeLabel;
    QComboBox *trackComboBox;
    QSpinBox *trackCountSpinBox;
    
    // Аудио воспроизведение
    QAudioOutput *audioOutput;
    QBuffer *audioBuffer;
    QTimer *playbackTimer;
    bool isPlayingBack;
    int currentPlaybackTrack;
    
    // Элементы управления дорожками
    QVector<QSlider*> trackVolumeSliders;
    QVector<QPushButton*> trackMuteButtons;
    QVector<QPushButton*> trackSoloButtons;
    QVector<QLineEdit*> trackNameEdits;
    QVector<QComboBox*> trackInstrumentComboBoxes;
    
    int currentTrack; // Текущая выбранная дорожка
    
    // Инструменты (темпы) Yamaha PSR-E333
    QMap<int, QString> instruments = {
        {1, "CrandPno (Гранд-пианино)"},
        {2, "BritePno (Яркое пианино)"},
        {3, "Harpsi (Клавесин)"},
        {6, "Harpsi (Клавесин) - другой тип"},
        {25, "ClassicGt (Классическая гитара)"},
        {27, "JazzGtr (Джазовая гитара)"},
        {28, "FngrBass (Фингер-бас)"},
        {29, "Ovrdrive (Овердрайв-гитара)"},
        {33, "FngrBass (Фингер-бас) - другой тип"},
        {37, "Ovrdrive (Овердрайв-гитара) - другой тип"},
        {38, "DstGtr (Дисторшн-гитара)"},
        {39, "FngrBass (Фингер-бас) - другой тип"},
        {40, "SlapBas (Слэп-бас)"},
        {48, "String (Струнные)"},
        {49, "SlowStr (Медленные струнные)"},
        {81, "LeadSx (Саксофон)"},
        {89, "SoftPad (Мягкий пад)"},
        {95, "Bright (Яркий звук)"},
        {96, "Bright (Яркий звук) - другой тип"},
        {100, "Bright (Яркий звук) - еще тип"}
    };

    void initializeKeyButtons();
    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();
    void startSerialReader();
    void saveWavFile(const QString &filename);
    void saveMusicFile(const QString &filename);
    void initializeDeviceDetection();
    void autoDetectAndConnect();
    void convertToBrr(const QString &wavFile, const QString &brrFile);
    void playWavExternal(const QString &wavFile);
    void loadMidiFile(const QString &filename);
    void parseMidiLog(const QString &content);
    void simulateNote(int note, int velocity, double duration);
    void scanMidiFiles();
    QString getInstrumentName(int program);
    void generateInstrumentMelody(int instrument);
    void updateTrackInfo();
    void initializeTracks();
    void initializeTrackControls();
    void updateTrackControls();
    void playTrack(int track);
    void playAllTracks();
    void stopPlayback();
    void loadWavToCurrentTrack();
    void mergeCurrentTrackWith(int targetTrack);
    void copyCurrentTrackTo(int targetTrack);
    void muteCurrentTrack(bool muted);
    void soloCurrentTrack(bool solo);
    void setCurrentTrackVolume(int volume);
    void setCurrentTrackName(const QString &name);
    void setCurrentTrackInstrument(int instrument);
    
    // Вспомогательные методы
    void showTrackContextMenu(const QPoint &pos);
    void createNewTrack();
    void deleteTrack(int track);
    void exportTrack(int track, const QString &filename);
    void importTrack(int track, const QString &filename);
};

#endif