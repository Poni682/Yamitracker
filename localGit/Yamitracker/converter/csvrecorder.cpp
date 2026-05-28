// csvrecorder.cpp
#include "csvrecorder.h"
#include <QDebug>

CsvRecorder::CsvRecorder(QObject *parent)
    : QObject(parent)
    , recording(false)
{
}

CsvRecorder::~CsvRecorder()
{
    if (recording) {
        stopRecording();
    }
}

void CsvRecorder::startRecording(const QString &filename)
{
    file.setFileName(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << filename;
        return;
    }

    stream.setDevice(&file);
    writeHeader();

    recording = true;
    timer.start();
    startTime = 0;
}

void CsvRecorder::stopRecording()
{
    if (recording) {
        recording = false;
        file.close();
        qDebug() << "Recording saved to" << file.fileName();
    }
}

void CsvRecorder::writeHeader()
{
    stream << "time_ms,type,channel,note,velocity,controller,value,raw_data\n";
    stream.flush();
}

void CsvRecorder::writeEvent(const QString &type, const QString &details)
{
    if (!recording) return;

    qint64 elapsed = timer.elapsed();
    if (startTime == 0) {
        startTime = elapsed;
        elapsed = 0;
    } else {
        elapsed -= startTime;
    }

    stream << elapsed << "," << type << "," << details << "\n";
    stream.flush();
}

void CsvRecorder::recordMidiMessage(const QByteArray &data)
{
    QString details = QString(",,,,\"%1\"").arg(QString(data.toHex()));
    writeEvent("MIDI", details);
}

void CsvRecorder::recordNoteOn(int channel, int note, int velocity)
{
    QString details = QString("%1,%2,%3,,,")
    .arg(channel)
        .arg(note)
        .arg(velocity);
    writeEvent("NOTE_ON", details);
}

void CsvRecorder::recordNoteOff(int channel, int note, int velocity)
{
    QString details = QString("%1,%2,%3,,,")
    .arg(channel)
        .arg(note)
        .arg(velocity);
    writeEvent("NOTE_OFF", details);
}

void CsvRecorder::recordControlChange(int channel, int controller, int value)
{
    QString details = QString("%1,,,%2,%3,")
    .arg(channel)
        .arg(controller)
        .arg(value);
    writeEvent("CONTROL", details);
}
