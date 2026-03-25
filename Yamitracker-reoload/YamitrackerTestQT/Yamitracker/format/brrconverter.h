#ifndef BRRCONVERTER_H
#define BRRCONVERTER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QFile>
#include <cmath>

class BrrConverter : public QObject
{
    Q_OBJECT

public:
    BrrConverter(QObject *parent = nullptr);

    bool convertWavToBrr(const QString &wavFilePath, const QString &brrFilePath);
    bool convertMidiToBrr(const QString &midiFilePath, const QString &brrFilePath);

private:
    struct ADPCMState {
        int prev_sample;
        int prev_index;

        ADPCMState() : prev_sample(0), prev_index(0) {}
    };

    struct BRRBlock {
        unsigned char header;
        unsigned char data[8];
    };

    bool decodeWavHeader(QFile &file, int &sampleRate, int &channels, int &bitsPerSample, int &dataSize);
    int16_t clamp16(int sample);
    int encodeADPCMNibble(int sample, ADPCMState &state);
    void writeBrrBlock(QFile &file, const QVector<unsigned char> &samples, bool loop = false);
    int decodeADPCMNibble(int nibble, ADPCMState &state);
    QVector<int16_t> decodeBrrFile(const QString &brrFilePath);

    static const int BRR_BLOCK_SIZE = 9;
    static const int SAMPLES_PER_BLOCK = 16;
    static const int FILTER_TYPES = 4;

    const int step_table[89] = {
        7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
        50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230,
        253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963,
        1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
        3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487,
        12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
    };
};

#endif
