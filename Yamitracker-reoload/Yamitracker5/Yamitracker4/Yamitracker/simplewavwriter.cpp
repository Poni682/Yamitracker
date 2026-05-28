#include "simplewavwriter.h"
#include <QDebug>
#include <QDateTime>
#include <cmath>
#include <algorithm>
#include <QMap>

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

// НОВЫЙ МЕТОД: Конвертирует время в секундах в тики Pently (базовый темп 1000)
int SimpleWavWriter::secondsToPentlyTicks(double seconds) const
{
    // Базовый темп: 1000 тиков в секунде
    return static_cast<int>(seconds * 1000.0);
}

// НОВЫЙ ИСПРАВЛЕННЫЙ МЕТОД: Конвертирует тики в строку задержки (макс. D_D8 = 255 тиков)
QString SimpleWavWriter::ticksToDelayString(int ticks) const
{
    if (ticks <= 0) return QString();
    
    // Ограничиваем максимальное значение 255 тиков (D_D8)
    if (ticks > 255) {
        ticks = 255;
    }
    
    // Определяем наилучшее представление задержки
    // В Pently формат поддерживает только определенные значения задержек
    if (ticks >= 240) return QString("D_D8");
    if (ticks >= 120) return QString("D_D4");
    if (ticks >= 60) return QString("D_D2");
    if (ticks >= 30) return QString("D_1");
    if (ticks >= 15) return QString("D_2");
    if (ticks >= 8) return QString("D_4");
    if (ticks >= 4) return QString("D_8");
    if (ticks >= 2) return QString("D_16");
    
    return QString("D_32");
}

// НОВЫЙ МЕТОД: Разбивает большую задержку на несколько маленьких
QVector<QString> SimpleWavWriter::splitLongDelay(int ticks) const
{
    QVector<QString> delays;
    
    while (ticks > 255) {
        delays.append("D_D8");
        ticks -= 255;
    }
    
    if (ticks > 0) {
        QString delay = ticksToDelayString(ticks);
        if (!delay.isEmpty()) {
            delays.append(delay);
        }
    }
    
    return delays;
}

// НОВЫЙ МЕТОД: Создает паттерн из событий дорожки с учетом ограничений Pently
QString SimpleWavWriter::generatePattern(int trackIndex) const
{
    if (trackIndex < 0 || trackIndex >= tracks.size()) return QString();
    
    const Track& track = tracks[trackIndex];
    if (track.events.isEmpty()) return QString();
    
    QString pattern;
    
    // Сортируем события по времени
    QVector<NoteEvent> sortedEvents = track.events;
    std::sort(sortedEvents.begin(), sortedEvents.end(),
              [](const NoteEvent &a, const NoteEvent &b) { return a.time < b.time; });
    
    double lastTime = 0;
    for (const NoteEvent &event : sortedEvents) {
        // Вычисляем задержку от предыдущего события
        double delay = event.time - lastTime;
        if (delay > 0) {
            int delayTicks = secondsToPentlyTicks(delay);
            
            // Разбиваем большую задержку на несколько маленьких
            QVector<QString> delayCommands = splitLongDelay(delayTicks);
            for (const QString &delayCmd : delayCommands) {
                pattern += QString("  .byt %1\n").arg(delayCmd);
            }
        }
        
        // Добавляем ноту с ее длительностью
        QString noteStr = noteToPentlyNote(event.note);
        int durationTicks = secondsToPentlyTicks(event.duration);
        QString durationStr = ticksToDelayString(durationTicks);
        
        if (!noteStr.isEmpty() && !durationStr.isEmpty()) {
            pattern += QString("  .byt %1|%2\n").arg(noteStr).arg(durationStr);
        }
        
        lastTime = event.time + event.duration;
    }
    
    return pattern;
}

// НОВЫЙ МЕТОД: Вычисляет длительность композиции в тактах (с учетом ограничения 255)
int SimpleWavWriter::calculateTotalRows() const
{
    double maxDuration = 0;
    
    for (const Track& track : tracks) {
        for (const NoteEvent &event : track.events) {
            double endTime = event.time + event.duration;
            if (endTime > maxDuration) {
                maxDuration = endTime;
            }
        }
    }
    
    int totalTicks = secondsToPentlyTicks(maxDuration);
    
    // Ограничиваем максимальное количество тиков для waitRows
    if (totalTicks > 255) {
        totalTicks = 255;
        qDebug() << "ВНИМАНИЕ: Длительность композиции превышает 255 тиков. Ограничено до 255.";
    }
    
    return totalTicks;
}

