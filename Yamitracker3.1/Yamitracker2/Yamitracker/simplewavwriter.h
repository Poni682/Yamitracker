#ifndef SIMPLEWAVWRITER_H
#define SIMPLEWAVWRITER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <cmath>

struct WavEvent {
    double time;
    int note;
    int velocity;
    int duration; // в сэмплах
    int instrument; // тип инструмента
};

// Типы волновых форм
enum WaveformType {
    WAVEFORM_SINE = 0,      // Синус (чистый тон)
    WAVEFORM_SQUARE = 1,    // Прямоугольная (клавесин, пианино)
    WAVEFORM_SAWTOOTH = 2,  // Пилообразная (струнные, бас)
    WAVEFORM_TRIANGLE = 3,  // Треугольная (флейта, орган)
    WAVEFORM_NOISE = 4      // Шум (перкуссия)
};

class SimpleWavWriter
{
public:
    SimpleWavWriter();
    ~SimpleWavWriter();

    bool startRecording(int sampleRate = 16000);
    bool stopRecording();
    bool saveToFile(const QString &filename);
    
    void addNote(double time, int note, int velocity, double duration, int instrument = 0);
    void addSilence(double duration);
    
    bool isRecording() const { return m_isRecording; }
    void clear();
    
    void setSampleRate(int rate) { m_sampleRate = rate; }
    int sampleRate() const { return m_sampleRate; }
    
    // Настройки инструментов
    void setBassVolumeMultiplier(double multiplier) { m_bassVolumeMultiplier = multiplier; }
    void setPlaybackSpeed(double speed) { m_playbackSpeed = speed; }
    
private:
    QVector<WavEvent> m_events;
    bool m_isRecording;
    double m_startTime;
    int m_sampleRate;
    double m_bassVolumeMultiplier;
    double m_playbackSpeed;
    
    void writeWavHeader(QDataStream &stream, quint32 dataSize);
    void writeWavData(QDataStream &stream);
    
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
};

#endif