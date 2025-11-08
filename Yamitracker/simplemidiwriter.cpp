// simplemidiwriter.cpp
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
    
    // Записываем заголовок MIDI файла
    writeMidiHeader(stream);
    
    // Записываем трек с событиями
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
    event.type = 0x90; // Note On
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
    event.type = 0x80; // Note Off
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
    // MIDI header chunk
    stream.writeRawData("MThd", 4);        // Chunk type
    stream << (quint32)6;                  // Chunk length
    stream << (quint16)1;                  // Format type (single track)
    stream << (quint16)1;                  // Number of tracks
    stream << (quint16)480;                // Ticks per quarter note
}

void SimpleMidiWriter::writeMidiTrack(QDataStream &stream)
{
    // Временный буфер для расчета длины трека
    QByteArray trackData;
    QDataStream trackStream(&trackData, QIODevice::WriteOnly);
    trackStream.setByteOrder(QDataStream::BigEndian);
    
    double lastTime = 0;
    
    // Добавляем события в трек
    for (const MidiEvent &event : m_events) {
        // Вычисляем дельту времени в тиках
        quint32 deltaTime = (event.time - lastTime) * 480; // 480 тиков в секунду
        lastTime = event.time;
        
        // Записываем дельту времени
        writeVariableLength(trackStream, deltaTime);
        
        // Записываем MIDI событие
        quint8 statusByte = event.type | (event.channel & 0x0F);
        trackStream << statusByte;
        trackStream << (quint8)event.note;
        trackStream << (quint8)event.velocity;
    }
    
    // Добавляем конец трека
    writeVariableLength(trackStream, 0); // delta time
    trackStream.writeRawData("\xFF\x2F\x00", 3); // end of track
    
    // Записываем заголовок трека
    stream.writeRawData("MTrk", 4); // Chunk type
    stream << (quint32)trackData.size(); // Chunk length
    stream.writeRawData(trackData.constData(), trackData.size()); // Track data
}