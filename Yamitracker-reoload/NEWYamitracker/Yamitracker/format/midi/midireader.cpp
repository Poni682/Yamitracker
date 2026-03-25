// midireader.cpp
#include "midireader.h"
#include <QDebug>
#include <QSocketNotifier>

MidiReader::MidiReader(QObject *parent)
    : QObject(parent)
    , seq(nullptr)
    , notifier(nullptr)
{
    // Открываем ALSA секвенсор
    if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
        qCritical() << "Cannot open ALSA sequencer";
        return;
    }

    // Устанавливаем имя клиента
    snd_seq_set_client_name(seq, "Qt MIDI Reader");

    // Создаем порт для входящих сообщений
    clientId = snd_seq_client_id(seq);
    portId = snd_seq_create_simple_port(
        seq,
        "input",
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION
        );

    if (portId < 0) {
        qCritical() << "Cannot create port";
        return;
    }

    // Создаем сокет для отслеживания MIDI событий
    int pollFd = snd_seq_poll_descriptors_count(seq, POLLIN);
    if (pollFd > 0) {
        struct pollfd *pfd = new struct pollfd[pollFd];
        snd_seq_poll_descriptors(seq, pfd, pollFd, POLLIN);

        notifier = new QSocketNotifier(pfd[0].fd, QSocketNotifier::Read, this);
        connect(notifier, &QSocketNotifier::activated, this, &MidiReader::readMidiData);

        delete[] pfd;
    }
}

MidiReader::~MidiReader()
{
    if (notifier) delete notifier;
    if (seq) snd_seq_close(seq);
}

QStringList MidiReader::getAvailableDevices()
{
    QStringList devices;

    if (!seq) return devices;

    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    snd_seq_client_info_set_client(cinfo, -1);

    while (snd_seq_query_next_client(seq, cinfo) >= 0) {
        int client = snd_seq_client_info_get_client(cinfo);

        // Пропускаем системные клиенты
        if (client == 0) continue;

        snd_seq_port_info_set_client(pinfo, client);
        snd_seq_port_info_set_port(pinfo, -1);

        while (snd_seq_query_next_port(seq, pinfo) >= 0) {
            int caps = snd_seq_port_info_get_capability(pinfo);

            // Ищем порты, которые могут отправлять данные
            if (caps & SND_SEQ_PORT_CAP_READ) {
                QString deviceName = QString("%1:%2 %3")
                .arg(snd_seq_client_info_get_name(cinfo))
                    .arg(snd_seq_port_info_get_name(pinfo));

                devices << deviceName;
            }
        }
    }

    return devices;
}

bool MidiReader::connectToMidiDevice(const QString &deviceName)
{
    if (!seq) return false;

    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    snd_seq_client_info_set_client(cinfo, -1);

    while (snd_seq_query_next_client(seq, cinfo) >= 0) {
        int client = snd_seq_client_info_get_client(cinfo);
        if (client == 0) continue;

        snd_seq_port_info_set_client(pinfo, client);
        snd_seq_port_info_set_port(pinfo, -1);

        while (snd_seq_query_next_port(seq, pinfo) >= 0) {
            QString currentName = QString("%1:%2")
            .arg(snd_seq_client_info_get_name(cinfo))
                .arg(snd_seq_port_info_get_name(pinfo));

            if (currentName == deviceName) {
                // Подключаемся к найденному порту
                int result = snd_seq_connect_from(seq, portId, client,
                                                  snd_seq_port_info_get_port(pinfo));
                if (result >= 0) {
                    qDebug() << "Connected to:" << currentName;
                    return true;
                } else {
                    qDebug() << "Failed to connect:" << snd_strerror(result);
                    return false;
                }
            }
        }
    }

    return false;
}

void MidiReader::readMidiData()
{
    if (!seq) return;

    snd_seq_event_t *ev;

    // Читаем все доступные события
    do {
        snd_seq_event_input(seq, &ev);

        if (ev) {
            processMidiEvent(ev);
            snd_seq_free_event(ev);
        }
    } while (snd_seq_event_input_pending(seq, 0) > 0);
}

void MidiReader::processMidiEvent(snd_seq_event_t *ev)
{
    QByteArray midiData;

    switch (ev->type) {
    case SND_SEQ_EVENT_NOTEON:
        if (ev->data.note.velocity > 0) {
            emit noteOn(ev->data.note.channel,
                        ev->data.note.note,
                        ev->data.note.velocity);

            // Формируем сырые MIDI данные
            midiData.append(static_cast<char>(0x90 | (ev->data.note.channel & 0x0F)));
            midiData.append(static_cast<char>(ev->data.note.note & 0x7F));
            midiData.append(static_cast<char>(ev->data.note.velocity & 0x7F));
        } else {
            emit noteOff(ev->data.note.channel,
                         ev->data.note.note,
                         0);

            midiData.append(static_cast<char>(0x80 | (ev->data.note.channel & 0x0F)));
            midiData.append(static_cast<char>(ev->data.note.note & 0x7F));
            midiData.append(static_cast<char>(0));
        }
        break;

    case SND_SEQ_EVENT_NOTEOFF:
        emit noteOff(ev->data.note.channel,
                     ev->data.note.note,
                     ev->data.note.velocity);

        midiData.append(static_cast<char>(0x80 | (ev->data.note.channel & 0x0F)));
        midiData.append(static_cast<char>(ev->data.note.note & 0x7F));
        midiData.append(static_cast<char>(ev->data.note.velocity & 0x7F));
        break;

    case SND_SEQ_EVENT_CONTROLLER:
        emit controlChange(ev->data.control.channel,
                           ev->data.control.param,
                           ev->data.control.value);

        midiData.append(static_cast<char>(0xB0 | (ev->data.control.channel & 0x0F)));
        midiData.append(static_cast<char>(ev->data.control.param & 0x7F));
        midiData.append(static_cast<char>(ev->data.control.value & 0x7F));
        break;

    case SND_SEQ_EVENT_PITCHBEND:
        emit pitchBend(ev->data.control.channel,
                       ev->data.control.value);

        midiData.append(static_cast<char>(0xE0 | (ev->data.control.channel & 0x0F)));
        midiData.append(static_cast<char>(ev->data.control.value & 0x7F));
        midiData.append(static_cast<char>((ev->data.control.value >> 7) & 0x7F));
        break;

    default:
        // Другие типы MIDI событий
        break;
    }

    if (!midiData.isEmpty()) {
        emit midiMessage(midiData);
    }
}
