// simplewavwriter.h
#ifndef SIMPLEWAVWRITER_H
#define SIMPLEWAVWRITER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QAudioFormat>
#include <QByteArray>
#include <QFile>

struct NoteEvent {
    double time;        // Время начала в секундах
    int note;           // Номер ноты (0-127)
    int velocity;       // Сила нажатия (0-127)
    double duration;    // Длительность в секундах
    int instrument;     // Номер инструмента
    double volume;      // Громкость ноты (0.0-1.0)
    
    NoteEvent(double t = 0, int n = 0, int v = 100, double d = 0, int i = 1, double vol = 1.0)
        : time(t), note(n), velocity(v), duration(d), instrument(i), volume(vol) {}
};

class SimpleWavWriter : public QObject
{
    Q_OBJECT

public:
    static const int MAX_TRACKS = 7;  // ИЗМЕНЕНО: было 16, стало 7
    static const int SAMPLE_RATE = 44100; // Улучшенное качество
    static const int BIT_DEPTH = 16;
    
    explicit SimpleWavWriter(QObject *parent = nullptr);
    
    bool startRecording(int sampleRate = SAMPLE_RATE);
    void stopRecording();
    bool isRecording() const { return recording; }
    bool isPaused() const { return paused; }
    void pauseRecording();
    void resumeRecording();
    
    void addNote(int track, double time, int note, int velocity, double duration, int instrument = 1);
    void clearTrack(int track);
    void clearAllTracks();
    
    bool saveToFile(const QString &filename);
    bool saveToMusicFile(const QString &filename);
    
    // Работа с дорожками
    QString getTrackName(int track) const;
    void setTrackName(int track, const QString &name);
    int getEventCount(int track) const;
    int getTrackInstrument(int track) const;
    void setTrackInstrument(int track, int instrument);
    double getTrackVolume(int track) const;
    void setTrackVolume(int track, double volume);
    bool isTrackMuted(int track) const;
    void muteTrack(int track, bool muted);
    bool isTrackSolo(int track) const;
    void soloTrack(int track, bool solo);
    
    // Работа с аудиоданными
    QByteArray getTrackAudioData(int track, bool mix = false);
    QByteArray getAllTracksAudioData();
    
    // Загрузка/сохранение дорожек
    bool loadWavToTrack(const QString &filename, int track);
    void copyTrack(int fromTrack, int toTrack);
    void mergeTrack(int fromTrack, int toTrack);
    
    // Эффекты и настройки
    void setPlaybackSpeed(double speed);
    void setBassVolumeMultiplier(double multiplier);
    
    // Генерация звука
    QByteArray generateNoteAudio(int note, int velocity, double duration, int instrument, double volume = 1.0);
    QByteArray generateSilence(double duration);
    
signals:
    void trackUpdated(int track);
    void recordingStatusChanged(bool recording);

private:
    struct Track {
        QString name;
        QVector<NoteEvent> events;
        int instrument;
        double volume;
        bool muted;
        bool solo;
        int index; // Добавляем индекс дорожки
        
        Track() : instrument(1), volume(1.0), muted(false), solo(false), index(0) {
        }
        
        // Конструктор с индексом
        Track(int idx) : instrument(1), volume(1.0), muted(false), solo(false), index(idx) {
            name = QString("Дорожка %1").arg(idx + 1);
        }
    };
    
    bool recording;
    bool paused;
    QVector<Track> tracks;
    QAudioFormat format;
    double playbackSpeed;
    double bassVolumeMultiplier;
    
    void writeWavHeader(QFile &file, quint32 dataSize);
    QByteArray mixTracks(const QVector<int> &trackIndices);
    double noteToFrequency(int note) const;
    QByteArray generateSineWave(double frequency, double duration, double amplitude);
    QByteArray generateSquareWave(double frequency, double duration, double amplitude);
    QByteArray generateSawtoothWave(double frequency, double duration, double amplitude);
    QByteArray generateTriangleWave(double frequency, double duration, double amplitude);
    QByteArray generateInstrumentWave(int instrument, double frequency, double duration, double amplitude);
    
    // Инструменты для форматирования данных
    QByteArray intToBytes(qint32 value);
    QByteArray shortToBytes(qint16 value);
    
    // Форматирование для .s файла
    QString noteToPentlyNote(int note) const;
    QString durationToPentlyDuration(double duration) const;
    QString instrumentToPentlyInstrument(int instrument) const;
    
    // НОВЫЕ МЕТОДЫ для конвертации в Pently формат
    int secondsToPentlyTicks(double seconds) const;
    QString ticksToDelayString(int ticks) const;
    QString generatePattern(int trackIndex) const;
};

#endif // SIMPLEWAVWRITER_H