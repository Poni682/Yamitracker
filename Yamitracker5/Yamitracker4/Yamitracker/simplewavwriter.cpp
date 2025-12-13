#include "simplewavwriter.h"
#include <QDebug>
#include <QDateTime>
#include <cmath>
#include <algorithm>

SimpleWavWriter::SimpleWavWriter(QObject *parent) 
    : QObject(parent), recording(false), paused(false), playbackSpeed(1.0), bassVolumeMultiplier(2.0)
{
    // Инициализируем дорожки
    tracks.resize(MAX_TRACKS);
    for (int i = 0; i < MAX_TRACKS; i++) {
        tracks[i] = Track(i); // Используем конструктор с индексом
        tracks[i].instrument = 1; // По умолчанию Grand Piano
        tracks[i].volume = 1.0;
        tracks[i].muted = false;
        tracks[i].solo = false;
    }
    
    // Настраиваем формат аудио
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(1); // Моно
    format.setSampleSize(BIT_DEPTH);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
}

bool SimpleWavWriter::startRecording(int sampleRate)
{
    if (recording) return false;
    
    format.setSampleRate(sampleRate);
    recording = true;
    paused = false;
    
    emit recordingStatusChanged(true);
    return true;
}

void SimpleWavWriter::stopRecording()
{
    recording = false;
    paused = false;
    emit recordingStatusChanged(false);
}

void SimpleWavWriter::pauseRecording()
{
    if (recording && !paused) {
        paused = true;
    }
}

void SimpleWavWriter::resumeRecording()
{
    if (recording && paused) {
        paused = false;
    }
}

void SimpleWavWriter::addNote(int track, double time, int note, int velocity, double duration, int instrument)
{
    if (track < 0 || track >= MAX_TRACKS) return;
    
    // Применяем множитель громкости для басовых инструментов
    double volume = velocity / 127.0;
    static QSet<int> bassInstruments = {28, 33, 39, 40, 29, 37, 38};
    if (bassInstruments.contains(instrument)) {
        volume *= bassVolumeMultiplier;
        volume = qMin(volume, 1.0); // Ограничиваем максимальную громкость
    }
    
    tracks[track].events.append(NoteEvent(time, note, velocity, duration, instrument, volume));
    tracks[track].instrument = instrument;
    
    emit trackUpdated(track);
}

void SimpleWavWriter::clearTrack(int track)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].events.clear();
        emit trackUpdated(track);
    }
}

void SimpleWavWriter::clearAllTracks()
{
    for (int i = 0; i < MAX_TRACKS; i++) {
        tracks[i].events.clear();
        emit trackUpdated(i);
    }
}

QString SimpleWavWriter::getTrackName(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].name;
    }
    return QString();
}

void SimpleWavWriter::setTrackName(int track, const QString &name)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].name = name;
        emit trackUpdated(track);
    }
}

int SimpleWavWriter::getEventCount(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].events.size();
    }
    return 0;
}

int SimpleWavWriter::getTrackInstrument(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].instrument;
    }
    return 1;
}

void SimpleWavWriter::setTrackInstrument(int track, int instrument)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].instrument = instrument;
        emit trackUpdated(track);
    }
}

double SimpleWavWriter::getTrackVolume(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].volume;
    }
    return 1.0;
}

void SimpleWavWriter::setTrackVolume(int track, double volume)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].volume = qBound(0.0, volume, 2.0);
        emit trackUpdated(track);
    }
}

bool SimpleWavWriter::isTrackMuted(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].muted;
    }
    return false;
}

void SimpleWavWriter::muteTrack(int track, bool muted)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].muted = muted;
        emit trackUpdated(track);
    }
}

bool SimpleWavWriter::isTrackSolo(int track) const
{
    if (track >= 0 && track < MAX_TRACKS) {
        return tracks[track].solo;
    }
    return false;
}

void SimpleWavWriter::soloTrack(int track, bool solo)
{
    if (track >= 0 && track < MAX_TRACKS) {
        tracks[track].solo = solo;
        emit trackUpdated(track);
    }
}

