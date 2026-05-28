// #include "detecteddevaice.h"
// #include <QDebug>
// #include <QDir>
// #include <QFile>
// #include <QFileInfo>
// #include <QRegularExpression>
// #include <QTextStream>

// DetectedDevaice::DetectedDevaice(QObject *parent)
//     : QObject(parent)
//     , m_detectionTimer(new QTimer(this))
//     , m_midiProcess(new QProcess(this))
//     , m_readTimer(new QTimer(this))
//     , m_isConnected(false)
// {
//     // Настройка таймера обнаружения
//     m_detectionTimer->setSingleShot(false);
//     connect(m_detectionTimer, &QTimer::timeout, this, &DetectedDevaice::checkDevices);

//     // Настройка процесса для чтения MIDI данных
//     m_midiProcess->setProcessChannelMode(QProcess::MergedChannels);
//     connect(m_midiProcess, &QProcess::readyReadStandardOutput, this, &DetectedDevaice::onMidiDataReady);
//     connect(m_midiProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//             this, &DetectedDevaice::onProcessFinished);

//     // Таймер для периодического чтения данных
//     m_readTimer->setInterval(100);
//     m_readTimer->setSingleShot(false);
//     connect(m_readTimer, &QTimer::timeout, this, &DetectedDevaice::onMidiDataReady);

//     // Первоначальная проверка устройств
//     QTimer::singleShot(1000, this, &DetectedDevaice::checkDevices);
// }

// DetectedDevaice::~DetectedDevaice()
// {
//     stopDetection();
//     disconnectDevice();
// }

// void DetectedDevaice::startDetection(int intervalMs)
// {
//     m_detectionTimer->start(intervalMs);
// }

// void DetectedDevaice::stopDetection()
// {
//     m_detectionTimer->stop();
// }

// void DetectedDevaice::forceReconnect()
// {
//     disconnectDevice();
//     checkDevices();
// }

// void DetectedDevaice::checkDevices()
// {
//     // Если уже подключены, проверяем доступность текущего порта
//     if (m_isConnected) {
//         QSerialPortInfo info(m_currentPort);
//         if (!info.isValid()) {
//             qDebug() << "Device disconnected:" << m_currentPort;
//             disconnectDevice();
//         }
//         return;
//     }

//     // Ищем новые MIDI устройства
//     detectMIDIDevices();
// }

// void DetectedDevaice::detectMIDIDevices()
// {
//     qDebug() << "Scanning for MIDI devices...";

//     // Метод 1: Проверка через ALSA (наиболее надежно для Linux)
//     QDir seqDir("/dev/snd/");
//     QStringList midiDevices;

//     // Поиск MIDI устройств в /dev/snd/
//     if (seqDir.exists()) {
//         QStringList filters;
//         filters << "midi*" << "seq";
//         QFileInfoList devices = seqDir.entryInfoList(filters, QDir::System | QDir::Files);

//         foreach (const QFileInfo &device, devices) {
//             QString devPath = device.absoluteFilePath();
//             qDebug() << "Found potential MIDI device:" << devPath;

//             if (testMIDIDevice(devPath)) {
//                 midiDevices.append(devPath);
//             }
//         }
//     }

//     // Метод 2: Проверка через aconnect (ALSA)
//     QProcess aconnectProcess;
//     aconnectProcess.start("aconnect", QStringList() << "-i" << "-l");
//     if (aconnectProcess.waitForFinished(2000)) {
//         QString output = QString::fromLocal8Bit(aconnectProcess.readAllStandardOutput());
//         QRegularExpression regex("client\\s+(\\d+):\\s+'([^']+)'");
//         QRegularExpressionMatchIterator matches = regex.globalMatch(output);

//         while (matches.hasNext()) {
//             QRegularExpressionMatch match = matches.next();
//             QString clientId = match.captured(1);
//             QString clientName = match.captured(2);

//             qDebug() << "ALSA MIDI client found:" << clientId << clientName;

//             // Проверяем, есть ли у клиента выходные порты
//             if (output.contains(QString("client %1").arg(clientId)) &&
//                 output.contains("Output")) {

//                 // Формируем виртуальное устройство ALSA
//                 QString alsaDevice = QString("virtual:%1").arg(clientId);
//                 if (testMIDIDevice(alsaDevice)) {
//                     midiDevices.append(alsaDevice);
//                     m_deviceName = clientName;
//                 }
//             }
//         }
//     }

//     // Метод 3: Проверка через /proc/asound/ (альтернативный метод)
//     QFile cardsFile("/proc/asound/cards");
//     if (cardsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QTextStream in(&cardsFile);
//         QString cardsData = in.readAll();
//         cardsFile.close();

//         QRegularExpression regex("\\[(.*?)\\]");
//         QRegularExpressionMatchIterator matches = regex.globalMatch(cardsData);

//         while (matches.hasNext()) {
//             QRegularExpressionMatch match = matches.next();
//             QString cardName = match.captured(1).trimmed();
//             if (!cardName.isEmpty()) {
//                 qDebug() << "Sound card found:" << cardName;
//             }
//         }
//     }

//     // Подключаемся к первому найденному устройству
//     if (!midiDevices.isEmpty()) {
//         connectToDevice(midiDevices.first());
//     } else {
//         qDebug() << "No MIDI devices found";
//     }
// }

