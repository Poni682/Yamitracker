#include "audiosynthesizer.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <QtGlobal> // Добавляем для Q_UNUSED

AudioSynthesizer::AudioSynthesizer(int sampleRate) 
    : sampleRate(sampleRate), reverbLevel(0.0f), chorusLevel(0.0f) {
    reverbBuffer.resize(sampleRate * 2, 0.0f); // 2 секунды реверберации
    reverbPos = 0;
    lfoPhase = 0.0;
    lfoInc = 5.0 / sampleRate; // LFO 5 Гц
    
    // Инициализация случайных чисел для детюна
    srand(static_cast<unsigned int>(time(nullptr)));
}

AudioSynthesizer::~AudioSynthesizer() {
    voices.clear();
}

void AudioSynthesizer::addNote(int note, int velocity, int instrument) {
    Voice voice;
    voice.note = note;
    voice.phase = 0.0;
    voice.phaseInc = noteToFrequency(note) / sampleRate;
    voice.amplitude = velocity / 127.0;
    
    // Настройки огибающей в зависимости от инструмента
    switch(instrument % 4) {
        case 0: // Фортепиано
            voice.attackTime = 0.01;
            voice.decayTime = 0.1;
            voice.sustainLevel = 0.7;
            voice.releaseTime = 0.5;
            voice.waveform = 0; // Синус
            break;
        case 1: // Скрипка/Струнные
            voice.attackTime = 0.05;
            voice.decayTime = 0.2;
            voice.sustainLevel = 0.8;
            voice.releaseTime = 0.8;
            voice.waveform = 1; // Пила
            break;
        case 2: // Флейта/Духовые
            voice.attackTime = 0.1;
            voice.decayTime = 0.05;
            voice.sustainLevel = 0.9;
            voice.releaseTime = 0.3;
            voice.waveform = 0; // Синус
            break;
        case 3: // Ударные/Перкуссия
            voice.attackTime = 0.001;
            voice.decayTime = 0.05;
            voice.sustainLevel = 0.0;
            voice.releaseTime = 0.1;
            voice.waveform = 2; // Квадрат
            break;
        default:
            voice.attackTime = 0.05;
            voice.decayTime = 0.15;
            voice.sustainLevel = 0.75;
            voice.releaseTime = 0.4;
            voice.waveform = 0;
    }
    
    voice.envelope = 0.0;
    voice.isReleasing = false;
    voice.time = 0.0;
    
    // Случайный детюн для хоруса (-0.01 до 0.01)
    voice.detune = (rand() % 200 - 100) / 10000.0;
    
    // Проверяем, нет ли уже такой ноты
    auto it = std::find_if(voices.begin(), voices.end(),
        [note](const Voice& v) { return v.note == note; });
    
    if (it != voices.end()) {
        // Заменяем существующую ноту
        *it = voice;
    } else {
        // Добавляем новую ноту (ограничиваем полифонию 32 голосами)
        if (voices.size() < 32) {
            voices.push_back(voice);
        }
    }
}

void AudioSynthesizer::removeNote(int note) {
    for (auto& voice : voices) {
        if (voice.note == note && !voice.isReleasing) {
            voice.isReleasing = true;
            voice.time = 0.0;
        }
    }
}

void AudioSynthesizer::clearNotes() {
    voices.clear();
}

void AudioSynthesizer::setInstrument(int instrument) {
    // Обновляем настройки для новых нот
    // Существующие ноты продолжают играть с текущими настройками
    Q_UNUSED(instrument);
}

void AudioSynthesizer::setReverbLevel(float level) {
    reverbLevel = level < 0.0f ? 0.0f : (level > 1.0f ? 1.0f : level);
}

void AudioSynthesizer::setChorusLevel(float level) {
    chorusLevel = level < 0.0f ? 0.0f : (level > 1.0f ? 1.0f : level);
}

