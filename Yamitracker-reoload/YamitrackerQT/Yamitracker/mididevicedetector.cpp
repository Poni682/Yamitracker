#include "mididevicedetector.h"
#include <alsa/asoundlib.h>
#include <QDebug>
#include <QRegularExpression>

MidiDeviceDetector::MidiDeviceDetector()
{
    scanAlsaDevices();
}

MidiDeviceDetector::~MidiDeviceDetector()
{
}

QList<MidiDeviceInfo> MidiDeviceDetector::detectDevices()
{
    return m_devices;
}

MidiDeviceInfo MidiDeviceDetector::detectYamahaDevice()
{
    for (const auto& device : m_devices) {
        if (isYamahaDevice(device.name)) {
            return device;
        }
    }
    
    // Возвращаем устройство по умолчанию, если Yamaha не найдена
    MidiDeviceInfo defaultDevice;
    defaultDevice.port = "/dev/dmmidi1";
    defaultDevice.name = "Yamaha PSR-E333 (Default)";
    defaultDevice.manufacturer = "Yamaha";
    defaultDevice.model = "PSR-E333";
    defaultDevice.isInput = true;
    defaultDevice.isOutput = true;
    
    return defaultDevice;
}

bool MidiDeviceDetector::isYamahaDevice(const QString& deviceName)
{
    QString name = deviceName.toLower();
    
    // Паттерны для идентификации Yamaha устройств
    QList<QString> yamahaPatterns = {
        "yamaha", "psr", "e333", "e443", "e343", "dgx", "motif", "mox", "montage"
    };
    
    for (const QString& pattern : yamahaPatterns) {
        if (name.contains(pattern)) {
            return true;
        }
    }
    
    return false;
}

void MidiDeviceDetector::scanAlsaDevices()
{
    m_devices.clear();
    
    // Сканируем ALSA MIDI устройства
    snd_seq_t *seq;
    if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
        qDebug() << "Cannot open ALSA sequencer";
        return;
    }
    
    snd_seq_set_client_name(seq, "Yamitracker Scanner");
    
    // Получаем список клиентов
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(seq, cinfo) >= 0) {
        int client = snd_seq_client_info_get_client(cinfo);
        
        snd_seq_port_info_set_client(pinfo, client);
        snd_seq_port_info_set_port(pinfo, -1);
        while (snd_seq_query_next_port(seq, pinfo) >= 0) {
            unsigned int caps = snd_seq_port_info_get_capability(pinfo);
            
            if ((caps & (SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ)) &&
                (caps & (SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE))) {
                
                MidiDeviceInfo device;
                device.name = QString("%1: %2").arg(
                    snd_seq_client_info_get_name(cinfo),
                    snd_seq_port_info_get_name(pinfo)
                );
                device.port = QString("hw:%1,%2").arg(client).arg(snd_seq_port_info_get_port(pinfo));
                device.manufacturer = snd_seq_client_info_get_name(cinfo);
                device.model = snd_seq_port_info_get_name(pinfo);
                device.isInput = (caps & SND_SEQ_PORT_CAP_READ) != 0;
                device.isOutput = (caps & SND_SEQ_PORT_CAP_WRITE) != 0;
                
                m_devices.append(device);
            }
        }
    }
    
    snd_seq_close(seq);
    
    // Добавляем стандартное устройство для обратной совместимости
    MidiDeviceInfo defaultDevice;
    defaultDevice.name = "Direct MIDI (dmmidi1)";
    defaultDevice.port = "/dev/dmmidi1";
    defaultDevice.manufacturer = "Yamaha";
    defaultDevice.model = "PSR-E333";
    defaultDevice.isInput = true;
    defaultDevice.isOutput = true;
    m_devices.append(defaultDevice);
    
    qDebug() << "Found" << m_devices.size() << "MIDI devices";
    for (const auto& device : m_devices) {
        qDebug() << "Device:" << device.name << "Port:" << device.port;
    }
}