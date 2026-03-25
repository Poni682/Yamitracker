#include "yamimedia.h"
#include <algorithm>
#include <cctype>
#include <cmath>

namespace yami {

YamiFile::YamiFile() 
    : bpm(120.0f), 
      time_signature_numerator(4), time_signature_denominator(4) {}

bool YamiFile::loadFromText(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    clear();
    std::string line;
    Track* current_track = nullptr;
    bool in_notes_section = false;
    
    while (std::getline(file, line)) {
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;
        
        if (line.find("BPM:") == 0) {
            bpm = std::stof(line.substr(4));
        }
        else if (line.find("TIME_SIGNATURE:") == 0) {
            auto parts = split(line.substr(15), '/');
            if (parts.size() == 2) {
                time_signature_numerator = std::stoi(parts[0]);
                time_signature_denominator = std::stoi(parts[1]);
            }
        }
        else if (line.find("TRACK:") == 0) {
            in_notes_section = false;
            uint8_t track_id = std::stoi(line.substr(6));
            tracks.emplace_back(track_id);
            current_track = &tracks.back();
        }
        else if (current_track && line.find("CHANNEL:") == 0) {
            current_track->channel = std::stoi(line.substr(8));
        }
        else if (current_track && line.find("INSTRUMENT:") == 0) {
            current_track->instrument = std::stoi(line.substr(11));
        }
        else if (current_track && line.find("VOLUME:") == 0) {
            current_track->volume = std::stoi(line.substr(7));
        }
        else if (current_track && line == "NOTES:") {
            in_notes_section = true;
        }
        else if (in_notes_section && current_track) {
            parseNote(line, *current_track);
        }
    }
    
    return true;
}

bool YamiFile::saveToText(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# YAMI Format - Yet Another Music Interface\n";
    file << "BPM:" << bpm << "\n";
    file << "TIME_SIGNATURE:" << (int)time_signature_numerator << "/" 
         << (int)time_signature_denominator << "\n";
    file << "TRACKS:" << tracks.size() << "\n\n";
    
    for (const auto& track : tracks) {
        file << "# Track " << (int)track.track_id << "\n";
        file << "TRACK:" << (int)track.track_id << "\n";
        file << "CHANNEL:" << (int)track.channel << "\n";
        file << "INSTRUMENT:" << (int)track.instrument << "\n";
        file << "VOLUME:" << (int)track.volume << "\n";
        file << "NOTES:\n";
        
        for (const auto& note : track.notes) {
            file << pitchToNoteName(note.pitch) << ","
                 << note.start_time << ","
                 << note.duration << ","
                 << (int)note.velocity << "\n";
        }
        file << "\n";
    }
    
    return true;
}

bool YamiFile::loadFromBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    clear();
    
    char magic[4];
    file.read(magic, 4);
    if (std::string(magic, 4) != "YAMB") {
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&bpm), sizeof(bpm));
    file.read(reinterpret_cast<char*>(&time_signature_numerator), sizeof(time_signature_numerator));
    file.read(reinterpret_cast<char*>(&time_signature_denominator), sizeof(time_signature_denominator));
    
    uint16_t num_tracks;
    file.read(reinterpret_cast<char*>(&num_tracks), sizeof(num_tracks));
    
    for (uint16_t i = 0; i < num_tracks; ++i) {
        Track track;
        file.read(reinterpret_cast<char*>(&track.track_id), sizeof(track.track_id));
        file.read(reinterpret_cast<char*>(&track.channel), sizeof(track.channel));
        file.read(reinterpret_cast<char*>(&track.instrument), sizeof(track.instrument));
        file.read(reinterpret_cast<char*>(&track.volume), sizeof(track.volume));
        
        uint32_t num_notes;
        file.read(reinterpret_cast<char*>(&num_notes), sizeof(num_notes));
        
        for (uint32_t j = 0; j < num_notes; ++j) {
            Note note;
            file.read(reinterpret_cast<char*>(&note.pitch), sizeof(note.pitch));
            file.read(reinterpret_cast<char*>(&note.start_time), sizeof(note.start_time));
            file.read(reinterpret_cast<char*>(&note.duration), sizeof(note.duration));
            file.read(reinterpret_cast<char*>(&note.velocity), sizeof(note.velocity));
            track.notes.push_back(note);
        }
        
        tracks.push_back(track);
    }
    
    return true;
}

bool YamiFile::saveToBinary(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    const char magic[] = "YAMB";
    file.write(magic, 4);
    file.write(reinterpret_cast<const char*>(&bpm), sizeof(bpm));
    file.write(reinterpret_cast<const char*>(&time_signature_numerator), sizeof(time_signature_numerator));
    file.write(reinterpret_cast<const char*>(&time_signature_denominator), sizeof(time_signature_denominator));
    
    uint16_t num_tracks = tracks.size();
    file.write(reinterpret_cast<const char*>(&num_tracks), sizeof(num_tracks));
    
    for (const auto& track : tracks) {
        file.write(reinterpret_cast<const char*>(&track.track_id), sizeof(track.track_id));
        file.write(reinterpret_cast<const char*>(&track.channel), sizeof(track.channel));
        file.write(reinterpret_cast<const char*>(&track.instrument), sizeof(track.instrument));
        file.write(reinterpret_cast<const char*>(&track.volume), sizeof(track.volume));
        
        uint32_t num_notes = track.notes.size();
        file.write(reinterpret_cast<const char*>(&num_notes), sizeof(num_notes));
        
        for (const auto& note : track.notes) {
            file.write(reinterpret_cast<const char*>(&note.pitch), sizeof(note.pitch));
            file.write(reinterpret_cast<const char*>(&note.start_time), sizeof(note.start_time));
            file.write(reinterpret_cast<const char*>(&note.duration), sizeof(note.duration));
            file.write(reinterpret_cast<const char*>(&note.velocity), sizeof(note.velocity));
        }
    }
    
    return true;
}

bool YamiFile::parseHeader(const std::string& line) {
    return true;
}

bool YamiFile::parseTrackHeader(const std::string& line, Track& track) {
    return true;
}

bool YamiFile::parseNote(const std::string& line, Track& track) {
    auto parts = split(line, ',');
    if (parts.size() != 4) {
        return false;
    }
    
    try {
        uint8_t pitch = noteNameToPitch(parts[0]);
        float start_time = std::stof(parts[1]);
        float duration = std::stof(parts[2]);
        uint8_t velocity = std::stoi(parts[3]);
        
        track.notes.emplace_back(pitch, start_time, duration, velocity);
        return true;
    }
    catch (...) {
        return false;
    }
}

std::vector<std::string> YamiFile::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

uint8_t noteNameToPitch(const std::string& noteName) {
    static const std::unordered_map<char, int> noteBase = {
        {'C', 0}, {'D', 2}, {'E', 4}, {'F', 5}, {'G', 7}, {'A', 9}, {'B', 11}
    };
    
    if (noteName.length() < 2) return 60;
    
    char noteChar = std::toupper(noteName[0]);
    int octave = std::stoi(noteName.substr(1));
    
    int pitch = (octave + 1) * 12;
    
    auto it = noteBase.find(noteChar);
    if (it != noteBase.end()) {
        pitch += it->second;
    }
    
    if (noteName.find('#') != std::string::npos) {
        pitch += 1;
    }
    
    return std::min(std::max(pitch, 0), 127);
}

std::string pitchToNoteName(uint8_t pitch) {
    static const std::string noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    
    int octave = (pitch / 12) - 1;
    int noteIndex = pitch % 12;
    
    return noteNames[noteIndex] + std::to_string(octave);
}

}