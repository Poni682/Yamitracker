// midireader.h
#ifndef MIDIREADER_H
#define MIDIREADER_H

#include <QObject>
#include <QSocketNotifier>
#include <alsa/asoundlib.h>

class MidiReader : public QObject
{
    Q_OBJECT
public:
    explicit MidiReader(QObject *parent = nullptr);
    ~MidiReader();

    bool connectToMidiDevice(const QString &deviceName = QString());
    QStringList getAvailableDevices();

signals:
    void noteOn(int channel, int note, int velocity);
    void noteOff(int channel, int note, int velocity);
    void controlChange(int channel, int controller, int value);
    void pitchBend(int channel, int value);
    void midiMessage(const QByteArray &data);

private slots:
    void readMidiData();

private:
    snd_seq_t *seq;
    int clientId;
    int portId;
    QSocketNotifier *notifier;

    void processMidiEvent(snd_seq_event_t *ev);
};

#endif
