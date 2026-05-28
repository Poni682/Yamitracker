#ifndef MIDIEVENTLIST_H
#define MIDIEVENTLIST_H
#include <QString>
#include <QMap>


QMap<int, QString> noteNames = {
    {0, "C-1"}, {1, "C#-1"}, {2, "D-1"}, {3, "D#-1"}, {4, "E-1"}, {5, "F-1"},
    {6, "F#-1"}, {7, "G-1"}, {8, "G#-1"}, {9, "A-1"}, {10, "A#-1"}, {11, "B-1"},
    {12, "C0"}, {13, "C#0"}, {14, "D0"}, {15, "D#0"}, {16, "E0"}, {17, "F0"},
    {18, "F#0"}, {19, "G0"}, {20, "G#0"}, {21, "A0"}, {22, "A#0"}, {23, "B0"},
    {24, "C1"}, {25, "C#1"}, {26, "D1"}, {27, "D#1"}, {28, "E1"}, {29, "F1"},
    {30, "F#1"}, {31, "G1"}, {32, "G#1"}, {33, "A1"}, {34, "A#1"}, {35, "B1"},
    {36, "C2"}, {37, "C#2"}, {38, "D2"}, {39, "D#2"}, {40, "E2"}, {41, "F2"},
    {42, "F#2"}, {43, "G2"}, {44, "G#2"}, {45, "A2"}, {46, "A#2"}, {47, "B2"},
    {48, "C3"}, {49, "C#3"}, {50, "D3"}, {51, "D#3"}, {52, "E3"}, {53, "F3"},
    {54, "F#3"}, {55, "G3"}, {56, "G#3"}, {57, "A3"}, {58, "A#3"}, {59, "B3"},
    {60, "C4"}, {61, "C#4"}, {62, "D4"}, {63, "D#4"}, {64, "E4"}, {65, "F4"},
    {66, "F#4"}, {67, "G4"}, {68, "G#4"}, {69, "A4"}, {70, "A#4"}, {71, "B4"},
    {72, "C5"}, {73, "C#5"}, {74, "D5"}, {75, "D#5"}, {76, "E5"}, {77, "F5"},
    {78, "F#5"}, {79, "G5"}, {80, "G#5"}, {81, "A5"}, {82, "A#5"}, {83, "B5"},
    {84, "C6"}
};


// MIDI Message Types
enum class MidiStatus : unsigned char {
    // Канальные сообщения (Channel Voice Messages)
    NoteOff          = 0x80,  // 1000nnnn 0kkkkkkk 0vvvvvvv - отпускание клавиши
    NoteOn           = 0x90,  // 1001nnnn 0kkkkkkk 0vvvvvvv - нажатие клавиши
    PolyAftertouch   = 0xA0,  // 1010nnnn 0kkkkkkk 0vvvvvvv - полифоническое послекасание
    ControlChange    = 0xB0,  // 1011nnnn 0ccccccc 0vvvvvvv - изменение контроллера
    ProgramChange    = 0xC0,  // 1100nnnn 0ppppppp - смена программы/инструмента
    ChannelAftertouch = 0xD0, // 1101nnnn 0vvvvvvv - канальное послекасание
    PitchBend        = 0xE0,  // 1110nnnn 0lllllll 0mmmmmmm - колесо высоты тона

    // Системные сообщения (System Messages)
    SysEx            = 0xF0,  // 11110000 - начало System Exclusive
    TimeCodeQuarter  = 0xF1,  // 11110001 - MIDI Time Code Quarter Frame
    SongPosition     = 0xF2,  // 11110010 - Song Position Pointer
    SongSelect       = 0xF3,  // 11110011 - Song Select
    TuneRequest      = 0xF6,  // 11110110 - Tune Request
    SysExEnd         = 0xF7,  // 11110111 - конец System Exclusive

    // Системные реального времени (System Real-Time)
    TimingClock      = 0xF8,  // 11111000 - Timing Clock
    Start            = 0xFA,  // 11111010 - Start
    Continue         = 0xFB,  // 11111011 - Continue
    Stop             = 0xFC,  // 11111100 - Stop
    ActiveSensing    = 0xFE,  // 11111110 - Active Sensing
    SystemReset      = 0xFF,  // 11111111 - System Reset
};

// Стандартные MIDI контроллеры
enum class MidiController : unsigned char {
    // Педали
    SustainPedal     = 0x40,  // 64 - педаль сустейна (как в вашем коде)
    PortamentoPedal  = 0x41,  // 65 - педаль портаменто
    SostenutoPedal   = 0x42,  // 66 - педаль сотенуто
    SoftPedal        = 0x43,  // 67 - мягкая педаль
    Hold2            = 0x45,  // 69 - Hold 2

    // Управление звуком
    Volume           = 0x07,  // 7 - громкость канала
    Pan              = 0x0A,  // 10 - панорама
    Expression       = 0x0B,  // 11 - экспрессия
    ReverbSend       = 0x5B,  // 91 - посыл на ревербератор
    ChorusSend       = 0x5D,  // 93 - посыл на хорус

    // Wheel/Levers
    ModulationWheel  = 0x01,  // 1 - колесо модуляции
    BreathControl    = 0x02,  // 2 - дыхание
    FootControl      = 0x04,  // 4 - ножной контроллер
    PortamentoTime   = 0x05,  // 5 - время портаменто

    // RPN/NRPN
    RPN_MSB          = 0x65,  // 101 - Registered Parameter Number MSB
    RPN_LSB          = 0x64,  // 100 - Registered Parameter Number LSB
    NRPN_MSB         = 0x63,  // 99 - Non-Registered Parameter Number MSB
    NRPN_LSB         = 0x62,  // 98 - Non-Registered Parameter Number LSB
    DataEntry_MSB     = 0x06,  // 6 - Data Entry MSB
    DataEntry_LSB     = 0x26,  // 38 - Data Entry LSB

    // Yamaha PSR-E333 специфичные
    FilterCutoff     = 0x4A,  // 74 - Cutoff (яркость) на Yamaha
    FilterResonance  = 0x47,  // 71 - Resonance (гармоники) на Yamaha
    Attack           = 0x49,  // 73 - Attack (атака)
    Release          = 0x48,  // 72 - Release (затухание)
};

// Номера нот октавы (для справки)
enum class MidiNoteOctave : int {
    C  = 0,
    CSharp = 1,
    D  = 2,
    DSharp = 3,
    E  = 4,
    F  = 5,
    FSharp = 6,
    G  = 7,
    GSharp = 8,
    A  = 9,
    ASharp = 10,
    B  = 11
};

#endif // MIDIEVENTLIST_H
