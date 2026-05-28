// csvrecorder.h
#ifndef CSVRECORDER_H
#define CSVRECORDER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QElapsedTimer>

class CsvRecorder : public QObject
{
    Q_OBJECT
public:
    explicit CsvRecorder(QObject *parent = nullptr);
    ~CsvRecorder();

    void startRecording(const QString &filename);
    void stopRecording();

public slots:
    void recordMidiMessage(const QByteArray &data);
    void recordNoteOn(int channel, int note, int velocity);
    void recordNoteOff(int channel, int note, int velocity);
    void recordControlChange(int channel, int controller, int value);

private:
    QFile file;
    QTextStream stream;
    QElapsedTimer timer;
    qint64 startTime;
    bool recording;

    void writeHeader();
    void writeEvent(const QString &type, const QString &details);
};

#endif
