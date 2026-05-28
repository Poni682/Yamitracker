#ifndef ALSAPLAYER_H
#define ALSAPLAYER_H

#include <alsa/asoundlib.h>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include <mutex>
#include <functional>

namespace yami {

class ALSAPlayer {
private:
    snd_pcm_t* pcmHandle;
    std::thread audioThread;
    std::atomic<bool> running;
    std::mutex audioMutex;
    
    struct NoteEvent {
        uint8_t note;
        uint8_t velocity;
        double startTime;
        double duration;
        bool active;
    };
    
    std::vector<NoteEvent> scheduledNotes;
    uint32_t sampleRate;
    double currentTime;
    
    void audioThreadFunction();
    void generateAudioBuffer(int16_t* buffer, uint32_t frames);
    float generateNoteSample(uint8_t note, double time);
    
public:
    ALSAPlayer();
    ~ALSAPlayer();
    
    bool initialize();
    void playNote(uint8_t note, uint8_t velocity = 127, double duration = 0.0);
    void stopNote(uint8_t note);
    void playYamiFile(const std::string& filename);
    void stop();
    void setSampleRate(uint32_t rate) { sampleRate = rate; }
};

} // namespace yami

#endif