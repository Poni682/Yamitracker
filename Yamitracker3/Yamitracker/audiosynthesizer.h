#ifndef AUDIOSYNTHESIZER_H
#define AUDIOSYNTHESIZER_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

class AudioSynthesizer
{
public:
    AudioSynthesizer(int sampleRate = 44100);
    ~AudioSynthesizer();
    
    void addNote(int note, int velocity, int instrument = 0);
    void removeNote(int note);
    void clearNotes();
    
    void setInstrument(int instrument);
    void setReverbLevel(float level);
    void setChorusLevel(float level);
    
    short generateSample();
    
private:
    struct Voice {
        int note;
        double phase;
        double phaseInc;
        double amplitude;
        double attackTime;
        double decayTime;
        double sustainLevel;
        double releaseTime;
        double envelope;
        bool isReleasing;
        double time;
        int waveform; // 0=синус, 1=пила, 2=квадрат, 3=треугольник
        double detune;
    };
    
    int sampleRate;
    std::vector<Voice> voices;
    std::vector<float> reverbBuffer;
    int reverbPos;
    float reverbLevel;
    float chorusLevel;
    double lfoPhase;
    double lfoInc;
    
    double noteToFrequency(int note);
    double generateWaveform(double phase, int waveform);
    void updateEnvelope(Voice& voice);
};

#endif