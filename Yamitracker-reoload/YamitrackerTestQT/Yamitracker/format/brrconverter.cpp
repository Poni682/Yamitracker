#include "brrconverter.h"
#include <QDebug>

BrrConverter::BrrConverter(QObject *parent) : QObject(parent)
{
}

bool BrrConverter::convertWavToBrr(const QString &wavFilePath, const QString &brrFilePath)
{
    QFile wavFile(wavFilePath);
    if (!wavFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open WAV file:" << wavFilePath;
        return false;
    }

    int sampleRate, channels, bitsPerSample, dataSize;
    if (!decodeWavHeader(wavFile, sampleRate, channels, bitsPerSample, dataSize)) {
        qDebug() << "Invalid WAV file format";
        wavFile.close();
        return false;
    }

    // Читаем аудиоданные
    QByteArray audioData = wavFile.read(dataSize);
    wavFile.close();

    // Конвертируем в 16-bit моно
    QVector<int16_t> samples;
    int bytesPerSample = bitsPerSample / 8;
    int totalSamples = dataSize / (bytesPerSample * channels);

    for (int i = 0; i < totalSamples; ++i) {
        int32_t sample = 0;

        for (int ch = 0; ch < channels; ++ch) {
            int32_t channelSample = 0;
            int offset = (i * channels + ch) * bytesPerSample;

            if (bitsPerSample == 8) {
                channelSample = (int8_t)audioData[offset];
                channelSample = (channelSample - 128) * 256;
            } else if (bitsPerSample == 16) {
                channelSample = (int16_t)((audioData[offset + 1] << 8) | audioData[offset]);
            } else if (bitsPerSample == 24) {
                channelSample = (int32_t)((audioData[offset + 2] << 16) |
                                           (audioData[offset + 1] << 8) |
                                           audioData[offset]);
                if (channelSample & 0x800000) channelSample |= 0xFF000000;
                channelSample >>= 8;
            } else if (bitsPerSample == 32) {
                channelSample = (int32_t)((audioData[offset + 3] << 24) |
                                           (audioData[offset + 2] << 16) |
                                           (audioData[offset + 1] << 8) |
                                           audioData[offset]);
                channelSample >>= 16;
            }

            sample += channelSample;
        }

        // Микшируем каналы
        sample /= channels;
        samples.append(clamp16(sample));
    }

    // Понижаем частоту дискретизации для SNES (макс 32kHz -> 16kHz)
    if (sampleRate > 16000) {
        int ratio = sampleRate / 16000;
        QVector<int16_t> downsampled;
        for (int i = 0; i < samples.size(); i += ratio) {
            downsampled.append(samples[i]);
        }
        samples = downsampled;
    } else if (sampleRate < 16000) {
        // Если частота ниже, повышаем (простое повторение)
        int ratio = 16000 / sampleRate;
        QVector<int16_t> upsampled;
        for (int i = 0; i < samples.size(); ++i) {
            for (int j = 0; j < ratio; ++j) {
                upsampled.append(samples[i]);
            }
        }
        samples = upsampled;
    }

    // Открываем BRR файл для записи
    QFile brrFile(brrFilePath);
    if (!brrFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open BRR file for writing:" << brrFilePath;
        return false;
    }

    ADPCMState state;
    QVector<unsigned char> blockSamples;

    // Конвертируем в BRR блоками по 16 сэмплов
    for (int i = 0; i < samples.size(); ++i) {
        int encoded = encodeADPCMNibble(samples[i], state);
        blockSamples.append(encoded);

        if (blockSamples.size() == 32) { // 32 ниббла = 16 сэмплов
            writeBrrBlock(brrFile, blockSamples, false);
            blockSamples.clear();
        }
    }

    // Записываем последний неполный блок
    if (!blockSamples.isEmpty()) {
        // Дополняем до 32 нибблов
        while (blockSamples.size() < 32) {
            blockSamples.append(0);
        }
        writeBrrBlock(brrFile, blockSamples, true); // Последний блок с флагом loop
    } else if (samples.size() > 0) {
        // Если был полный последний блок, помечаем его как loop
        brrFile.seek(brrFile.size() - BRR_BLOCK_SIZE);
        unsigned char header = 0x03; // Фильтр 0, loop flag = 1
        brrFile.write((char*)&header, 1);
    }

    brrFile.close();
    qDebug() << "BRR file created:" << brrFilePath << "samples:" << samples.size() << "sample rate:" << sampleRate;
    return true;
}

bool BrrConverter::convertMidiToBrr(const QString &midiFilePath, const QString &brrFilePath)
{
    qDebug() << "MIDI to BRR conversion requires MIDI synthesizer";
    qDebug() << "Please convert MIDI to WAV first, then use WAV to BRR";

    return false;
}

