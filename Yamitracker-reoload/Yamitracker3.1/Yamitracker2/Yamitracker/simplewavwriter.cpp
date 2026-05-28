#include "simplewavwriter.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>
#include <cstdlib>
#include <ctime>

SimpleWavWriter::SimpleWavWriter()
    : m_isRecording(false)
    , m_startTime(0)
    , m_sampleRate(16000)
    , m_bassVolumeMultiplier(2.0)
    , m_playbackSpeed(1.0)
{
    std::srand(std::time(nullptr));
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
    
    QByteArray audioData;
    QDataStream audioStream(&audioData, QIODevice::WriteOnly);
    audioStream.setByteOrder(QDataStream::LittleEndian);
    
    writeWavData(audioStream);
    
    writeWavHeader(stream, audioData.size());
    
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
    event.instrument = instrument;
    
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

WaveformType SimpleWavWriter::getWaveformForInstrument(int instrument)
{
    // Определяем волновую форму по типу инструмента
    if (instrument == 1) return WAVEFORM_SINE;          // CrandPno - синус
    if (instrument == 2) return WAVEFORM_SQUARE;        // BritePno - прямоугольная
    if (instrument == 3 || instrument == 6) return WAVEFORM_SQUARE; // Harpsi - прямоугольная
    if (instrument == 25 || instrument == 27) return WAVEFORM_TRIANGLE; // Гитары - треугольная
    if (instrument >= 28 && instrument <= 40) return WAVEFORM_SAWTOOTH; // Басы - пилообразная
    if (instrument == 48 || instrument == 49) return WAVEFORM_SINE;     // Струнные - синус
    if (instrument == 81) return WAVEFORM_TRIANGLE;     // Саксофон - треугольная
    if (instrument == 89) return WAVEFORM_SINE;         // SoftPad - синус
    if (instrument >= 95 && instrument <= 100) return WAVEFORM_SQUARE; // Bright - прямоугольная
    
    return WAVEFORM_SINE; // По умолчанию
}

double SimpleWavWriter::getFilterCutoffForInstrument(int instrument)
{
    // Частота среза фильтра
    if (instrument >= 28 && instrument <= 40) return 400.0; // Бас - низкие частоты
    if (instrument == 1) return 1000.0; // Пианино
    if (instrument == 2) return 2000.0; // Яркое пианино
    if (instrument == 3 || instrument == 6) return 1500.0; // Клавесин
    if (instrument == 25 || instrument == 27) return 800.0; // Гитары
    if (instrument == 81) return 600.0;  // Саксофон
    return 1000.0; // По умолчанию
}

double SimpleWavWriter::getAttackForInstrument(int instrument)
{
    // Время атаки
    if (instrument >= 28 && instrument <= 40) return 0.1;  // Бас - медленная атака
    if (instrument == 1) return 0.05;  // Пианино
    if (instrument == 2) return 0.02;  // Яркое пианино - быстрая атака
    if (instrument == 3 || instrument == 6) return 0.01; // Клавесин - очень быстрая
    if (instrument == 81) return 0.08;  // Саксофон
    return 0.05;
}

double SimpleWavWriter::getDecayForInstrument(int instrument)
{
    // Время спада
    if (instrument >= 28 && instrument <= 40) return 0.2;  // Бас
    if (instrument == 1) return 0.1;   // Пианино
    if (instrument == 2) return 0.05;  // Яркое пианино
    if (instrument == 3 || instrument == 6) return 0.02; // Клавесин
    return 0.1;
}

double SimpleWavWriter::getSustainForInstrument(int instrument)
{
    // Уровень сустейна
    if (instrument >= 28 && instrument <= 40) return 0.8;  // Бас - высокий сустейн
    if (instrument == 1) return 0.7;   // Пианино
    if (instrument == 2) return 0.6;   // Яркое пианино
    if (instrument == 3 || instrument == 6) return 0.5; // Клавесин - быстрый спад
    if (instrument == 81) return 0.9;  // Саксофон - длинный сустейн
    return 0.7;
}

double SimpleWavWriter::getReleaseForInstrument(int instrument)
{
    // Время релиза
    if (instrument >= 28 && instrument <= 40) return 0.3;  // Бас - длинный релиз
    if (instrument == 1) return 0.2;   // Пианино
    if (instrument == 2) return 0.1;   // Яркое пианино
    if (instrument == 3 || instrument == 6) return 0.05; // Клавесин - короткий
    if (instrument == 81) return 0.4;  // Саксофон - длинный
    return 0.2;
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
        for (int i = 0; i < m_sampleRate * 2; i++) {
            short sample = 0;
            stream << sample;
        }
        return;
    }
    
    // Применяем скорость воспроизведения
    double timeScale = 1.0 / m_playbackSpeed;
    
    for (const WavEvent &event : m_events) {
        if (event.time == -1) {
            // Тишина
            for (int i = 0; i < event.duration; i++) {
                short sample = 0;
                stream << sample;
            }
        } else {
            // Нота
            double frequency = noteToFrequency(event.note);
            
            // Определяем усиление для басовых нот
            double volumeMultiplier = 1.0;
            if (event.instrument >= 28 && event.instrument <= 40) {
                volumeMultiplier = m_bassVolumeMultiplier;
            }
            
            double baseAmplitude = (event.velocity / 127.0) * 0.7 * volumeMultiplier;
            
            // Параметры огибающей для этого инструмента
            double attack = getAttackForInstrument(event.instrument);
            double decay = getDecayForInstrument(event.instrument);
            double sustain = getSustainForInstrument(event.instrument);
            double release = getReleaseForInstrument(event.instrument);
            
            // Генерируем ноту
            for (int i = 0; i < event.duration; i++) {
                double noteTime = (double)i / m_sampleRate * timeScale;
                double noteDuration = (double)event.duration / m_sampleRate * timeScale;
                
                // Огибающая ADSR
                double envelope = adsrEnvelope(noteTime, noteDuration, attack, decay, sustain, release);
                
                // Амплитуда с учетом огибающей
                double amplitude = baseAmplitude * envelope;
                
                // Ограничиваем амплитуду чтобы не было клиппинга
                if (amplitude > 1.0) amplitude = 1.0;
                
                // Генерируем волну
                double t = event.time + noteTime;
                short sample = generateWaveform(t, frequency, amplitude, event.instrument);
                
                stream << sample;
            }
        }
    }
    
    // Добавляем немного тишины в конце
    for (int i = 0; i < m_sampleRate * 0.5; i++) {
        short sample = 0;
        stream << sample;
    }
}

