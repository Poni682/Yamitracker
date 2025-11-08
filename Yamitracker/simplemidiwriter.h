// simplemidiwriter.h
#ifndef SIMPLEMIDIWRITER_H
#define SIMPLEMIDIWRITER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDataStream>

struct MidiEvent {
    double time;    // время в секундах
    int type;       // тип события: 0x90 - note on, 0x80 - note off
    int channel;    // канал (0-15)
    int note;       // нота (0-127)
    int velocity;   // скорость нажатия (0-127)
};

class SimpleMidiWriter
{
public:
    SimpleMidiWriter();
    ~SimpleMidiWriter();

    bool startRecording(const QString &filename);
    bool stopRecording();
    bool saveToFile(const QString &filename);
    
    void addNoteOn(double time, int channel, int note, int velocity);
    void addNoteOff(double time, int channel, int note);
    
    bool isRecording() const { return m_isRecording; }
    void clear();

private:
    QVector<MidiEvent> m_events;
    bool m_isRecording;
    double m_startTime;
    
    void writeVariableLength(QDataStream &stream, quint32 value);
    void writeMidiHeader(QDataStream &stream);
    void writeMidiTrack(QDataStream &stream);
};

#endif // SIMPLEMIDIWRITER_H