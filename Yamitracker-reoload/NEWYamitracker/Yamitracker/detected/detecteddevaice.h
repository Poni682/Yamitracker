// #ifndef DETECTEDDEVAICE_H
// #define DETECTEDDEVAICE_H

// #include <QObject>
// #include <QTimer>
// #include <QProcess>
// #include <QMap>
// #include <QSerialPortInfo>

// class DetectedDevaice : QObject{
//     Q_OBJECT

// public:
//     explicit DetectedDevaice(QObject *parent = nullptr);
//     ~DetectedDevaice();

//     // Статус подключения
//     bool isConnected() const { return m_isConnected; }
//     QString getCurrentPort() const { return m_currentPort; }
//     QString getDeviceName() const { return m_deviceName; }

//     // Управление
//     void startDetection(int intervalMs = 5000);
//     void stopDetection();
//     void forceReconnect();

// signals:
//     void deviceConnected(const QString &port, const QString &deviceName);
//     void deviceDisconnected();
//     void deviceError(const QString &error);
//     void midiEvent(const QByteArray &data);

// public slots:
//     void checkDevices();

// private slots:
//     void onMidiDataReady();
//     void onProcessFinished();

// private:
//     void detectMIDIDevices();
//     bool testMIDIDevice(const QString &device);
//     void connectToDevice(const QString &device);
//     void disconnectDevice();
//     QString getDeviceFriendlyName(const QString &device);

//     QTimer *m_detectionTimer;
//     QProcess *m_midiProcess;
//     QTimer *m_readTimer;

//     bool m_isConnected;
//     QString m_currentPort;
//     QString m_deviceName;

//     // Известные производители MIDI устройств
//     const QStringList m_knownVendors = {"Yamaha", "Roland", "Korg", "Casio", "MIDI"};
// };

// #endif // DETECTEDDEVAICE_H
