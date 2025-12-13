#include "yamiplay.h"
#include <iostream>
#include <chrono>

namespace yami {

ALSAPlayer::ALSAPlayer() 
    : pcmHandle(nullptr), running(false), sampleRate(44100), 
      currentTime(0.0), masterVolume(0.3f) {
    voices.resize(8); // 8 голосов полифонии
}

ALSAPlayer::~ALSAPlayer() {
    stopAll();
    if (pcmHandle) {
        snd_pcm_close(pcmHandle);
    }
}

bool ALSAPlayer::initialize() {
    int err = snd_pcm_open(&pcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        std::cerr << "Cannot open audio device: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    snd_pcm_hw_params_t* hw_params;
    snd_pcm_hw_params_alloca(&hw_params);
    
    err = snd_pcm_hw_params_any(pcmHandle, hw_params);
    if (err < 0) {
        std::cerr << "Cannot initialize hardware parameters: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    err = snd_pcm_hw_params_set_access(pcmHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        std::cerr << "Cannot set access type: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    err = snd_pcm_hw_params_set_format(pcmHandle, hw_params, SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        std::cerr << "Cannot set sample format: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    unsigned int actualRate = sampleRate;
    err = snd_pcm_hw_params_set_rate_near(pcmHandle, hw_params, &actualRate, 0);
    if (err < 0) {
        std::cerr << "Cannot set sample rate: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    err = snd_pcm_hw_params_set_channels(pcmHandle, hw_params, 1);
    if (err < 0) {
        std::cerr << "Cannot set channel count: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    err = snd_pcm_hw_params(pcmHandle, hw_params);
    if (err < 0) {
        std::cerr << "Cannot set hardware parameters: " << snd_strerror(err) << std::endl;
        return false;
    }
    
    running = true;
    audioThread = std::thread(&ALSAPlayer::audioThreadFunction, this);
    
    return true;
}

void ALSAPlayer::audioThreadFunction() {
    const uint32_t bufferSize = 512;
    int16_t buffer[bufferSize];
    
    while (running) {
        generateSamples(buffer, bufferSize);
        
        int frames = snd_pcm_writei(pcmHandle, buffer, bufferSize);
        if (frames == -EPIPE) {
            std::cerr << "Underrun occurred" << std::endl;
            snd_pcm_prepare(pcmHandle);
        } else if (frames < 0) {
            std::cerr << "Error writing to PCM device: " << snd_strerror(frames) << std::endl;
        }
        
        // Небольшая пауза чтобы не грузить CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ALSAPlayer::generateSamples(int16_t* buffer, uint32_t count) {
    std::lock_guard<std::mutex> lock(audioMutex);
    
    for (uint32_t i = 0; i < count; ++i) {
        float sample = 0.0f;
        
        for (auto& voice : voices) {
            if (voice.active) {
                sample += generateWaveform(voice.phase, voice.note) * voice.amplitude;
                voice.phase += voice.phaseIncrement;
                
                if (voice.phase >= 2.0 * M_PI) {
                    voice.phase -= 2.0 * M_PI;
                }
                
                // ADSR envelope - release phase
                if (voice.releaseTime > 0.0f) {
                    voice.amplitude -= voice.amplitude * 0.01f;
                    if (voice.amplitude < 0.001f) {
                        voice.active = false;
                    }
                }
            }
        }
        
        // Ограничение и преобразование в 16-бит
        sample = std::max(-1.0f, std::min(1.0f, sample));
        buffer[i] = static_cast<int16_t>(sample * 32767.0f);
        
        currentTime += 1.0 / sampleRate;
    }
}

float ALSAPlayer::generateWaveform(double phase, uint8_t note) {
    // Пилообразная волна с небольшой синусоидальной составляющей
    double saw = 2.0 * (phase / (2.0 * M_PI)) - 1.0;
    double sine = sin(phase);
    
    // Смешиваем волны для более интересного тембра
    return 0.7f * saw + 0.3f * sine;
}

void ALSAPlayer::playNote(uint8_t note, uint8_t velocity, double duration) {
    std::lock_guard<std::mutex> lock(audioMutex);
    
    for (auto& voice : voices) {
        if (!voice.active) {
            voice.note = note;
            voice.phase = 0.0;
            double frequency = 440.0 * pow(2.0, (note - 69) / 12.0);
            voice.phaseIncrement = (2.0 * M_PI * frequency) / sampleRate;
            voice.amplitude = (velocity / 127.0f) * masterVolume;
            voice.releaseTime = 0.0f;
            voice.active = true;
            std::cout << "Playing note: " << (int)note << " (freq: " << frequency << "Hz)" << std::endl;
            break;
        }
    }
}

void ALSAPlayer::stopNote(uint8_t note) {
    std::lock_guard<std::mutex> lock(audioMutex);
    
    for (auto& voice : voices) {
        if (voice.active && voice.note == note) {
            voice.releaseTime = 0.1f; // 100ms release
        }
    }
}

void ALSAPlayer::stopAll() {
    running = false;
    if (audioThread.joinable()) {
        audioThread.join();
    }
    
    std::lock_guard<std::mutex> lock(audioMutex);
    for (auto& voice : voices) {
        voice.active = false;
    }
}

} // namespace yami