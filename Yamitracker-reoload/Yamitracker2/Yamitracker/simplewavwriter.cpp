#include "simplewavwriter.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>

SimpleWavWriter::SimpleWavWriter()
    : m_isRecording(false)
    , m_startTime(0)
    , m_sampleRate(16000)
{
}

SimpleWavWriter::~SimpleWavWriter()
{
}

bool SimpleWavWriter::startRecording(int sampleRate)
{
    if (m_isRecording) {
        stopRecording();
    }
    
    m_events.clear();
    m_isRecording = true;
    m_startTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    m_sampleRate = sampleRate;
    
    qDebug() << "Started WAV recording, sample rate:" << m_sampleRate;
    return true;
}

bool SimpleWavWriter::stopRecording()
{
    if (!m_isRecording) {
        return false;
    }
    
    m_isRecording = false;
    qDebug() << "Stopped WAV recording, events count:" << m_events.size();
    return true;
}

bool SimpleWavWriter::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }
    
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    
    // Сначала записываем данные в буфер
    QByteArray audioData;
    QDataStream audioStream(&audioData, QIODevice::WriteOnly);
    audioStream.setByteOrder(QDataStream::LittleEndian);
    
    writeWavData(audioStream);
    
    // Теперь записываем заголовок с правильным размером
    writeWavHeader(stream, audioData.size());
    
    // Записываем аудиоданные
    stream.writeRawData(audioData.constData(), audioData.size());
    
    file.close();
    qDebug() << "WAV file saved:" << filename << "size:" << audioData.size() << "bytes";
    return true;
}

void SimpleWavWriter::addNote(double time, int note, int velocity, double duration)
{
    if (!m_isRecording) return;
    
    WavEvent event;
    event.time = time;
    event.note = note;
    event.velocity = velocity;
    event.duration = duration * m_sampleRate;
    
    m_events.append(event);
}

void SimpleWavWriter::addSilence(double duration)
{
    if (!m_isRecording) return;
    
    WavEvent event;
    event.time = -1; // Флаг тишины
    event.note = 0;
    event.velocity = 0;
    event.duration = duration * m_sampleRate;
    
    m_events.append(event);
}

void SimpleWavWriter::clear()
{
    m_events.clear();
    m_isRecording = false;
}

void SimpleWavWriter::writeWavHeader(QDataStream &stream, quint32 dataSize)
{
    // RIFF заголовок
    stream.writeRawData("RIFF", 4);
    quint32 chunkSize = 36 + dataSize;
    stream << chunkSize;
    stream.writeRawData("WAVE", 4);
    
    // fmt chunk
    stream.writeRawData("fmt ", 4);
    quint32 subchunk1Size = 16;
    stream << subchunk1Size;
    
    quint16 audioFormat = 1; // PCM
    stream << audioFormat;
    
    quint16 numChannels = 1; // Моно
    stream << numChannels;
    
    quint32 sampleRate = m_sampleRate;
    stream << sampleRate;
    
    quint32 byteRate = sampleRate * numChannels * 2; // 16-bit = 2 байта
    stream << byteRate;
    
    quint16 blockAlign = numChannels * 2;
    stream << blockAlign;
    
    quint16 bitsPerSample = 16;
    stream << bitsPerSample;
    
    // data chunk
    stream.writeRawData("data", 4);
    stream << dataSize;
}

void SimpleWavWriter::writeWavData(QDataStream &stream)
{
    if (m_events.isEmpty()) {
        // Если нет событий, создаем короткий тихий фрагмент
        for (int i = 0; i < m_sampleRate * 2; i++) { // 2 секунды тишины
            short sample = 0;
            stream << sample;
        }
        return;
    }
    
    double totalDuration = 0;
    for (const WavEvent &event : m_events) {
        if (event.time == -1) {
            // Тишина
            for (int i = 0; i < event.duration; i++) {
                short sample = 0;
                stream << sample;
            }
            totalDuration += (double)event.duration / m_sampleRate;
        } else {
            // Нота
            double frequency = noteToFrequency(event.note);
            double amplitude = (event.velocity / 127.0) * 0.7; // Нормализуем громкость
            
            for (int i = 0; i < event.duration; i++) {
                double t = totalDuration + (double)i / m_sampleRate;
                short sample = generateSineWave(t, frequency, amplitude);
                
                // ADSR envelope
                double attackTime = 0.05;
                double decayTime = 0.1;
                double sustainLevel = 0.8;
                double releaseTime = 0.2;
                
                double noteTime = (double)i / m_sampleRate;
                double noteDuration = (double)event.duration / m_sampleRate;
                double envelope = 1.0;
                
                if (noteTime < attackTime) {
                    envelope = noteTime / attackTime;
                } else if (noteTime < attackTime + decayTime) {
                    envelope = 1.0 - ((noteTime - attackTime) / decayTime) * (1.0 - sustainLevel);
                } else if (noteTime < noteDuration - releaseTime) {
                    envelope = sustainLevel;
                } else {
                    envelope = sustainLevel * (1.0 - (noteTime - (noteDuration - releaseTime)) / releaseTime);
                }
                
                sample = sample * envelope;
                stream << sample;
            }
            totalDuration += (double)event.duration / m_sampleRate;
        }
    }
    
    // Добавляем немного тишины в конце
    for (int i = 0; i < m_sampleRate * 0.5; i++) { // 0.5 секунды тишины
        short sample = 0;
        stream << sample;
    }
}

double SimpleWavWriter::noteToFrequency(int note)
{
    // A4 = 440Hz, номер MIDI ноты 69
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

short SimpleWavWriter::generateSineWave(double time, double frequency, double amplitude)
{
    double value = sin(2.0 * M_PI * frequency * time);
    return static_cast<short>(value * amplitude * 32767.0);
}