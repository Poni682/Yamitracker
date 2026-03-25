 // simplifiedmidiwriter.h
#include "midifile.h"
#include <QString>

class SimplifiedMidiWriter
{
public:
    SimplifiedMidiWriter();
    ~SimplifiedMidiWriter();

    bool startRecording(const QString &filename);
    bool stopRecording();
    
    void addNoteOn(double time, int channel, int note, int velocity);
    void addNoteOff(double time, int channel, int note);
    void addControlChange(double time, int channel, int controller, int value);

private:
    smf::MidiFile m_midiFile;
    int m_track;
    bool m_isRecording;
};