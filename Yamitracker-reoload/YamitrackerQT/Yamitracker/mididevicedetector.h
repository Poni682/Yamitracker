#ifndef MIDIDEVICEDETECTOR_H
#define MIDIDEVICEDETECTOR_H

#include <QString>
#include <QList>
#include <QMap>

struct MidiDeviceInfo {
    QString name;
    QString port;
    QString manufacturer;
    QString model;
    bool isInput;
    bool isOutput;
};

class MidiDeviceDetector
{
public:
    MidiDeviceDetector();
    ~MidiDeviceDetector();
    
    QList<MidiDeviceInfo> detectDevices();
    MidiDeviceInfo detectYamahaDevice();
    bool isYamahaDevice(const QString& deviceName);
    
private:
    void scanAlsaDevices();
    QList<MidiDeviceInfo> m_devices;
};

#endif