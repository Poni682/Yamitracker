/****************************************************************************
** Meta object code from reading C++ file 'yamitracker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/yamitracker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'yamitracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialReader_t {
    QByteArrayData data[13];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialReader_t qt_meta_stringdata_SerialReader = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SerialReader"
QT_MOC_LITERAL(1, 13, 14), // "noteOnReceived"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "data"
QT_MOC_LITERAL(4, 34, 8), // "velocity"
QT_MOC_LITERAL(5, 43, 15), // "noteOffReceived"
QT_MOC_LITERAL(6, 59, 5), // "error"
QT_MOC_LITERAL(7, 65, 7), // "message"
QT_MOC_LITERAL(8, 73, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(9, 97, 9), // "connected"
QT_MOC_LITERAL(10, 107, 18), // "deviceInfoReceived"
QT_MOC_LITERAL(11, 126, 12), // "manufacturer"
QT_MOC_LITERAL(12, 139, 5) // "model"

    },
    "SerialReader\0noteOnReceived\0\0data\0"
    "velocity\0noteOffReceived\0error\0message\0"
    "connectionStatusChanged\0connected\0"
    "deviceInfoReceived\0manufacturer\0model"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialReader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    1,   44,    2, 0x06 /* Public */,
       6,    1,   47,    2, 0x06 /* Public */,
       8,    1,   50,    2, 0x06 /* Public */,
      10,    2,   53,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,   12,

       0        // eod
};

void SerialReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialReader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->noteOnReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->noteOffReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->deviceInfoReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialReader::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReader::noteOnReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialReader::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReader::noteOffReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialReader::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReader::error)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialReader::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReader::connectionStatusChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialReader::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReader::deviceInfoReceived)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialReader::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_SerialReader.data,
    qt_meta_data_SerialReader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialReader.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SerialReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SerialReader::noteOnReceived(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialReader::noteOffReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SerialReader::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialReader::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialReader::deviceInfoReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_Yamitracker_t {
    QByteArrayData data[51];
    char stringdata0[896];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Yamitracker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Yamitracker_t qt_meta_stringdata_Yamitracker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Yamitracker"
QT_MOC_LITERAL(1, 12, 13), // "onPlayClicked"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 14), // "onPauseClicked"
QT_MOC_LITERAL(4, 42, 13), // "onStopClicked"
QT_MOC_LITERAL(5, 56, 22), // "onSaveRecordingClicked"
QT_MOC_LITERAL(6, 79, 21), // "onConvertToBrrClicked"
QT_MOC_LITERAL(7, 101, 22), // "onSaveMusicFileClicked"
QT_MOC_LITERAL(8, 124, 23), // "onRefreshDevicesClicked"
QT_MOC_LITERAL(9, 148, 21), // "onLoadMidiFileClicked"
QT_MOC_LITERAL(10, 170, 21), // "onPlayMidiFileClicked"
QT_MOC_LITERAL(11, 192, 20), // "onRecordFromMidiFile"
QT_MOC_LITERAL(12, 213, 25), // "onSelectedMidiFileChanged"
QT_MOC_LITERAL(13, 239, 25), // "onSelectFileForConversion"
QT_MOC_LITERAL(14, 265, 23), // "onSelectFileForPlayback"
QT_MOC_LITERAL(15, 289, 16), // "onNoteOnReceived"
QT_MOC_LITERAL(16, 306, 4), // "data"
QT_MOC_LITERAL(17, 311, 8), // "velocity"
QT_MOC_LITERAL(18, 320, 17), // "onNoteOffReceived"
QT_MOC_LITERAL(19, 338, 25), // "onConnectionStatusChanged"
QT_MOC_LITERAL(20, 364, 9), // "connected"
QT_MOC_LITERAL(21, 374, 7), // "onError"
QT_MOC_LITERAL(22, 382, 7), // "message"
QT_MOC_LITERAL(23, 390, 20), // "onDeviceInfoReceived"
QT_MOC_LITERAL(24, 411, 12), // "manufacturer"
QT_MOC_LITERAL(25, 424, 5), // "model"
QT_MOC_LITERAL(26, 430, 21), // "onCurrentTrackChanged"
QT_MOC_LITERAL(27, 452, 5), // "index"
QT_MOC_LITERAL(28, 458, 17), // "onAddTrackClicked"
QT_MOC_LITERAL(29, 476, 20), // "onRemoveTrackClicked"
QT_MOC_LITERAL(30, 497, 19), // "onClearTrackClicked"
QT_MOC_LITERAL(31, 517, 18), // "onCopyTrackClicked"
QT_MOC_LITERAL(32, 536, 20), // "onMergeTracksClicked"
QT_MOC_LITERAL(33, 557, 23), // "onLoadWavToTrackClicked"
QT_MOC_LITERAL(34, 581, 18), // "onPlayTrackClicked"
QT_MOC_LITERAL(35, 600, 22), // "onPlayAllTracksClicked"
QT_MOC_LITERAL(36, 623, 21), // "onStopPlaybackClicked"
QT_MOC_LITERAL(37, 645, 18), // "onPlaybackFinished"
QT_MOC_LITERAL(38, 664, 18), // "onMuteTrackClicked"
QT_MOC_LITERAL(39, 683, 18), // "onSoloTrackClicked"
QT_MOC_LITERAL(40, 702, 20), // "onTrackVolumeChanged"
QT_MOC_LITERAL(41, 723, 5), // "value"
QT_MOC_LITERAL(42, 729, 18), // "onTrackNameChanged"
QT_MOC_LITERAL(43, 748, 24), // "onTrackInstrumentChanged"
QT_MOC_LITERAL(44, 773, 22), // "onPlaybackSpeedChanged"
QT_MOC_LITERAL(45, 796, 19), // "onBassVolumeChanged"
QT_MOC_LITERAL(46, 816, 16), // "attemptReconnect"
QT_MOC_LITERAL(47, 833, 23), // "handleAudioStateChanged"
QT_MOC_LITERAL(48, 857, 13), // "QAudio::State"
QT_MOC_LITERAL(49, 871, 5), // "state"
QT_MOC_LITERAL(50, 877, 18) // "clearStatusMessage"

    },
    "Yamitracker\0onPlayClicked\0\0onPauseClicked\0"
    "onStopClicked\0onSaveRecordingClicked\0"
    "onConvertToBrrClicked\0onSaveMusicFileClicked\0"
    "onRefreshDevicesClicked\0onLoadMidiFileClicked\0"
    "onPlayMidiFileClicked\0onRecordFromMidiFile\0"
    "onSelectedMidiFileChanged\0"
    "onSelectFileForConversion\0"
    "onSelectFileForPlayback\0onNoteOnReceived\0"
    "data\0velocity\0onNoteOffReceived\0"
    "onConnectionStatusChanged\0connected\0"
    "onError\0message\0onDeviceInfoReceived\0"
    "manufacturer\0model\0onCurrentTrackChanged\0"
    "index\0onAddTrackClicked\0onRemoveTrackClicked\0"
    "onClearTrackClicked\0onCopyTrackClicked\0"
    "onMergeTracksClicked\0onLoadWavToTrackClicked\0"
    "onPlayTrackClicked\0onPlayAllTracksClicked\0"
    "onStopPlaybackClicked\0onPlaybackFinished\0"
    "onMuteTrackClicked\0onSoloTrackClicked\0"
    "onTrackVolumeChanged\0value\0"
    "onTrackNameChanged\0onTrackInstrumentChanged\0"
    "onPlaybackSpeedChanged\0onBassVolumeChanged\0"
    "attemptReconnect\0handleAudioStateChanged\0"
    "QAudio::State\0state\0clearStatusMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Yamitracker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  209,    2, 0x08 /* Private */,
       3,    0,  210,    2, 0x08 /* Private */,
       4,    0,  211,    2, 0x08 /* Private */,
       5,    0,  212,    2, 0x08 /* Private */,
       6,    0,  213,    2, 0x08 /* Private */,
       7,    0,  214,    2, 0x08 /* Private */,
       8,    0,  215,    2, 0x08 /* Private */,
       9,    0,  216,    2, 0x08 /* Private */,
      10,    0,  217,    2, 0x08 /* Private */,
      11,    0,  218,    2, 0x08 /* Private */,
      12,    0,  219,    2, 0x08 /* Private */,
      13,    0,  220,    2, 0x08 /* Private */,
      14,    0,  221,    2, 0x08 /* Private */,
      15,    2,  222,    2, 0x08 /* Private */,
      18,    1,  227,    2, 0x08 /* Private */,
      19,    1,  230,    2, 0x08 /* Private */,
      21,    1,  233,    2, 0x08 /* Private */,
      23,    2,  236,    2, 0x08 /* Private */,
      26,    1,  241,    2, 0x08 /* Private */,
      28,    0,  244,    2, 0x08 /* Private */,
      29,    0,  245,    2, 0x08 /* Private */,
      30,    0,  246,    2, 0x08 /* Private */,
      31,    0,  247,    2, 0x08 /* Private */,
      32,    0,  248,    2, 0x08 /* Private */,
      33,    0,  249,    2, 0x08 /* Private */,
      34,    0,  250,    2, 0x08 /* Private */,
      35,    0,  251,    2, 0x08 /* Private */,
      36,    0,  252,    2, 0x08 /* Private */,
      37,    0,  253,    2, 0x08 /* Private */,
      38,    0,  254,    2, 0x08 /* Private */,
      39,    0,  255,    2, 0x08 /* Private */,
      40,    1,  256,    2, 0x08 /* Private */,
      42,    0,  259,    2, 0x08 /* Private */,
      43,    1,  260,    2, 0x08 /* Private */,
      44,    1,  263,    2, 0x08 /* Private */,
      45,    1,  266,    2, 0x08 /* Private */,
      46,    0,  269,    2, 0x08 /* Private */,
      47,    1,  270,    2, 0x08 /* Private */,
      50,    0,  273,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   16,   17,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   24,   25,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 48,   49,
    QMetaType::Void,

       0        // eod
};

