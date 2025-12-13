#include "yamiplay.h"
#include <iostream>
#include <fstream>
#include <cmath>

namespace yami {

bool AudioSample::loadWAV(const std::string& filename) {
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    
    if (!SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength)) {
        std::cerr << "Failed to load WAV: " << filename << " - " << SDL_GetError() << std::endl;
        return false;
    }
    
    sampleRate = wavSpec.freq;
    
    // Конвертируем в 16-битный моно
    if (wavSpec.format == AUDIO_S16SYS && wavSpec.channels == 1) {
        samples.resize(wavLength / 2);
        memcpy(samples.data(), wavBuffer, wavLength);
    } else {
        // Простая конвертация (для реального использования нужна более сложная)
        samples.resize(wavLength / (wavSpec.channels * 2));
        // Упрощенная конвертация - берем только первый канал
        for (size_t i = 0; i < samples.size(); ++i) {
            samples[i] = reinterpret_cast<int16_t*>(wavBuffer)[i * wavSpec.channels];
        }
    }
    
    length = samples.size();
    SDL_FreeWAV(wavBuffer);
    return true;
}

AudioPlayer::AudioPlayer() : audioDevice(0), position(0), volume(1.0f) {}

AudioPlayer::~AudioPlayer() {
    if (audioDevice) {
        SDL_CloseAudioDevice(audioDevice);
    }
}

bool AudioPlayer::initialize() {
    SDL_AudioSpec desired, obtained;
    
    SDL_zero(desired);
    desired.freq = 44100;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = 4096;
    desired.callback = audioCallback;
    desired.userdata = this;
    
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
    if (!audioDevice) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_PauseAudioDevice(audioDevice, 0);
    return true;
}

bool AudioPlayer::loadNoteSample(uint8_t note, const std::string& filename) {
    AudioSample sample;
    if (sample.loadWAV(filename)) {
        noteSamples[note] = sample;
        return true;
    }
    return false;
}

void AudioPlayer::playNote(uint8_t note, uint8_t velocity) {
    std::lock_guard<std::mutex> lock(audioMutex);
    auto it = noteSamples.find(note);
    if (it != noteSamples.end()) {
        activeSamples.emplace_back(0, &it->second);
    }
}

void AudioPlayer::stopNote(uint8_t note) {
    // Для семплов просто даем доиграть
}

void AudioPlayer::stopAll() {
    std::lock_guard<std::mutex> lock(audioMutex);
    activeSamples.clear();
}

void AudioPlayer::audioCallback(void* userdata, Uint8* stream, int len) {
    AudioPlayer* player = static_cast<AudioPlayer*>(userdata);
    player->mixAudio(reinterpret_cast<int16_t*>(stream), len / 2);
}

void AudioPlayer::mixAudio(int16_t* stream, uint32_t length) {
    std::lock_guard<std::mutex> lock(audioMutex);
    
    // Очищаем буфер
    memset(stream, 0, length * sizeof(int16_t));
    
    // Микшируем активные семплы
    for (auto it = activeSamples.begin(); it != activeSamples.end();) {
        uint32_t& pos = it->first;
        const AudioSample* sample = it->second;
        
        uint32_t samplesToMix = std::min(length, sample->length - pos);
        
        for (uint32_t i = 0; i < samplesToMix; ++i) {
            int32_t mixed = stream[i] + static_cast<int32_t>(sample->samples[pos + i] * volume);
            stream[i] = static_cast<int16_t>(std::max(-32768, std::min(32767, mixed)));
        }
        
        pos += samplesToMix;
        
        if (pos >= sample->length) {
            it = activeSamples.erase(it);
        } else {
            ++it;
        }
    }
}

// Реализация 16-битного синтезатора
Synthesizer16Bit::Synthesizer16Bit(uint32_t rate) 
    : sampleRate(rate), masterVolume(0.3f) {
    voices.resize(16); // 16 голосов полифонии
}

void Synthesizer16Bit::noteOn(uint8_t note, uint8_t velocity) {
    for (auto& voice : voices) {
        if (!voice.active) {
            voice.note = note;
            voice.phase = 0.0f;
            float frequency = 440.0f * pow(2.0f, (note - 69) / 12.0f);
            voice.phaseIncrement = (2.0f * M_PI * frequency) / sampleRate;
            voice.amplitude = (velocity / 127.0f) * masterVolume;
            voice.releaseTime = 0.0f;
            voice.active = true;
            break;
        }
    }
}

void Synthesizer16Bit::noteOff(uint8_t note) {
    for (auto& voice : voices) {
        if (voice.active && voice.note == note) {
            voice.releaseTime = 0.1f; // 100ms release
        }
    }
}

void Synthesizer16Bit::generateSamples(int16_t* buffer, uint32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        float sample = 0.0f;
        
        for (auto& voice : voices) {
            if (voice.active) {
                sample += generateWaveform(voice.phase, voice.note) * voice.amplitude;
                voice.phase += voice.phaseIncrement;
                
                if (voice.phase >= 2.0f * M_PI) {
                    voice.phase -= 2.0f * M_PI;
                }
                
                // ADSR envelope
                if (voice.releaseTime > 0.0f) {
                    voice.amplitude -= voice.amplitude * 0.01f;
                    if (voice.amplitude < 0.001f) {
                        voice.active = false;
                    }
                }
            }
        }
        
        buffer[i] = static_cast<int16_t>(sample * 32767.0f);
    }
}

float Synthesizer16Bit::generateWaveform(float phase, uint8_t note) {
    // Простая пилообразная волна с фильтром для более приятного звучания
    float saw = 2.0f * (phase / (2.0f * M_PI)) - 1.0f;
    
    // Добавляем немного синусоиды для смягчения
    float sine = sin(phase);
    
    // Смешиваем волны
    return 0.7f * saw + 0.3f * sine;
}

} // namespace yami