bool BrrConverter::decodeWavHeader(QFile &file, int &sampleRate, int &channels, int &bitsPerSample, int &dataSize)
{
    char buffer[44];
    if (file.read(buffer, 44) != 44) {
        return false;
    }

    // Проверяем RIFF заголовок
    if (buffer[0] != 'R' || buffer[1] != 'I' || buffer[2] != 'F' || buffer[3] != 'F') {
        return false;
    }

    // Проверяем WAVE формат
    if (buffer[8] != 'W' || buffer[9] != 'A' || buffer[10] != 'V' || buffer[11] != 'E') {
        return false;
    }

    // Проверяем fmt chunk
    if (buffer[12] != 'f' || buffer[13] != 'm' || buffer[14] != 't' || buffer[15] != ' ') {
        return false;
    }

    // Читаем параметры
    int fmtSize = *(int*)(buffer + 16);
    if (fmtSize < 16) return false;

    int audioFormat = *(short*)(buffer + 20);
    if (audioFormat != 1) return false; // Только PCM

    channels = *(short*)(buffer + 22);
    sampleRate = *(int*)(buffer + 24);
    bitsPerSample = *(short*)(buffer + 34);

    // Ищем data chunk
    int offset = 36;
    while (offset < file.size()) {
        file.seek(offset);
        char chunkId[4];
        if (file.read(chunkId, 4) != 4) break;

        int chunkSize;
        if (file.read((char*)&chunkSize, 4) != 4) break;

        if (chunkId[0] == 'd' && chunkId[1] == 'a' && chunkId[2] == 't' && chunkId[3] == 'a') {
            dataSize = chunkSize;
            file.seek(offset + 8);
            return true;
        }

        offset += 8 + chunkSize;
    }

    return false;
}

int16_t BrrConverter::clamp16(int sample)
{
    if (sample > 32767) return 32767;
    if (sample < -32768) return -32768;
    return (int16_t)sample;
}

int BrrConverter::encodeADPCMNibble(int sample, ADPCMState &state)
{
    int diff = sample - state.prev_sample;
    int step = step_table[state.prev_index];

    // Квантование
    int nibble = 0;
    int predicted = state.prev_sample;

    // Применяем фильтр (простейший - без фильтра)
    int filtered = predicted;

    diff = sample - filtered;

    // Находим наилучший nibble
    int best_nibble = 0;
    int best_error = INT_MAX;

    for (int n = 0; n < 16; ++n) {
        int step_index = state.prev_index + ((n & 7) * 2 - 7);
        if (step_index < 0) step_index = 0;
        if (step_index > 88) step_index = 88;

        int current_step = step_table[step_index];
        int delta = (n & 8) ? -current_step : current_step;

        int current_sample = filtered + delta;
        int error = abs(sample - current_sample);

        if (error < best_error) {
            best_error = error;
            best_nibble = n;
        }
    }

    // Декодируем выбранный nibble для обновления состояния
    int step_index = state.prev_index + ((best_nibble & 7) * 2 - 7);
    if (step_index < 0) step_index = 0;
    if (step_index > 88) step_index = 88;

    int current_step = step_table[step_index];
    int delta = (best_nibble & 8) ? -current_step : current_step;

    state.prev_sample = filtered + delta;
    state.prev_index = step_index;

    // Ограничиваем индекс
    if (state.prev_index < 0) state.prev_index = 0;
    if (state.prev_index > 88) state.prev_index = 88;

    // Ограничиваем сэмпл
    state.prev_sample = clamp16(state.prev_sample);

    return best_nibble;
}

void BrrConverter::writeBrrBlock(QFile &file, const QVector<unsigned char> &samples, bool loop)
{
    if (samples.size() != 32) {
        qDebug() << "Invalid sample count for BRR block:" << samples.size();
        return;
    }

    unsigned char header = 0x00;
    if (loop) {
        header = 0x03; // Фильтр 0 + loop
    }

    file.write((char*)&header, 1);

    // Упаковываем 32 ниббла в 8 байт
    for (int i = 0; i < 16; ++i) {
        unsigned char byte = (samples[i * 2] << 4) | (samples[i * 2 + 1] & 0x0F);
        file.write((char*)&byte, 1);
    }
}

int BrrConverter::decodeADPCMNibble(int nibble, ADPCMState &state)
{
    int step = step_table[state.prev_index];
    int delta = step >> 3;

    if (nibble & 1) delta += step >> 2;
    if (nibble & 2) delta += step >> 1;
    if (nibble & 4) delta += step;

    if (nibble & 8) delta = -delta;

    // Применяем фильтр (простейший)
    int sample = state.prev_sample + delta;

    // Ограничиваем
    sample = clamp16(sample);

    // Обновляем индекс
    state.prev_index += ((nibble & 7) * 2 - 7);
    if (state.prev_index < 0) state.prev_index = 0;
    if (state.prev_index > 88) state.prev_index = 88;

    state.prev_sample = sample;

    return sample;
}

QVector<int16_t> BrrConverter::decodeBrrFile(const QString &brrFilePath)
{
    QVector<int16_t> samples;
    QFile file(brrFilePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open BRR file for reading:" << brrFilePath;
        return samples;
    }

    ADPCMState state;

    while (!file.atEnd()) {
        unsigned char header;
        if (file.read((char*)&header, 1) != 1) break;

        unsigned char data[8];
        if (file.read((char*)data, 8) != 8) break;

        // Извлекаем нибблы
        for (int i = 0; i < 8; ++i) {
            unsigned char byte = data[i];
            int nibble1 = (byte >> 4) & 0x0F;
            int nibble2 = byte & 0x0F;

            int sample1 = decodeADPCMNibble(nibble1, state);
            int sample2 = decodeADPCMNibble(nibble2, state);

            samples.append(sample1);
            samples.append(sample2);
        }

        // Проверяем конец
        if (header & 0x01) {
            break; // Конец сэмпла
        }
    }

    file.close();
    return samples;
}