// ИСПРАВЛЕННЫЙ МЕТОД: Сохранение в музыкальный файл Pently с учетом ограничений
bool SimpleWavWriter::saveToMusicFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    
    // 1. Проверяем, есть ли данные для сохранения
    int trackCount = 0;
    QVector<int> trackIndices;
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (!tracks[i].events.isEmpty()) {
            trackCount++;
            trackIndices.append(i);
        }
    }
    
    if (trackCount == 0) {
        out << "; Нет данных для сохранения\n";
        file.close();
        return false;
    }
    
    qDebug() << "Сохранение музыки: найдено" << trackCount << "дорожек с данными";
    
    // 2. Заголовок файла
    out << "; Generated by Yamitracker\n";
    out << "; Date: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "; Tracks: " << trackCount << "\n\n";
    
    // 3. Таблица инструментов
    out << "music_inst_table:\n";
    
    // Карта преобразования инструментов GM в Pently
    QMap<int, QString> pentlyInstruments = {
        {1, "PLING_8"},   // Grand Piano
        {2, "PLING_4"},   // Bright Piano
        {3, "PLING_2"},   // Harpsichord
        {6, "PLING_2"},   // Harpsichord 2
        
        // Bass instruments
        {28, "BASSGUITAR"},
        {33, "BASSGUITAR"},
        {39, "BASSGUITAR"},
        {40, "BASSGUITAR"},
        
        // Guitar/Overdrive
        {29, "XYLSHORT"},
        {37, "XYLSHORT"},
        {38, "XYLSHORT"},
        
        // Strings
        {48, "XYLMED"},
        {49, "XYLMED"},
        
        // Sax/Horn
        {81, "HORNBLAT"},
        
        // Bright
        {95, "XYLSHORT"},
        {96, "XYLSHORT"},
        {100, "XYLSHORT"}
    };
    
    // Карта параметров инструментов Pently (lv, rv, frq, smp, att, dec, sus, lvl)
    QMap<QString, QString> instrumentParams = {
        {"PLING_8",    " 3, 10,  1,  0, 31, 19, 15,  2"},
        {"PLING_4",    " 7,  7,  1,  1, 31, 19, 15,  2"},
        {"PLING_2",    " 9,  6,  1,  2, 31, 17, 15,  2"},
        {"BASSGUITAR", "12, 12,  1,  4, 31, 17, 15,  8"},
        {"BASSCLAR",   "12, 12,  1,  2, 21, 21, 27,  4"},
        {"HORNBLAT",   " 3,  9,  1,  0, 21, 21, 27,  4"},
        {"XYLSHORT",   "10,  6,  1,  2, 31, 23, 20,  2"},
        {"XYLMED",     "10,  6,  1,  2, 31, 19, 17,  2"},
        {"STRING",     "10,  6,  1,  2, 31, 23, 20,  2"}
    };
    
    // Генерируем уникальные инструменты для каждой дорожки
    QMap<int, QString> trackInstruments;
    for (int trackIndex : trackIndices) {
        int gmInstrument = tracks[trackIndex].instrument;
        QString pentlyInst = pentlyInstruments.value(gmInstrument, "PLING_8");
        trackInstruments[trackIndex] = pentlyInst;
        
        QString params = instrumentParams.value(pentlyInst, "10, 10,  1,  1, 31, 17, 15,  8");
        out << "  INST " << pentlyInst << "," << params << "\n";
    }
    
    out << "\n";
    
    // 4. Таблица паттернов
    out << "pently_patterns:\n";
    for (int trackIndex : trackIndices) {
        out << "  .addr PPDAT_track" << trackIndex << "\n";
    }
    
    // 5. Данные паттернов
    for (int trackIndex : trackIndices) {
        out << "\nPPDAT_track" << trackIndex << ":\n";
        QString patternData = generatePattern(trackIndex);
        if (patternData.isEmpty()) {
            out << "  .byt PATEND\n";
        } else {
            out << patternData;
            out << "  .byt PATEND\n";
        }
    }
    
    // 6. Таблица песен (теперь только ОДНА песня)
    out << "\npently_songs:\n";
    out << "  .addr  PSDAT_main_song\n\n";
    
    // 7. Создаем ОДНУ главную песню, которая управляет всеми дорожками
    out << "PSDAT_main_song:\n";
    
    // Вычисляем длительность композиции с учетом ограничений Pently
    int totalRows = calculateTotalRows();
    
    qDebug() << "Общая длительность в тиках (ограничено):" << totalRows;
    
    // Устанавливаем темп
    out << "  setTempo 1000\n";
    
    // Запускаем все паттерны одновременно
    int channel = 0;
    for (int trackIndex : trackIndices) {
        QString instName = trackInstruments[trackIndex];
        out << QString("  playPat %1, %2, 23, %3\n")
                   .arg(channel).arg(trackIndex).arg(instName);
        channel++;
    }
    
    // Ждем длительность композиции (ограничено 255)
    out << QString("  waitRows %1\n").arg(totalRows);
    
    // Останавливаем все паттерны
    for (int i = 0; i < channel; i++) {
        out << QString("  stopPat %1\n").arg(i);
    }
    
    // Зацикливаем
    out << "  dalSegno\n";
    
    // 8. Добавляем информацию об ограничениях
    out << "\n; ОГРАНИЧЕНИЯ PENTLY:\n";
    out << "; - Максимальная длительность задержки: D_D8 (255 тиков)\n";
    out << "; - Максимальный waitRows: 255 тиков\n";
    out << "; - Длительные паузы разбиваются на несколько D_D8\n";
    
    file.close();
    
    qDebug() << "Музыкальный файл сохранен:" << filename;
    qDebug() << "Треков:" << trackCount;
    qDebug() << "Длительность (тиков):" << totalRows;
    qDebug() << "ПРЕДУПРЕЖДЕНИЕ: Композиция адаптирована под ограничения Pently";
    
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
            
            // Вычисляем начальный сэмпл (на основе абсолютного времени)
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
    // Преобразуем MIDI номер ноты в формат Pently
    // MIDI: 60 = C4, 61 = C#4, 62 = D4, и т.д.
    
    int octave = note / 12;
    int noteInOctave = note % 12;
    
    // Базовые названия нот
    QString noteName;
    switch (noteInOctave) {
        case 0: noteName = "N_C"; break;
        case 1: noteName = "N_CS"; break;
        case 2: noteName = "N_D"; break;
        case 3: noteName = "N_DS"; break;
        case 4: noteName = "N_E"; break;
        case 5: noteName = "N_F"; break;
        case 6: noteName = "N_FS"; break;
        case 7: noteName = "N_G"; break;
        case 8: noteName = "N_GS"; break;
        case 9: noteName = "N_A"; break;
        case 10: noteName = "N_AS"; break;
        case 11: noteName = "N_B"; break;
    }
    
    // Добавляем октаву
    if (octave == 4) {
        // Средняя октава (C4-B4) - без суффикса в Pently
        return noteName;
    } else if (octave == 5) {
        // Высокая октава (C5-B5) - добавляем H
        return noteName + "H";
    } else if (octave == 3) {
        // Низкая октава (C3-B3) - добавляем суффикс октавы
        return noteName + "|OCTAVE_1";
    } else if (octave == 2) {
        // Очень низкая октава (C2-B2) - добавляем суффикс
        return noteName + "L";
    } else if (octave == 6) {
        // Очень высокая октава (C6+) - добавляем суффикс
        return noteName + "H|OCTAVE_1";
    }
    
    // По умолчанию возвращаем базовое имя
    return noteName;
}

QString SimpleWavWriter::durationToPentlyDuration(double duration) const
{
    // Конвертируем длительность в секундах в формат Pently
    int ticks = secondsToPentlyTicks(duration);
    return ticksToDelayString(ticks);
}

QString SimpleWavWriter::instrumentToPentlyInstrument(int instrument) const
{
    static QMap<int, QString> instMap = {
        {1, "PLING_8"}, {2, "PLING_4"}, {3, "PLING_2"}, {6, "PLING_2"},
        {28, "BASSGUITAR"}, {33, "BASSGUITAR"}, {39, "BASSGUITAR"}, {40, "BASSGUITAR"},
        {29, "XYLSHORT"}, {37, "XYLSHORT"}, {38, "XYLSHORT"},
        {48, "XYLMED"}, {49, "XYLMED"}, {81, "HORNBLAT"},
        {95, "XYLSHORT"}, {96, "XYLSHORT"}, {100, "XYLSHORT"}
    };
    
    return instMap.value(instrument, "PLING_8");
}