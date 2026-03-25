// midirecorder.cpp
#include "midirecorder.h"
#include <QDataStream>
#include <QDebug>

MidiRecorder::MidiRecorder(QObject *parent)
    : QObject(parent)
    , recording(false)
    , tempo(120)
    , resolution(480)
{
}

MidiRecorder::~MidiRecorder()
{
    if (recording) {
        stopRecording();
    }
}

void MidiRecorder::startRecording(int tempo, int resolution)
{
    events.clear();
    this->tempo = tempo;
    this->resolution = resolution;
    recording = true;
    timer.start();
    startTime = 0;
}

void MidiRecorder::stopRecording()
{
    recording = false;
}

qint64 MidiRecorder::getCurrentTick()
{
    if (!recording) return 0;

    qint64 elapsedMs = timer.elapsed();
    if (startTime == 0) {
        startTime = elapsedMs;
        return 0;
    }

    // Конвертируем миллисекунды в тики
    // Формула: ticks = (ms * resolution * tempo) / (60000)
    qint64 elapsed = elapsedMs - startTime;
    return (elapsed * resolution * tempo) / 60000;
}

void MidiRecorder::addEvent(const QByteArray &data, int type)
{
    if (!recording) return;

    MidiEvent event;
    event.tick = getCurrentTick();
    event.data = data;
    event.type = type;
    events.append(event);
}

void MidiRecorder::recordMidiMessage(const QByteArray &data)
{
    if (!recording || data.isEmpty()) return;
    addEvent(data, 0);
}

void MidiRecorder::recordNoteOn(int channel, int note, int velocity)
{
    QByteArray data;
    data.append(0x90 | (channel & 0x0F));
    data.append(note & 0x7F);
    data.append(velocity & 0x7F);
    addEvent(data, 1);
}

void MidiRecorder::recordNoteOff(int channel, int note, int velocity)
{
    QByteArray data;
    data.append(0x80 | (channel & 0x0F));
    data.append(note & 0x7F);
    data.append(velocity & 0x7F);
    addEvent(data, 2);
}

void MidiRecorder::recordControlChange(int channel, int controller, int value)
{
    QByteArray data;
    data.append(0xB0 | (channel & 0x0F));
    data.append(controller & 0x7F);
    data.append(value & 0x7F);
    addEvent(data, 3);
}

QByteArray MidiRecorder::encodeVariableLength(qint32 value)
{
    QByteArray result;

    if (value == 0) {
        result.append(char(0));
        return result;
    }

    // Формируем байты в обратном порядке
    QVector<char> bytes;
    while (value > 0) {
        bytes.prepend(value & 0x7F);
        value >>= 7;
    }

    // Устанавливаем старший бит для всех байтов кроме последнего
    for (int i = 0; i < bytes.size() - 1; i++) {
        bytes[i] |= 0x80;
    }

    result.append(bytes.data(), bytes.size());
    return result;
}

bool MidiRecorder::writeSMFHeader(QFile &file)
{
    QDataStream out(&file);
    out.setByteOrder(QDataStream::BigEndian);

    // Chunk type "MThd"
    out.writeRawData("MThd", 4);

    // Header length (always 6)
    out << quint32(6);

    // Format (0 = single track)
    out << quint16(0);

    // Number of tracks
    out << quint16(1);

    // Division (resolution)
    out << quint16(resolution);

    return true;
}

bool MidiRecorder::writeSMFTrack(QFile &file)
{
    QDataStream out(&file);
    out.setByteOrder(QDataStream::BigEndian);

    // Временная мета-информация
    QByteArray trackData;
    QDataStream trackStream(&trackData, QIODevice::WriteOnly);
    trackStream.setByteOrder(QDataStream::BigEndian);

    // Добавляем темп (meta event)
    trackStream << encodeVariableLength(0); // Delta time
    trackStream << quint8(0xFF); // Meta event
    trackStream << quint8(0x51); // Set tempo
    trackStream << quint8(3); // Data length

    // Tempo in microseconds per quarter note
    quint32 microPerQuarter = 60000000 / tempo;
    trackStream << quint8((microPerQuarter >> 16) & 0xFF);
    trackStream << quint8((microPerQuarter >> 8) & 0xFF);
    trackStream << quint8(microPerQuarter & 0xFF);

    // Добавляем все MIDI события
    qint64 lastTick = 0;
    for (const MidiEvent &event : events) {
        qint64 delta = event.tick - lastTick;
        trackStream << encodeVariableLength(delta);
        trackStream.writeRawData(event.data.constData(), event.data.size());
        lastTick = event.tick;
    }

    // Добавляем конечный мета-событие
    trackStream << encodeVariableLength(0); // Delta time
    trackStream << quint8(0xFF); // Meta event
    trackStream << quint8(0x2F); // End of track
    trackStream << quint8(0); // Data length

    // Записываем трек в файл
    out.writeRawData("MTrk", 4); // Track chunk type
    out << quint32(trackData.size()); // Track length
    out.writeRawData(trackData.constData(), trackData.size());

    return true;
}

bool MidiRecorder::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file:" << filename;
        return false;
    }

    if (!writeSMFHeader(file)) {
        return false;
    }

    if (!writeSMFTrack(file)) {
        return false;
    }

    file.close();
    qDebug() << "Saved" << events.size() << "events to" << filename;
    return true;
}