QByteArray SimpleWavWriter::getTrackAudioData(int track, bool mix)
{
    if (track < 0 || track >= MAX_TRACKS) return QByteArray();
    
    if (mix) {
        // Смешиваем все дорожки
        QVector<int> allTracks;
        for (int i = 0; i < MAX_TRACKS; i++) {
            allTracks.append(i);
        }
        return mixTracks(allTracks);
    }
    
    return mixTracks(QVector<int>{track});
}

QByteArray SimpleWavWriter::getAllTracksAudioData()
{
    // Собираем индексы всех дорожек, которые не заглушены и имеют данные
    QVector<int> activeTracks;
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (!tracks[i].muted && !tracks[i].events.isEmpty()) {
            activeTracks.append(i);
        }
    }
    
    if (activeTracks.isEmpty()) {
        return QByteArray();
    }
    
    return mixTracks(activeTracks);
}

bool SimpleWavWriter::loadWavToTrack(const QString &filename, int track)
{
    // Временно заглушка - в реальной реализации нужно парсить WAV файл
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return false;
    
    // Пропускаем заголовок WAV (44 байта)
    file.seek(44);
    QByteArray audioData = file.readAll();
    file.close();
    
    // Создаем событие для импортированного аудио
    if (!audioData.isEmpty()) {
        // Очищаем дорожку и добавляем специальное событие
        tracks[track].events.clear();
        // Здесь нужно добавить логику для импорта WAV
        emit trackUpdated(track);
        return true;
    }
    
    return false;
}

void SimpleWavWriter::copyTrack(int fromTrack, int toTrack)
{
    if (fromTrack < 0 || fromTrack >= MAX_TRACKS || 
        toTrack < 0 || toTrack >= MAX_TRACKS || fromTrack == toTrack) {
        return;
    }
    
    tracks[toTrack] = tracks[fromTrack];
    tracks[toTrack].name = QString("Копия %1").arg(tracks[fromTrack].name);
    emit trackUpdated(toTrack);
}

void SimpleWavWriter::mergeTrack(int fromTrack, int toTrack)
{
    if (fromTrack < 0 || fromTrack >= MAX_TRACKS || 
        toTrack < 0 || toTrack >= MAX_TRACKS || fromTrack == toTrack) {
        return;
    }
    
    tracks[toTrack].events.append(tracks[fromTrack].events);
    // Сортируем события по времени
    std::sort(tracks[toTrack].events.begin(), tracks[toTrack].events.end(),
              [](const NoteEvent &a, const NoteEvent &b) { return a.time < b.time; });
    
    emit trackUpdated(toTrack);
}

void SimpleWavWriter::setPlaybackSpeed(double speed)
{
    playbackSpeed = qBound(0.5, speed, 2.0);
}

void SimpleWavWriter::setBassVolumeMultiplier(double multiplier)
{
    bassVolumeMultiplier = qBound(1.0, multiplier, 4.0);
}

QByteArray SimpleWavWriter::generateNoteAudio(int note, int velocity, double duration, int instrument, double volume)
{
    double frequency = noteToFrequency(note);
    double amplitude = (velocity / 127.0) * volume;
    
    return generateInstrumentWave(instrument, frequency, duration, amplitude);
}

QByteArray SimpleWavWriter::generateSilence(double duration)
{
    int samples = static_cast<int>(duration * format.sampleRate());
    QByteArray silence(samples * sizeof(qint16), 0);
    return silence;
}

void SimpleWavWriter::writeWavHeader(QFile &file, quint32 dataSize)
{
    // RIFF header
    file.write("RIFF");
    file.write(intToBytes(36 + dataSize));
    file.write("WAVE");
    
    // fmt chunk
    file.write("fmt ");
    file.write(intToBytes(16)); // chunk size
    file.write(shortToBytes(1)); // audio format (PCM)
    file.write(shortToBytes(format.channelCount())); // channels
    file.write(intToBytes(format.sampleRate())); // sample rate
    file.write(intToBytes(format.sampleRate() * format.channelCount() * format.sampleSize() / 8)); // byte rate
    file.write(shortToBytes(format.channelCount() * format.sampleSize() / 8)); // block align
    file.write(shortToBytes(format.sampleSize())); // bits per sample
    
    // data chunk
    file.write("data");
    file.write(intToBytes(dataSize));
}

