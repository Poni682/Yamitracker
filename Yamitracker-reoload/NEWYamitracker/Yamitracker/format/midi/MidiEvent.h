#ifndef MIDIEVENT_H
#define MIDIEVENT_H
#include <QDataStream>

struct MidiEvent {
    qint64 tick;        // Время в тиках
    QByteArray data;    // MIDI данные
    int type;           // Тип события

    MidiEvent() : tick(0), type(0) {}
    MidiEvent(qint64 t, const QByteArray &d, int tp)
        : tick(t), data(d), type(tp) {}
};

#endif // MIDIEVENT_H
