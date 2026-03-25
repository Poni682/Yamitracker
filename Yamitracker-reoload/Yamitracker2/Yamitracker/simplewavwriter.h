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
};

class SimpleWavWriter
{
public:
    SimpleWavWriter();
    ~SimpleWavWriter();

    bool startRecording(int sampleRate = 16000);
    bool stopRecording();
    bool saveToFile(const QString &filename);
    
    void addNote(double time, int note, int velocity, double duration);
    void addSilence(double duration);
    
    bool isRecording() const { return m_isRecording; }
    void clear();
    
    void setSampleRate(int rate) { m_sampleRate = rate; }
    int sampleRate() const { return m_sampleRate; }
    
private:
    QVector<WavEvent> m_events;
    bool m_isRecording;
    double m_startTime;
    int m_sampleRate;
    
    void writeWavHeader(QDataStream &stream, quint32 dataSize);
    void writeWavData(QDataStream &stream);
    
    // Генерация звука
    short generateSineWave(double time, double frequency, double amplitude);
    
    double noteToFrequency(int note);
};

#endif