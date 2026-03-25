// midirecorder.h
#ifndef MIDIRECORDER_H
#define MIDIRECORDER_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QElapsedTimer>
#include "MidiEvent.h"

class MidiRecorder : public QObject
{
    Q_OBJECT
public:
    explicit MidiRecorder(QObject *parent = nullptr);
    ~MidiRecorder();

    void startRecording(int tempo = 120, int resolution = 480);
    void stopRecording();
    bool saveToFile(const QString &filename);
    bool isRecording() const { return recording; }

public slots:
    void recordMidiMessage(const QByteArray &data);
    void recordNoteOn(int channel, int note, int velocity);
    void recordNoteOff(int channel, int note, int velocity);
    void recordControlChange(int channel, int controller, int value);

private:
    QVector<MidiEvent> events;
    QElapsedTimer timer;
    qint64 startTime;
    bool recording;
    int tempo;          // BPM
    int resolution;     // Тиков на четверть

    qint64 getCurrentTick();
    void addEvent(const QByteArray &data, int type);
    QByteArray encodeVariableLength(qint32 value);
    bool writeSMFHeader(QFile &file);
    bool writeSMFTrack(QFile &file);
};

#endif
