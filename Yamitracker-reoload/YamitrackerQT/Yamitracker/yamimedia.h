#ifndef YAMIMEDIA_H
#define YAMIMEDIA_H

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace yami {

struct Note {
    uint8_t pitch;     
    float start_time;   
    float duration;   
    uint8_t velocity;   
    
    Note(uint8_t p = 60, float st = 0.0f, float d = 1.0f, uint8_t v = 80)
        : pitch(p), start_time(st), duration(d), velocity(v) {}
};

struct Track {
    uint8_t track_id;
    uint8_t channel;
    uint8_t instrument;
    uint8_t volume;
    std::vector<Note> notes;
    
    Track(uint8_t id = 0) : track_id(id), channel(0), instrument(0), volume(100) {}
};

class YamiFile {
private:
    float bpm;
    uint8_t time_signature_numerator;
    uint8_t time_signature_denominator;
    std::vector<Track> tracks;
    
    bool parseHeader(const std::string& line);
    bool parseTrackHeader(const std::string& line, Track& track);
    bool parseNote(const std::string& line, Track& track);
    std::vector<std::string> split(const std::string& str, char delimiter);
    
public:
    YamiFile();
    
    bool loadFromText(const std::string& filename);
    bool saveToText(const std::string& filename);
    
    bool loadFromBinary(const std::string& filename);
    bool saveToBinary(const std::string& filename);
    
    void setBPM(float new_bpm) { bpm = new_bpm; }
    float getBPM() const { return bpm; }
    
    void setTimeSignature(uint8_t num, uint8_t denom) {
        time_signature_numerator = num;
        time_signature_denominator = denom;
    }
    
    std::pair<uint8_t, uint8_t> getTimeSignature() const {
        return {time_signature_numerator, time_signature_denominator};
    }
    
    std::vector<Track>& getTracks() { return tracks; }
    const std::vector<Track>& getTracks() const { return tracks; }
    
    Track& addTrack(uint8_t track_id) {
        tracks.emplace_back(track_id);
        return tracks.back();
    }
    
    void clear() {
        tracks.clear();
        bpm = 120.0f;
        time_signature_numerator = 4;
        time_signature_denominator = 4;
    }
};

uint8_t noteNameToPitch(const std::string& noteName);
std::string pitchToNoteName(uint8_t pitch);

} 

#endif