short AudioSynthesizer::generateSample() {
    double sample = 0.0;
    
    // Удаляем завершившиеся голоса
    voices.erase(std::remove_if(voices.begin(), voices.end(),
        [](const Voice& v) {
            return v.isReleasing && v.envelope <= 0.001;
        }), voices.end());
    
    // Генерируем звук для каждого активного голоса
    for (auto& voice : voices) {
        updateEnvelope(voice);
        
        if (voice.envelope <= 0.001) {
            continue;
        }
        
        // Основной тон с возможным детюном
        double phase = voice.phase;
        
        // Добавляем LFO модуляцию для хоруса
        if (chorusLevel > 0) {
            double lfo = sin(lfoPhase * 2 * M_PI) * chorusLevel * 0.1;
            phase += lfo + voice.detune;
        }
        
        // Генерируем волновую форму
        double voiceSample = generateWaveform(phase, voice.waveform);
        
        // Применяем огибающую и амплитуду
        voiceSample *= voice.amplitude * voice.envelope;
        
        sample += voiceSample;
        
        // Обновляем фазу для следующего сэмпла
        voice.phase += voice.phaseInc;
        if (voice.phase >= 1.0) {
            voice.phase -= 1.0;
        }
        
        voice.time += 1.0 / sampleRate;
    }
    
    // Обновляем LFO фазу
    lfoPhase += lfoInc;
    if (lfoPhase >= 1.0) {
        lfoPhase -= 1.0;
    }
    
    // Применяем реверберацию (простой алгоритм задержки с обратной связью)
    if (reverbLevel > 0.01f) {
        float wet = static_cast<float>(sample) * reverbLevel;
        float dry = static_cast<float>(sample) * (1.0f - reverbLevel * 0.5f);
        
        // Несколько задержек для более богатого звучания
        int delay1 = static_cast<int>(sampleRate * 0.03f); // 30ms
        int delay2 = static_cast<int>(sampleRate * 0.05f); // 50ms
        int delay3 = static_cast<int>(sampleRate * 0.07f); // 70ms
        
        size_t bufferSize = reverbBuffer.size();
        
        float delayed1 = reverbBuffer[(reverbPos - delay1 + bufferSize) % bufferSize];
        float delayed2 = reverbBuffer[(reverbPos - delay2 + bufferSize) % bufferSize];
        float delayed3 = reverbBuffer[(reverbPos - delay3 + bufferSize) % bufferSize];
        
        // Смешиваем задержки
        float combinedDelay = delayed1 * 0.4f + delayed2 * 0.35f + delayed3 * 0.25f;
        
        // Обратная связь с демпфированием
        wet = wet * 0.6f + combinedDelay * 0.4f;
        
        // Сохраняем в буфер реверберации
        reverbBuffer[reverbPos] = wet * 0.7f;
        reverbPos = (reverbPos + 1) % bufferSize;
        
        // Смешиваем сухой и мокрый сигналы
        sample = dry + wet * 0.5f;
    }
    
    // Ограничиваем амплитуду
    if (sample > 1.0) sample = 1.0;
    if (sample < -1.0) sample = -1.0;
    
    // Конвертируем в 16-битный формат
    return static_cast<short>(sample * 32767.0);
}

double AudioSynthesizer::noteToFrequency(int note) {
    // Стандартная формула MIDI ноты в частоту
    // A4 (нота 69) = 440 Гц
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

double AudioSynthesizer::generateWaveform(double phase, int waveform) {
    switch(waveform) {
        case 0: // Синус
            return sin(phase * 2 * M_PI);
            
        case 1: // Пила
            return 2.0 * (phase - floor(phase + 0.5));
            
        case 2: // Квадрат
            return (phase < 0.5) ? 1.0 : -1.0;
            
        case 3: // Треугольник
            return 1.0 - 4.0 * fabs(phase - 0.5);
            
        default:
            return sin(phase * 2 * M_PI);
    }
}

void AudioSynthesizer::updateEnvelope(Voice& voice) {
    double elapsed = voice.time;
    
    if (voice.isReleasing) {
        // Фаза релиза
        if (elapsed < voice.releaseTime && voice.releaseTime > 0) {
            voice.envelope = voice.sustainLevel * (1.0 - elapsed / voice.releaseTime);
        } else {
            voice.envelope = 0.0;
        }
    } else {
        // ADSR фазы
        if (elapsed < voice.attackTime) {
            // Атака
            voice.envelope = elapsed / voice.attackTime;
        } else if (elapsed < voice.attackTime + voice.decayTime) {
            // Спад
            double decayElapsed = elapsed - voice.attackTime;
            voice.envelope = 1.0 - (1.0 - voice.sustainLevel) * (decayElapsed / voice.decayTime);
        } else {
            // Сустейн
            voice.envelope = voice.sustainLevel;
        }
    }
    
    // Убеждаемся, что огибающая в допустимых пределах
    if (voice.envelope < 0.0) voice.envelope = 0.0;
    if (voice.envelope > 1.0) voice.envelope = 1.0;
}