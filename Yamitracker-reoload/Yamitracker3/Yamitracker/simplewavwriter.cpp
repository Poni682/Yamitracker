#include "simplewavwriter.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>
#include <algorithm>

SimpleWavWriter::SimpleWavWriter()
    : m_isRecording(false)
    , m_startTime(0)
    , m_sampleRate(44100) // Повысили частоту дискретизации
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

void SimpleWavWriter::addNote(double time, int note, int velocity, double duration, int instrument)
{
    if (!m_isRecording) return;
    
    WavEvent event;
    event.time = time;
    event.note = note;
    event.velocity = velocity;
    event.duration = duration * m_sampleRate;
    event.instrument = instrument; // Сохраняем инструмент
    
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
    event.instrument = 0;
    
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
    
    // Сортируем события по времени
    std::sort(m_events.begin(), m_events.end(),
        [](const WavEvent& a, const WavEvent& b) {
            return a.time < b.time;
        });
    
    double currentTime = 0;
    int totalSamples = 0;
    
    // Оцениваем общее количество сэмплов
    for (const WavEvent &event : m_events) {
        totalSamples += event.duration;
    }
    
    // Создаем вектор для хранения сэмплов
    QVector<float> samples(totalSamples + m_sampleRate, 0.0f); // +1 секунда на всякий случай
    
    // Рендерим события
    for (const WavEvent &event : m_events) {
        if (event.time == -1) {
            // Тишина - просто продвигаем время
            currentTime += (double)event.duration / m_sampleRate;
            continue;
        }
        
        // Генерируем звук для ноты
        double frequency = noteToFrequency(event.note);
        double amplitude = (event.velocity / 127.0) * 0.8;
        
        // Начинаем с нужного времени (преобразуем время в сэмплы)
        int startSample = static_cast<int>(event.time * m_sampleRate);
        
        for (int i = 0; i < event.duration; ++i) {
            if (startSample + i >= samples.size()) {
                samples.resize(startSample + i + m_sampleRate);
            }
            
            double t = currentTime + (double)i / m_sampleRate;
            
            // Генерируем волну в зависимости от инструмента
            double sampleValue = getWaveformForInstrument(event.instrument, t, frequency, amplitude);
            
            // ADSR envelope
            double noteRelativeTime = (double)i / m_sampleRate;
            double noteDuration = (double)event.duration / m_sampleRate;
            double envelope = 1.0;
            
            // Параметры огибающей в зависимости от инструмента
            double attackTime, decayTime, sustainLevel, releaseTime;
            
            switch(event.instrument % 4) {
                case 0: // Фортепиано
                    attackTime = 0.01;
                    decayTime = 0.1;
                    sustainLevel = 0.7;
                    releaseTime = 0.5;
                    break;
                case 1: // Струнные
                    attackTime = 0.05;
                    decayTime = 0.2;
                    sustainLevel = 0.8;
                    releaseTime = 0.8;
                    break;
                case 2: // Духовые
                    attackTime = 0.1;
                    decayTime = 0.05;
                    sustainLevel = 0.9;
                    releaseTime = 0.3;
                    break;
                case 3: // Ударные
                    attackTime = 0.001;
                    decayTime = 0.05;
                    sustainLevel = 0.0;
                    releaseTime = 0.1;
                    break;
                default:
                    attackTime = 0.05;
                    decayTime = 0.15;
                    sustainLevel = 0.75;
                    releaseTime = 0.4;
            }
            
            // Применяем огибающую
            if (noteRelativeTime < attackTime) {
                envelope = noteRelativeTime / attackTime;
            } else if (noteRelativeTime < attackTime + decayTime) {
                envelope = 1.0 - ((noteRelativeTime - attackTime) / decayTime) * (1.0 - sustainLevel);
            } else if (noteRelativeTime < noteDuration - releaseTime) {
                envelope = sustainLevel;
            } else {
                envelope = sustainLevel * (1.0 - (noteRelativeTime - (noteDuration - releaseTime)) / releaseTime);
            }
            
            sampleValue *= amplitude * envelope;
            
            // Добавляем к существующему сэмплу (полифония)
            samples[startSample + i] += static_cast<float>(sampleValue);
        }
        
        // Обновляем текущее время для следующего события
        currentTime = event.time + (double)event.duration / m_sampleRate;
    }
    
    // Находим максимальную амплитуду для нормализации
    float maxAmplitude = 0.0f;
    for (float sample : samples) {
        if (fabs(sample) > maxAmplitude) {
            maxAmplitude = fabs(sample);
        }
    }
    
    // Нормализуем и записываем сэмплы
    if (maxAmplitude > 0.0f) {
        float normalizeFactor = 0.9f / maxAmplitude;
        
        for (float sample : samples) {
            short finalSample = static_cast<short>(sample * normalizeFactor * 32767.0f);
            stream << finalSample;
        }
    } else {
        // Если нет звука, записываем тишину
        for (int i = 0; i < m_sampleRate * 2; i++) {
            short sample = 0;
            stream << sample;
        }
    }
}

double SimpleWavWriter::noteToFrequency(int note)
{
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

double SimpleWavWriter::getWaveformForInstrument(int instrument, double time, double frequency, double amplitude)
{
    int waveformType = instrument % 4;
    
    switch(waveformType) {
        case 0: // Синус (фортепиано, духовые)
            return sin(2.0 * M_PI * frequency * time);
        case 1: // Пила (скрипка, струнные)
            return 2.0 * (fmod(frequency * time, 1.0) - 0.5);
        case 2: // Квадрат (орган, клавесин)
            return sin(2.0 * M_PI * frequency * time) > 0 ? 1.0 : -1.0;
        case 3: // Смешанный (ударные, перкуссия)
            return sin(2.0 * M_PI * frequency * time) * 0.7 + 
                   (sin(2.0 * M_PI * frequency * 2 * time) > 0 ? 0.3 : -0.3);
        default:
            return sin(2.0 * M_PI * frequency * time);
    }
}

short SimpleWavWriter::generateSineWave(double time, double frequency, double amplitude)
{
    double value = sin(2.0 * M_PI * frequency * time);
    return static_cast<short>(value * amplitude * 32767.0);
}

short SimpleWavWriter::generateSquareWave(double time, double frequency, double amplitude)
{
    double value = sin(2.0 * M_PI * frequency * time) > 0 ? 1.0 : -1.0;
    return static_cast<short>(value * amplitude * 32767.0);
}

short SimpleWavWriter::generateSawWave(double time, double frequency, double amplitude)
{
    double phase = fmod(frequency * time, 1.0);
    double value = 2.0 * phase - 1.0;
    return static_cast<short>(value * amplitude * 32767.0);
}