bool SimpleWavWriter::saveToFile(const QString &filename)
{
    // Получаем аудиоданные всех активных дорожек
    QByteArray audioData = getAllTracksAudioData();
    if (audioData.isEmpty()) {
        qDebug() << "Нет данных для сохранения - все дорожки пусты или заглушены";
        
        // Выводим информацию о дорожках для отладки
        for (int i = 0; i < MAX_TRACKS; i++) {
            if (!tracks[i].events.isEmpty()) {
                qDebug() << "Дорожка" << i << ":" << tracks[i].name 
                         << "инструмент:" << tracks[i].instrument
                         << "нот:" << tracks[i].events.size()
                         << "заглушена:" << tracks[i].muted;
            }
        }
        
        return false;
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Не удалось открыть файл для записи:" << filename;
        return false;
    }
    
    // Записываем WAV заголовок
    writeWavHeader(file, audioData.size());
    
    // Записываем аудиоданные
    qint64 bytesWritten = file.write(audioData);
    file.close();
    
    if (bytesWritten != audioData.size()) {
        qDebug() << "Ошибка записи файла: записано" << bytesWritten << "байт из" << audioData.size();
        return false;
    }
    
    // Выводим информацию о сохраненном файле
    double duration = audioData.size() / (2.0 * SAMPLE_RATE); // байты / (2 байта на сэмпл * частота)
    int activeTracks = 0;
    int totalNotes = 0;
    
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (!tracks[i].muted && !tracks[i].events.isEmpty()) {
            activeTracks++;
            totalNotes += tracks[i].events.size();
        }
    }
    
    qDebug() << "Файл успешно сохранен:";
    qDebug() << "  Имя файла:" << filename;
    qDebug() << "  Размер:" << audioData.size() << "байт";
    qDebug() << "  Частота дискретизации:" << SAMPLE_RATE << "Гц";
    qDebug() << "  Битрейт:" << (SAMPLE_RATE * 2 * 8) / 1000 << "кбит/с";
    qDebug() << "  Длительность:" << duration << "секунд";
    qDebug() << "  Активных дорожек:" << activeTracks;
    qDebug() << "  Всего нот:" << totalNotes;
    
    return true;
}

bool SimpleWavWriter::saveToMusicFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    
    // Заголовок файла
    out << "; Generated by Yamitracker\n";
    out << "; Date: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n\n";
    
    // Таблица инструментов
    out << "music_inst_table:\n";
    QMap<int, QString> instrumentNames = {
        {1, "CrandPno"}, {2, "BritePno"}, {3, "Harpsi"}, {6, "Harpsi2"},
        {28, "FngrBass"}, {33, "FngrBass2"}, {39, "FngrBass3"}, {40, "SlapBas"},
        {29, "Ovrdrive"}, {37, "Ovrdrive2"}, {38, "DstGtr"},
        {48, "String"}, {49, "SlowStr"}, {81, "LeadSx"},
        {95, "Bright"}, {96, "Bright2"}, {100, "Bright3"}
    };
    
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (tracks[i].events.isEmpty()) continue;
        
        int inst = tracks[i].instrument;
        QString instName = instrumentNames.value(inst, QString("Inst_%1").arg(inst));
        
        // Параметры инструмента для Pently
        out << "  INST " << instName << ",         ";
        
        // lv, rv, frq, smp, att, dec, sus, lvl
        QMap<int, QString> params = {
            {1, "11, 10,  3,  6, 31, 17,  0,  8"},  // Grand Piano
            {28, "12, 12,  1,  4, 31, 17, 15,  8"}, // Bass
            {48, "10,  6,  1,  2, 31, 23, 20,  2"}, // Strings
            {81, " 3,  9,  1,  0, 21, 21, 27,  4"}  // Sax
        };
        
        out << params.value(inst, "10, 10,  1,  1, 31, 17, 15,  8") << "\n";
    }
    
    // Паттерны для каждой дорожки
    out << "\npently_patterns:\n";
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (tracks[i].events.isEmpty()) continue;
        
        out << "  .addr PPDAT_track" << i << "\n";
    }
    
    // Данные паттернов
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (tracks[i].events.isEmpty()) continue;
        
        out << "\nPPDAT_track" << i << ":\n";
        
        // Сортируем события по времени
        std::sort(tracks[i].events.begin(), tracks[i].events.end(),
                  [](const NoteEvent &a, const NoteEvent &b) { return a.time < b.time; });
        
        double lastTime = 0;
        for (const NoteEvent &event : tracks[i].events) {
            // Вычисляем задержку
            double delay = event.time - lastTime;
            if (delay > 0) {
                QString delayCmd = durationToPentlyDuration(delay);
                if (!delayCmd.isEmpty()) {
                    out << "  .byt " << delayCmd << "\n";
                }
            }
            
            // Добавляем ноту
            QString noteCmd = noteToPentlyNote(event.note);
            QString durCmd = durationToPentlyDuration(event.duration);
            
            out << "  .byt " << noteCmd << "|" << durCmd << "\n";
            
            lastTime = event.time + event.duration;
        }
        
        out << "  .byt PATEND\n";
    }
    
    // Песни (последовательности паттернов)
    out << "\npently_songs:\n";
    out << ".addr  ";
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (tracks[i].events.isEmpty()) continue;
        out << "PSDAT_track" << i;
        if (i < MAX_TRACKS - 1) out << ", ";
    }
    out << "\n";
    
    // Определения песен
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (tracks[i].events.isEmpty()) continue;
        
        out << "\nPSDAT_track" << i << ":\n";
        out << "  setTempo 1000\n";
        out << "  playPat " << i << ", " << i << ", 23, " 
            << instrumentNames.value(tracks[i].instrument, "PLING_8") << "\n";
        out << "  waitRows 255\n";
        out << "  dalSegno\n";
    }
    
    file.close();
    return true;
}