// bool DetectedDevaice::testMIDIDevice(const QString &device)
// {
//     // Для ALSA виртуальных устройств
//     if (device.startsWith("virtual:")) {
//         // Проверяем доступность через aconnect
//         QProcess checkProcess;
//         checkProcess.start("aconnect", QStringList() << "-i" << "-l");
//         if (checkProcess.waitForFinished(2000)) {
//             QString output = checkProcess.readAllStandardOutput();
//             QString clientId = device.mid(8); // Убираем "virtual:"

//             return output.contains(QString("client %1").arg(clientId));
//         }
//         return false;
//     }

//     // Для физических устройств
//     QFile deviceFile(device);
//     if (deviceFile.exists() && deviceFile.permission(QFile::ReadUser | QFile::WriteUser)) {
//         // Пробуем открыть устройство для теста
//         if (deviceFile.open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
//             deviceFile.close();
//             return true;
//         }
//     }

//     return false;
// }

// void DetectedDevaice::connectToDevice(const QString &device)
// {
//     if (m_isConnected && m_currentPort == device) {
//         return; // Уже подключены к этому устройству
//     }

//     disconnectDevice();

//     qDebug() << "Connecting to MIDI device:" << device;

//     m_currentPort = device;
//     m_deviceName = getDeviceFriendlyName(device);

//     // Для физических устройств используем cat для чтения
//     if (!device.startsWith("virtual:")) {
//         m_midiProcess->start("cat", QStringList() << device);
//     } else {
//         // Для ALSA виртуальных устройств используем arecordmidi
//         QString clientId = device.mid(8);
//         m_midiProcess->start("arecordmidi", QStringList() << "-p" << clientId << "/dev/null");
//     }

//     if (m_midiProcess->waitForStarted(2000)) {
//         m_isConnected = true;
//         m_readTimer->start();
//         emit deviceConnected(m_currentPort, m_deviceName);
//         qDebug() << "Successfully connected to:" << m_deviceName << "on" << m_currentPort;
//     } else {
//         QString error = QString("Failed to connect to device: %1").arg(m_midiProcess->errorString());
//         qDebug() << error;
//         emit deviceError(error);
//         disconnectDevice();
//     }
// }

// void DetectedDevaice::disconnectDevice()
// {
//     if (m_midiProcess->state() != QProcess::NotRunning) {
//         m_midiProcess->terminate();
//         m_midiProcess->waitForFinished(1000);
//         if (m_midiProcess->state() != QProcess::NotRunning) {
//             m_midiProcess->kill();
//         }
//     }

//     m_readTimer->stop();
//     m_isConnected = false;
//     m_currentPort.clear();
//     m_deviceName.clear();

//     emit deviceDisconnected();
//     qDebug() << "Device disconnected";
// }

// QString DetectedDevaice::getDeviceFriendlyName(const QString &device)
// {
//     // Для физических устройств пытаемся получить понятное имя
//     if (device.startsWith("/dev/snd/")) {
//         // Пробуем найти相关信息 в /proc/asound/
//         QFile infoFile("/proc/asound/cards");
//         if (infoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//             QTextStream in(&infoFile);
//             QString line;
//             while (in.readLineInto(&line)) {
//                 if (line.contains(device.section('/', -1))) {
//                     QRegularExpression regex("\\[(.*?)\\]");
//                     QRegularExpressionMatch match = regex.match(line);
//                     if (match.hasMatch()) {
//                         QString name = match.captured(1);
//                         infoFile.close();
//                         return name;
//                     }
//                 }
//             }
//             infoFile.close();
//         }
//     }
//     // Для ALSA виртуальных устройств
//     else if (device.startsWith("virtual:")) {
//         QProcess nameProcess;
//         nameProcess.start("aconnect", QStringList() << "-i" << "-l");
//         if (nameProcess.waitForFinished(2000)) {
//             QString output = nameProcess.readAllStandardOutput();
//             QString clientId = device.mid(8);
//             QRegularExpression regex(QString("client\\s+%1:\\s+'([^']+)'").arg(clientId));
//             QRegularExpressionMatch match = regex.match(output);
//             if (match.hasMatch()) {
//                 return match.captured(1);
//             }
//         }
//     }

//     // Проверяем известных производителей
//     foreach (const QString &vendor, m_knownVendors) {
//         if (device.contains(vendor, Qt::CaseInsensitive)) {
//             return vendor + " Synthesizer";
//         }
//     }

//     // Возвращаем базовое имя файла, если ничего не найдено
//     return device.section('/', -1);
// }

// void DetectedDevaice::onMidiDataReady()
// {
//     if (!m_isConnected || m_midiProcess->state() != QProcess::Running) {
//         return;
//     }

//     QByteArray data = m_midiProcess->readAllStandardOutput();
//     if (!data.isEmpty()) {
//         qDebug() << "MIDI data received:" << data.toHex();
//         emit midiEvent(data);
//     }
// }

// void DetectedDevaice::onProcessFinished()
// {
//     if (m_isConnected) {
//         qDebug() << "MIDI process finished unexpectedly";
//         disconnectDevice();
//     }
// }