void Yamitracker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Yamitracker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onPlayClicked(); break;
        case 1: _t->onPauseClicked(); break;
        case 2: _t->onStopClicked(); break;
        case 3: _t->onSaveRecordingClicked(); break;
        case 4: _t->onConvertToBrrClicked(); break;
        case 5: _t->onSaveMusicFileClicked(); break;
        case 6: _t->onRefreshDevicesClicked(); break;
        case 7: _t->onLoadMidiFileClicked(); break;
        case 8: _t->onPlayMidiFileClicked(); break;
        case 9: _t->onRecordFromMidiFile(); break;
        case 10: _t->onSelectedMidiFileChanged(); break;
        case 11: _t->onSelectFileForConversion(); break;
        case 12: _t->onSelectFileForPlayback(); break;
        case 13: _t->onNoteOnReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->onNoteOffReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->onConnectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->onError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->onDeviceInfoReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: _t->onCurrentTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onAddTrackClicked(); break;
        case 20: _t->onRemoveTrackClicked(); break;
        case 21: _t->onClearTrackClicked(); break;
        case 22: _t->onCopyTrackClicked(); break;
        case 23: _t->onMergeTracksClicked(); break;
        case 24: _t->onLoadWavToTrackClicked(); break;
        case 25: _t->onPlayTrackClicked(); break;
        case 26: _t->onPlayAllTracksClicked(); break;
        case 27: _t->onStopPlaybackClicked(); break;
        case 28: _t->onPlaybackFinished(); break;
        case 29: _t->onMuteTrackClicked(); break;
        case 30: _t->onSoloTrackClicked(); break;
        case 31: _t->onTrackVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->onTrackNameChanged(); break;
        case 33: _t->onTrackInstrumentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->onPlaybackSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->onBassVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->attemptReconnect(); break;
        case 37: _t->handleAudioStateChanged((*reinterpret_cast< QAudio::State(*)>(_a[1]))); break;
        case 38: _t->clearStatusMessage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 37:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAudio::State >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Yamitracker::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_Yamitracker.data,
    qt_meta_data_Yamitracker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Yamitracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Yamitracker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Yamitracker.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Yamitracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