QByteArray SimpleWavWriter::mixTracks(const QVector<int> &trackIndices)
{
    // Находим максимальную длительность среди выбранных дорожек
    double maxDuration = 0;
    for (int trackIndex : trackIndices) {
        if (trackIndex < 0 || trackIndex >= MAX_TRACKS) continue;
        
        const Track &track = tracks[trackIndex];
        for (const NoteEvent &event : track.events) {
            double endTime = event.time + event.duration;
            if (endTime > maxDuration) {
                maxDuration = endTime;
            }
        }
    }
    
    if (maxDuration <= 0) {
        return QByteArray();
    }
    
    // Создаем буфер для микширования
    int totalSamples = static_cast<int>(maxDuration * format.sampleRate());
    QVector<qint16> mixBuffer(totalSamples, 0);
    
    // Обрабатываем каждую дорожку
    for (int trackIndex : trackIndices) {
        if (trackIndex < 0 || trackIndex >= MAX_TRACKS) continue;
        
        const Track &track = tracks[trackIndex];
        double trackVolume = track.volume;
        
        // Обрабатываем каждое событие в дорожке
        for (const NoteEvent &event : track.events) {
            // Генерируем аудио для ноты
            QByteArray noteAudio = generateNoteAudio(
                event.note, 
                event.velocity, 
                event.duration, 
                event.instrument,
                event.volume * trackVolume
            );
            
            // Вычисляем начальный сэмпл
            int startSample = static_cast<int>(event.time * format.sampleRate());
            const qint16 *noteData = reinterpret_cast<const qint16*>(noteAudio.constData());
            int noteSamples = noteAudio.size() / sizeof(qint16);
            
            // Микшируем ноту в общий буфер
            for (int i = 0; i < noteSamples && startSample + i < totalSamples; i++) {
                // Прибавляем ноту к миксу
                qint32 mixed = mixBuffer[startSample + i] + noteData[i];
                
                // Ограничиваем значение для предотвращения клиппинга
                if (mixed > 32767) mixed = 32767;
                if (mixed < -32768) mixed = -32768;
                
                mixBuffer[startSample + i] = static_cast<qint16>(mixed);
            }
        }
    }
    
    // Конвертируем буфер в QByteArray
    QByteArray result;
    result.resize(totalSamples * sizeof(qint16));
    qint16 *resultData = reinterpret_cast<qint16*>(result.data());
    
    for (int i = 0; i < totalSamples; i++) {
        resultData[i] = mixBuffer[i];
    }
    
    return result;
}

