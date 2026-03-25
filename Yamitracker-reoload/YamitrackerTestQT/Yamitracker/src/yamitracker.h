// yamitracker.h
#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QProcess>
#include <QAudioOutput>
#include <QBuffer>
#include <QTimer>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QThread>
#include <QLabel>
#include <QSlider>
#include "simplewavwriter.h"
#include "brrconverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Yamitracker; }
QT_END_NAMESPACE

class SerialReader : public QThread
{
    Q_OBJECT

public:
    SerialReader(QObject *parent = nullptr) : QThread(parent), shouldStop(false) {}
    void setDevice(const QString &device) { currentDevice = device; }
    void stop() { shouldStop = true; }

signals:
    void noteOnReceived(const QString &data, int velocity);
    void noteOffReceived(const QString &data);
    void error(const QString &message);
    void connectionStatusChanged(bool connected);
    void deviceInfoReceived(const QString &manufacturer, const QString &model);

protected:
    void run() override;

private:
    QString currentDevice;
    bool shouldStop;
};

class Yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    Yamitracker(QWidget *parent = nullptr);
    ~Yamitracker();

private slots:
    // Основные слоты
    void onPlayClicked();
    void onPauseClicked();
    void onStopClicked();
    void onSaveRecordingClicked();
    void onConvertToBrrClicked();
    void onSaveMusicFileClicked();

    // Слоты для устройств и MIDI
    void onRefreshDevicesClicked();
    void onLoadMidiFileClicked();
    void onPlayMidiFileClicked();
    void onRecordFromMidiFile();
    void onSelectedMidiFileChanged();

    // Слоты для файлов
    void onSelectFileForConversion();
    void onSelectFileForPlayback();

    // Слоты для клавиш и устройств
    void onNoteOnReceived(const QString &data, int velocity);
    void onNoteOffReceived(const QString &data);
    void onConnectionStatusChanged(bool connected);
    void onError(const QString &message);
    void onDeviceInfoReceived(const QString &manufacturer, const QString &model);

    // Слоты для управления дорожками
    void onCurrentTrackChanged(int index);
    void onAddTrackClicked();
    void onRemoveTrackClicked();
    void onClearTrackClicked();
    void onCopyTrackClicked();
    void onMergeTracksClicked();
    void onLoadWavToTrackClicked();

    // Слоты для воспроизведения
    void onPlayTrackClicked();
    void onPlayAllTracksClicked();
    void onStopPlaybackClicked();
    void onPlaybackFinished();

    // Слоты для управления элементами дорожек
    void onMuteTrackClicked();
    void onSoloTrackClicked();
    void onTrackVolumeChanged(int value);
    void onTrackNameChanged();
    void onTrackInstrumentChanged(int index);

    // Слоты для эффектов
    void onPlaybackSpeedChanged(int value);
    void onBassVolumeChanged(int value);

    // Вспомогательные слоты
    void attemptReconnect();
    void handleAudioStateChanged(QAudio::State state);
    void clearStatusMessage();

private:
    Ui::Yamitracker *ui;
    SerialReader *serialReader;
    QTimer *reconnectTimer;
    bool isConnected;
    SimpleWavWriter *wavWriter;
    double recordingStartTime;
    BrrConverter *brrConverter;
    QProcess *audioPlayerProcess;
    QTimer *playbackTimer;

    // Элементы UI
    QSlider *playbackSpeedSlider;
    QSlider *bassVolumeSlider;
    QLabel *speedLabel;
    QLabel *volumeLabel;
    QLabel *deviceLabel;
    QPushButton *refreshDevicesButton;
    QListWidget *midiFilesList;
    QComboBox *instrumentComboBox;
    QComboBox *trackComboBox;

    // Аудио воспроизведение
    QAudioOutput *audioOutput;
    QBuffer *audioBuffer;
    bool isPlayingBack;
    int currentPlaybackTrack;
    int currentTrack;

    // Данные
    QString lastRecordedWavFile;
    QString currentMidiFilePath;
    QHash<QString, QPushButton*> keyButtons;
    QSet<QString> currentlyPressedKeys;
    QHash<QString, double> noteStartTimes;

    // Управление дорожками
    QVector<QLineEdit*> trackNameEdits;
    QVector<QComboBox*> trackInstrumentComboBoxes;
    QVector<QSlider*> trackVolumeSliders;
    QVector<QLabel*> trackVolumeLabels;
    QVector<QPushButton*> trackMuteButtons;
    QVector<QPushButton*> trackSoloButtons;

    // Вспомогательные методы
    void initializeTracks();
    void initializeTrackControls();
    void updateTrackControls();
    void updateTrackInfo();
    void updateSpeedLabel();
    void updateVolumeLabel();

    void initializeKeyButtons();
    void initializeDeviceDetection();
    void autoDetectAndConnect();
    void startSerialReader();

    void highlightKey(const QString &note);
    void clearKey(const QString &note);
    void clearAllHighlights();

    void scanMidiFiles();
    void loadMidiFile(const QString &filename);
    void parseMidiLog(const QString &content);
    void generateInstrumentMelody(int instrument);
    void simulateNote(int note, int velocity, double duration);

    void saveWavFile(const QString &filename);
    void saveMusicFile(const QString &filename);
    void playWavExternal(const QString &wavFile);
    void playTrack(int track);
    void playAllTracks();
    void stopPlayback();

    QByteArray mixSelectedTracks(const QVector<int> &trackIndices);
    QByteArray convertMonoToStereo(const QByteArray &monoData);

    QString getInstrumentName(int program);

    // Карта инструментов
    QMap<int, QString> instruments = {
        {1, "CrandPno"}, {2, "BritePno"}, {3, "Harpsi"}, {6, "Harpsi2"},
        {28, "FngrBass"}, {33, "FngrBass2"}, {39, "FngrBass3"}, {40, "SlapBas"},
        {29, "Ovrdrive"}, {37, "Ovrdrive2"}, {38, "DstGtr"},
        {48, "String"}, {49, "SlowStr"}, {81, "LeadSx"},
        {95, "Bright"}, {96, "Bright2"}, {100, "Bright3"}
    };

    // Флаг для отслеживания относительного времени записи
    double globalRecordingStartTime;
    bool isGlobalRecording;
};

#endif // YAMITRACKER_H