double SimpleWavWriter::adsrEnvelope(double noteTime, double noteDuration, 
                                   double attack, double decay, 
                                   double sustain, double release)
{
    double envelope = 1.0;
    
    if (noteTime < attack) {
        // Атака
        envelope = noteTime / attack;
    } else if (noteTime < attack + decay) {
        // Спад
        envelope = 1.0 - ((noteTime - attack) / decay) * (1.0 - sustain);
    } else if (noteTime < noteDuration - release) {
        // Сусгейн
        envelope = sustain;
    } else if (noteTime < noteDuration) {
        // Релиз
        double releaseTime = noteTime - (noteDuration - release);
        envelope = sustain * (1.0 - releaseTime / release);
    } else {
        // После релиза
        envelope = 0.0;
    }
    
    return envelope;
}

double SimpleWavWriter::sineWave(double phase)
{
    return sin(2.0 * M_PI * phase);
}

double SimpleWavWriter::squareWave(double phase)
{
    return sin(2.0 * M_PI * phase) > 0 ? 0.7 : -0.7;
}

double SimpleWavWriter::sawtoothWave(double phase)
{
    return 2.0 * (phase - floor(phase + 0.5));
}

double SimpleWavWriter::triangleWave(double phase)
{
    double x = 2.0 * (phase - floor(phase + 0.5));
    return 2.0 * fabs(x) - 1.0;
}

double SimpleWavWriter::noiseWave()
{
    return (2.0 * rand() / RAND_MAX) - 1.0;
}

double SimpleWavWriter::lowPassFilter(double input, double &prev, double cutoff)
{
    double RC = 1.0 / (2.0 * M_PI * cutoff);
    double dt = 1.0 / m_sampleRate;
    double alpha = dt / (RC + dt);
    double output = prev + alpha * (input - prev);
    prev = output;
    return output;
}

double SimpleWavWriter::highPassFilter(double input, double &prevInput, double &prevOutput, double cutoff)
{
    double RC = 1.0 / (2.0 * M_PI * cutoff);
    double dt = 1.0 / m_sampleRate;
    double alpha = RC / (RC + dt);
    double output = alpha * (prevOutput + input - prevInput);
    prevInput = input;
    prevOutput = output;
    return output;
}

double SimpleWavWriter::noteToFrequency(int note)
{
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

short SimpleWavWriter::generateWaveform(double time, double frequency, double amplitude, int instrument)
{
    double phase = fmod(time * frequency, 1.0);
    double value = 0.0;
    
    WaveformType waveform = getWaveformForInstrument(instrument);
    
    switch (waveform) {
    case WAVEFORM_SINE:
        value = sineWave(phase);
        break;
    case WAVEFORM_SQUARE:
        value = squareWave(phase);
        // Добавляем немного гармоник для прямоугольной волны
        for (int i = 3; i <= 9; i += 2) {
            value += 0.7 / i * sin(2.0 * M_PI * frequency * i * time);
        }
        break;
    case WAVEFORM_SAWTOOTH:
        value = sawtoothWave(phase);
        // Добавляем гармоники для пилообразной волны
        for (int i = 2; i <= 10; i++) {
            value += 0.5 / i * sin(2.0 * M_PI * frequency * i * time);
        }
        break;
    case WAVEFORM_TRIANGLE:
        value = triangleWave(phase);
        // Добавляем нечетные гармоники для треугольной волны
        for (int i = 3; i <= 9; i += 2) {
            value += 0.8 / (i * i) * sin(2.0 * M_PI * frequency * i * time);
        }
        break;
    case WAVEFORM_NOISE:
        value = noiseWave();
        break;
    }
    
    // Применяем фильтр в зависимости от инструмента
    static double prevLP = 0;
    static double prevHPInput = 0;
    static double prevHPOutput = 0;
    
    double cutoff = getFilterCutoffForInstrument(instrument);
    
    if (instrument >= 28 && instrument <= 40) {
        // Бас - сильный low-pass фильтр
        value = lowPassFilter(value, prevLP, cutoff);
    } else if (instrument == 3 || instrument == 6) {
        // Клавесин - немного high-pass для яркости
        value = highPassFilter(value, prevHPInput, prevHPOutput, 500.0);
    }
    
    // Ограничиваем значение чтобы избежать клиппинга
    if (value > 1.0) value = 1.0;
    if (value < -1.0) value = -1.0;
    
    return static_cast<short>(value * amplitude * 32767.0);
}