double SimpleWavWriter::noteToFrequency(int note) const
{
    // Формула для преобразования ноты в частоту
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

QByteArray SimpleWavWriter::generateSineWave(double frequency, double duration, double amplitude)
{
    int samples = static_cast<int>(duration * format.sampleRate());
    QByteArray data(samples * sizeof(qint16), 0);
    qint16 *buffer = reinterpret_cast<qint16*>(data.data());
    
    double angularFreq = 2.0 * M_PI * frequency;
    double maxAmplitude = 32767.0 * amplitude;
    
    for (int i = 0; i < samples; i++) {
        double t = i / static_cast<double>(format.sampleRate());
        double value = sin(angularFreq * t);
        buffer[i] = static_cast<qint16>(value * maxAmplitude);
    }
    
    return data;
}

QByteArray SimpleWavWriter::generateSquareWave(double frequency, double duration, double amplitude)
{
    int samples = static_cast<int>(duration * format.sampleRate());
    QByteArray data(samples * sizeof(qint16), 0);
    qint16 *buffer = reinterpret_cast<qint16*>(data.data());
    
    double period = 1.0 / frequency;
    double maxAmplitude = 32767.0 * amplitude;
    
    for (int i = 0; i < samples; i++) {
        double t = i / static_cast<double>(format.sampleRate());
        double phase = fmod(t, period) / period;
        buffer[i] = static_cast<qint16>((phase < 0.5 ? maxAmplitude : -maxAmplitude));
    }
    
    return data;
}

QByteArray SimpleWavWriter::generateSawtoothWave(double frequency, double duration, double amplitude)
{
    int samples = static_cast<int>(duration * format.sampleRate());
    QByteArray data(samples * sizeof(qint16), 0);
    qint16 *buffer = reinterpret_cast<qint16*>(data.data());
    
    double period = 1.0 / frequency;
    double maxAmplitude = 32767.0 * amplitude;
    
    for (int i = 0; i < samples; i++) {
        double t = i / static_cast<double>(format.sampleRate());
        double phase = fmod(t, period) / period;
        buffer[i] = static_cast<qint16>((2.0 * phase - 1.0) * maxAmplitude);
    }
    
    return data;
}

QByteArray SimpleWavWriter::generateTriangleWave(double frequency, double duration, double amplitude)
{
    int samples = static_cast<int>(duration * format.sampleRate());
    QByteArray data(samples * sizeof(qint16), 0);
    qint16 *buffer = reinterpret_cast<qint16*>(data.data());
    
    double period = 1.0 / frequency;
    double maxAmplitude = 32767.0 * amplitude;
    
    for (int i = 0; i < samples; i++) {
        double t = i / static_cast<double>(format.sampleRate());
        double phase = fmod(t, period) / period;
        double value;
        
        if (phase < 0.25) {
            value = 4.0 * phase;
        } else if (phase < 0.75) {
            value = 2.0 - 4.0 * phase;
        } else {
            value = 4.0 * phase - 4.0;
        }
        
        buffer[i] = static_cast<qint16>(value * maxAmplitude);
    }
    
    return data;
}

QByteArray SimpleWavWriter::generateInstrumentWave(int instrument, double frequency, double duration, double amplitude)
{
    // Выбираем форму волны в зависимости от инструмента
    static QMap<int, QString> waveTypes = {
        {1, "sine"}, {2, "sine"},     // Piano
        {3, "square"}, {6, "square"}, // Harpsichord
        {28, "sawtooth"}, {33, "sawtooth"}, {39, "sawtooth"}, {40, "square"}, // Bass
        {29, "square"}, {37, "square"}, {38, "square"}, // Guitar
        {48, "sine"}, {49, "sine"},    // Strings
        {81, "sine"},                  // Sax
        {95, "triangle"}, {96, "triangle"}, {100, "triangle"} // Bright
    };
    
    QString waveType = waveTypes.value(instrument, "sine");
    
    if (waveType == "square") {
        return generateSquareWave(frequency, duration, amplitude);
    } else if (waveType == "sawtooth") {
        return generateSawtoothWave(frequency, duration, amplitude);
    } else if (waveType == "triangle") {
        return generateTriangleWave(frequency, duration, amplitude);
    } else {
        return generateSineWave(frequency, duration, amplitude);
    }
}

QByteArray SimpleWavWriter::intToBytes(qint32 value)
{
    QByteArray bytes(4, 0);
    bytes[0] = static_cast<char>(value & 0xFF);
    bytes[1] = static_cast<char>((value >> 8) & 0xFF);
    bytes[2] = static_cast<char>((value >> 16) & 0xFF);
    bytes[3] = static_cast<char>((value >> 24) & 0xFF);
    return bytes;
}

QByteArray SimpleWavWriter::shortToBytes(qint16 value)
{
    QByteArray bytes(2, 0);
    bytes[0] = static_cast<char>(value & 0xFF);
    bytes[1] = static_cast<char>((value >> 8) & 0xFF);
    return bytes;
}

QString SimpleWavWriter::noteToPentlyNote(int note) const
{
    static QMap<int, QString> noteMap = {
        {60, "N_C"}, {61, "N_CS"}, {62, "N_D"}, {63, "N_DS"},
        {64, "N_E"}, {65, "N_F"}, {66, "N_FS"}, {67, "N_G"},
        {68, "N_GS"}, {69, "N_A"}, {70, "N_AS"}, {71, "N_B"},
        {72, "N_CH"}, {73, "N_CSH"}, {74, "N_DH"}, {75, "N_DSH"},
        {76, "N_EH"}, {77, "N_FH"}, {78, "N_FSH"}, {79, "N_GH"},
        {80, "N_GSH"}, {81, "N_AH"}, {82, "N_ASH"}, {83, "N_BH"}
    };
    
    // Приводим ноты к октаве
    int baseNote = note % 12;
    int octave = note / 12 - 1;
    
    QString baseName;
    switch (baseNote) {
        case 0: baseName = "N_C"; break;
        case 1: baseName = "N_CS"; break;
        case 2: baseName = "N_D"; break;
        case 3: baseName = "N_DS"; break;
        case 4: baseName = "N_E"; break;
        case 5: baseName = "N_F"; break;
        case 6: baseName = "N_FS"; break;
        case 7: baseName = "N_G"; break;
        case 8: baseName = "N_GS"; break;
        case 9: baseName = "N_A"; break;
        case 10: baseName = "N_AS"; break;
        case 11: baseName = "N_B"; break;
    }
    
    // Добавляем суффикс октавы
    if (octave == 1) {
        baseName += "|OCTAVE_1";
    } else if (octave == 2) {
        baseName += "H";
    } else if (octave == 3) {
        baseName += "H|OCTAVE_1";
    }
    
    return noteMap.value(note, baseName);
}

QString SimpleWavWriter::durationToPentlyDuration(double duration) const
{
    // Конвертируем длительность в тики Pently
    // Базовый темп: 1 секунда = 1000 тиков
    int ticks = static_cast<int>(duration * 1000);
    
    if (ticks >= 2000) return "D_2";
    if (ticks >= 1000) return "D_1";
    if (ticks >= 500) return "D_D2";
    if (ticks >= 250) return "D_D4";
    if (ticks >= 125) return "D_D8";
    if (ticks >= 62) return "D_D16";
    if (ticks >= 31) return "D_2";
    if (ticks >= 16) return "D_4";
    if (ticks >= 8) return "D_8";
    if (ticks >= 4) return "D_16";
    if (ticks >= 2) return "D_32";
    
    return "D_32";
}

QString SimpleWavWriter::instrumentToPentlyInstrument(int instrument) const
{
    static QMap<int, QString> instMap = {
        {1, "PLING_8"}, {2, "PLING_4"}, {3, "PLING_2"}, {6, "PLING_2"},
        {28, "BASSGUITAR"}, {33, "BASSGUITAR"}, {39, "BASSGUITAR"}, {40, "BASSGUITAR"},
        {29, "XYLSHORT"}, {37, "XYLSHORT"}, {38, "XYLSHORT"},
        {48, "STRING"}, {49, "SLOWSTR"}, {81, "HORNBLAT"},
        {95, "BRIGHT"}, {96, "BRIGHT"}, {100, "BRIGHT"}
    };
    
    return instMap.value(instrument, "PLING_8");
}