#ifndef SIMPLEWAVWRITER_H
#define SIMPLEWAVWRITER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QMap>
#include <QAudioOutput>
#include <QBuffer>
#include <cmath>
#include <algorithm>

struct WavEvent {
    int track;
    double time;
    int note;
    int velocity;
    int duration; // в сэмплах
    int instrument; // тип инструмента
    bool isPause;   // флаг паузы
};

// Типы волновых форм
enum WaveformType {
    WAVEFORM_SINE = 0,      // Синус (чистый тон)
    WAVEFORM_SQUARE = 1,    // Прямоугольная (клавесин, пианино)
    WAVEFORM_SAWTOOTH = 2,  // Пилообразная (струнные, бас)
    WAVEFORM_TRIANGLE = 3,  // Треугольная (флейта, орган)
    WAVEFORM_NOISE = 4      // Шум (перкуссия)
};

struct TrackInfo {
    QString name;
    int instrument;
    double volume;
    bool muted;
    QVector<WavEvent> events;
    QByteArray audioData; // Кэшированные аудиоданные для быстрого воспроизведения
};

class SimpleWavWriter : public QObject
{
    Q_OBJECT

public:
    static const int MAX_TRACKS = 7; // 7 дорожек (для 7 каналов Pently)
    static const int MAX_OCTAVES = 5; // Октавы: от C2 до C6
    
    SimpleWavWriter(QObject *parent = nullptr);
    ~SimpleWavWriter();

    bool startRecording(int sampleRate = 16000);
    bool pauseRecording();
    bool resumeRecording();
    bool stopRecording();
    bool saveToFile(const QString &filename);
    bool saveToMusicFile(const QString &filename); // Сохранить как музыкальный файл
    
    // Методы для работы с дорожками
    void addNote(int track, double time, int note, int velocity, double duration, int instrument = 0);
    void addSilence(int track, double duration);
    void addPause(int track, double duration);
    void clearTrack(int track);
    void clearAllTracks();
    void mergeTrack(int fromTrack, int toTrack);
    void copyTrack(int fromTrack, int toTrack);
    void setTrackName(int track, const QString &name);
    void setTrackInstrument(int track, int instrument);
    void muteTrack(int track, bool muted);
    void setTrackVolume(int track, double volume);
    
    // Воспроизведение
    QByteArray getTrackAudioData(int track, bool preview = false);
    QByteArray getAllTracksAudioData();
    QByteArray getMergedAudioData();
    
    // Загрузка из WAV
    bool loadWavToTrack(const QString &filename, int track);
    
    // Методы для получения информации
    int getEventCount(int track) const;
    QString getTrackName(int track) const;
    int getTrackInstrument(int track) const;
    double getTrackVolume(int track) const;
    bool isTrackMuted(int track) const;
    QVector<WavEvent> getTrackEvents(int track) const;
    double getTotalDuration() const;
    double getTrackDuration(int track) const;
    
    // Устаревшие методы (для обратной совместимости)
    void addNote(double time, int note, int velocity, double duration, int instrument = 0) {
        addNote(0, time, note, velocity, duration, instrument);
    }
    void addSilence(double duration) {
        addSilence(0, duration);
    }
    
    bool isRecording() const { return m_isRecording; }
    bool isPaused() const { return m_isPaused; }
    void clear();
    
    void setSampleRate(int rate) { m_sampleRate = rate; }
    int sampleRate() const { return m_sampleRate; }
    
    // Настройки инструментов
    void setBassVolumeMultiplier(double multiplier) { m_bassVolumeMultiplier = multiplier; }
    void setPlaybackSpeed(double speed) { m_playbackSpeed = speed; }
    void setMasterVolume(double volume) { m_masterVolume = volume; }
    
    // Синтез для предпрослушивания
    QByteArray synthesizeTrackPreview(int track);
    
signals:
    void trackUpdated(int track);
    void audioDataReady(int track, const QByteArray &data);
    
private:
    QVector<TrackInfo> m_tracks; // Дорожки для записи
    bool m_isRecording;
    bool m_isPaused;
    double m_startTime;
    double m_pauseTime;
    int m_sampleRate;
    double m_bassVolumeMultiplier;
    double m_playbackSpeed;
    double m_masterVolume;
    
    void writeWavHeader(QDataStream &stream, quint32 dataSize);
    QByteArray generateTrackAudio(int track, bool includeEffects = true);
    void generateAllTracksAudio();
    
    // Генерация звука с разными волновыми формами
    short generateWaveform(double time, double frequency, double amplitude, int instrument);
    
    // Разные волновые формы
    double sineWave(double phase);
    double squareWave(double phase);
    double sawtoothWave(double phase);
    double triangleWave(double phase);
    double noiseWave();
    
    // Фильтры
    double lowPassFilter(double input, double &prev, double cutoff);
    double highPassFilter(double input, double &prevInput, double &prevOutput, double cutoff);
    
    double noteToFrequency(int note);
    int frequencyToNote(double frequency);
    
    // Вспомогательные методы для сохранения музыки
    QString noteNumberToName(int note);
    QString getDurationSymbol(int durationSamples);
    QString getInstrumentMacroName(int instrument);
    
    // Получение параметров инструмента
    WaveformType getWaveformForInstrument(int instrument);
    double getFilterCutoffForInstrument(int instrument);
    double getAttackForInstrument(int instrument);
    double getDecayForInstrument(int instrument);
    double getSustainForInstrument(int instrument);
    double getReleaseForInstrument(int instrument);
    
    // Генераторы огибающей
    double adsrEnvelope(double noteTime, double noteDuration, 
                        double attack, double decay, 
                        double sustain, double release);
    
    // Методы для формата Pently
    QString generatePentlyPattern(int track, int patternIndex);
    QString generatePentlySong();
    QVector<QVector<WavEvent>> groupEventsByTime(int track, double timeThreshold = 0.25);
    QString getNoteCommand(const WavEvent &event, bool includeInstrument = false);
    
    // Обработка WAV файлов
    bool decodeWavData(const QByteArray &wavData, QVector<int16_t> &samples, int &sampleRate);
    bool loadWavFile(const QString &filename, QVector<int16_t> &samples, int &sampleRate);
};

#endif