#include "simplemidiwriter.h"
#include <QDateTime>
#include <QDebug>

SimpleMidiWriter::SimpleMidiWriter()
    : m_isRecording(false)
    , m_startTime(0)
{
}

SimpleMidiWriter::~SimpleMidiWriter()
{
}

bool SimpleMidiWriter::startRecording(const QString &filename)
{
    if (m_isRecording) {
        stopRecording();
    }
    
    m_events.clear();
    m_isRecording = true;
    m_startTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    
    qDebug() << "Started MIDI recording";
    return true;
}

bool SimpleMidiWriter::stopRecording()
{
    if (!m_isRecording) {
        return false;
    }
    
    m_isRecording = false;
    qDebug() << "Stopped MIDI recording, events count:" << m_events.size();
    return true;
}

bool SimpleMidiWriter::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }
    
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::BigEndian);
    
    writeMidiHeader(stream);
    
    writeMidiTrack(stream);
    
    file.close();
    qDebug() << "MIDI file saved:" << filename;
    return true;
}

void SimpleMidiWriter::addNoteOn(double time, int channel, int note, int velocity)
{
    if (!m_isRecording) return;
    
    MidiEvent event;
    event.time = time;
    event.type = 0x90;
    event.channel = channel;
    event.note = note;
    event.velocity = velocity;
    
    m_events.append(event);
}

void SimpleMidiWriter::addNoteOff(double time, int channel, int note)
{
    if (!m_isRecording) return;
    
    MidiEvent event;
    event.time = time;
    event.type = 0x80;
    event.channel = channel;
    event.note = note;
    event.velocity = 0;
    
    m_events.append(event);
}

void SimpleMidiWriter::clear()
{
    m_events.clear();
    m_isRecording = false;
}

void SimpleMidiWriter::writeVariableLength(QDataStream &stream, quint32 value)
{
    quint32 buffer = value & 0x7F;
    
    while ((value >>= 7) > 0) {
        buffer <<= 8;
        buffer |= 0x80;
        buffer += (value & 0x7F);
    }
    
    while (true) {
        stream << (quint8)(buffer & 0xFF);
        if (buffer & 0x80) {
            buffer >>= 8;
        } else {
            break;
        }
    }
}

void SimpleMidiWriter::writeMidiHeader(QDataStream &stream)
{
    stream.writeRawData("MThd", 4);
    stream << (quint32)6;
    stream << (quint16)1;
    stream << (quint16)1;
    stream << (quint16)480;
}

void SimpleMidiWriter::writeMidiTrack(QDataStream &stream)
{
    QByteArray trackData;
    QDataStream trackStream(&trackData, QIODevice::WriteOnly);
    trackStream.setByteOrder(QDataStream::BigEndian);
    
    double lastTime = 0;
    
    for (const MidiEvent &event : m_events) {
        quint32 deltaTime = (event.time - lastTime) * 480; 
        lastTime = event.time;
        
        writeVariableLength(trackStream, deltaTime);
        
        quint8 statusByte = event.type | (event.channel & 0x0F);
        trackStream << statusByte;
        trackStream << (quint8)event.note;
        trackStream << (quint8)event.velocity;
    }
    
    writeVariableLength(trackStream, 0);
    trackStream.writeRawData("\xFF\x2F\x00", 3);
    
    stream.writeRawData("MTrk", 4); 
    stream << (quint32)trackData.size();
    stream.writeRawData(trackData.constData(), trackData.size());
}       