/****************************************************************************
** Meta object code from reading C++ file 'yamitracker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../yamitracker.h"
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
QT_MOC_LITERAL(3, 29, 4), // "note"
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
    "SerialReader\0noteOnReceived\0\0note\0"
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
    QByteArrayData data[54];
    char stringdata0[951];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Yamitracker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Yamitracker_t qt_meta_stringdata_Yamitracker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Yamitracker"
QT_MOC_LITERAL(1, 12, 16), // "onNoteOnReceived"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "note"
QT_MOC_LITERAL(4, 35, 8), // "velocity"
QT_MOC_LITERAL(5, 44, 17), // "onNoteOffReceived"
QT_MOC_LITERAL(6, 62, 7), // "onError"
QT_MOC_LITERAL(7, 70, 7), // "message"
QT_MOC_LITERAL(8, 78, 13), // "onPlayClicked"
QT_MOC_LITERAL(9, 92, 14), // "onPauseClicked"
QT_MOC_LITERAL(10, 107, 13), // "onStopClicked"
QT_MOC_LITERAL(11, 121, 21), // "onConvertToBrrClicked"
QT_MOC_LITERAL(12, 143, 18), // "clearStatusMessage"
QT_MOC_LITERAL(13, 162, 25), // "onConnectionStatusChanged"
QT_MOC_LITERAL(14, 188, 9), // "connected"
QT_MOC_LITERAL(15, 198, 16), // "attemptReconnect"
QT_MOC_LITERAL(16, 215, 20), // "onDeviceInfoReceived"
QT_MOC_LITERAL(17, 236, 12), // "manufacturer"
QT_MOC_LITERAL(18, 249, 5), // "model"
QT_MOC_LITERAL(19, 255, 23), // "onRefreshDevicesClicked"
QT_MOC_LITERAL(20, 279, 21), // "onLoadMidiFileClicked"
QT_MOC_LITERAL(21, 301, 25), // "onSelectFileForConversion"
QT_MOC_LITERAL(22, 327, 23), // "onSelectFileForPlayback"
QT_MOC_LITERAL(23, 351, 22), // "onSaveRecordingClicked"
QT_MOC_LITERAL(24, 374, 21), // "onPlayMidiFileClicked"
QT_MOC_LITERAL(25, 396, 25), // "onSelectedMidiFileChanged"
QT_MOC_LITERAL(26, 422, 20), // "onRecordFromMidiFile"
QT_MOC_LITERAL(27, 443, 22), // "onPlaybackSpeedChanged"
QT_MOC_LITERAL(28, 466, 5), // "value"
QT_MOC_LITERAL(29, 472, 19), // "onBassVolumeChanged"
QT_MOC_LITERAL(30, 492, 16), // "updateSpeedLabel"
QT_MOC_LITERAL(31, 509, 17), // "updateVolumeLabel"
QT_MOC_LITERAL(32, 527, 21), // "onCurrentTrackChanged"
QT_MOC_LITERAL(33, 549, 5), // "index"
QT_MOC_LITERAL(34, 555, 22), // "onSaveMusicFileClicked"
QT_MOC_LITERAL(35, 578, 17), // "onAddTrackClicked"
QT_MOC_LITERAL(36, 596, 20), // "onRemoveTrackClicked"
QT_MOC_LITERAL(37, 617, 19), // "onClearTrackClicked"
QT_MOC_LITERAL(38, 637, 18), // "onPlayTrackClicked"
QT_MOC_LITERAL(39, 656, 22), // "onPlayAllTracksClicked"
QT_MOC_LITERAL(40, 679, 21), // "onStopPlaybackClicked"
QT_MOC_LITERAL(41, 701, 23), // "onLoadWavToTrackClicked"
QT_MOC_LITERAL(42, 725, 20), // "onMergeTracksClicked"
QT_MOC_LITERAL(43, 746, 18), // "onCopyTrackClicked"
QT_MOC_LITERAL(44, 765, 18), // "onMuteTrackClicked"
QT_MOC_LITERAL(45, 784, 18), // "onSoloTrackClicked"
QT_MOC_LITERAL(46, 803, 20), // "onTrackVolumeChanged"
QT_MOC_LITERAL(47, 824, 18), // "onTrackNameChanged"
QT_MOC_LITERAL(48, 843, 24), // "onTrackInstrumentChanged"
QT_MOC_LITERAL(49, 868, 23), // "handleAudioStateChanged"
QT_MOC_LITERAL(50, 892, 13), // "QAudio::State"
QT_MOC_LITERAL(51, 906, 5), // "state"
QT_MOC_LITERAL(52, 912, 19), // "onAudioOutputNotify"
QT_MOC_LITERAL(53, 932, 18) // "onPlaybackFinished"

    },
    "Yamitracker\0onNoteOnReceived\0\0note\0"
    "velocity\0onNoteOffReceived\0onError\0"
    "message\0onPlayClicked\0onPauseClicked\0"
    "onStopClicked\0onConvertToBrrClicked\0"
    "clearStatusMessage\0onConnectionStatusChanged\0"
    "connected\0attemptReconnect\0"
    "onDeviceInfoReceived\0manufacturer\0"
    "model\0onRefreshDevicesClicked\0"
    "onLoadMidiFileClicked\0onSelectFileForConversion\0"
    "onSelectFileForPlayback\0onSaveRecordingClicked\0"
    "onPlayMidiFileClicked\0onSelectedMidiFileChanged\0"
    "onRecordFromMidiFile\0onPlaybackSpeedChanged\0"
    "value\0onBassVolumeChanged\0updateSpeedLabel\0"
    "updateVolumeLabel\0onCurrentTrackChanged\0"
    "index\0onSaveMusicFileClicked\0"
    "onAddTrackClicked\0onRemoveTrackClicked\0"
    "onClearTrackClicked\0onPlayTrackClicked\0"
    "onPlayAllTracksClicked\0onStopPlaybackClicked\0"
    "onLoadWavToTrackClicked\0onMergeTracksClicked\0"
    "onCopyTrackClicked\0onMuteTrackClicked\0"
    "onSoloTrackClicked\0onTrackVolumeChanged\0"
    "onTrackNameChanged\0onTrackInstrumentChanged\0"
    "handleAudioStateChanged\0QAudio::State\0"
    "state\0onAudioOutputNotify\0onPlaybackFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Yamitracker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  224,    2, 0x08 /* Private */,
       5,    1,  229,    2, 0x08 /* Private */,
       6,    1,  232,    2, 0x08 /* Private */,
       8,    0,  235,    2, 0x08 /* Private */,
       9,    0,  236,    2, 0x08 /* Private */,
      10,    0,  237,    2, 0x08 /* Private */,
      11,    0,  238,    2, 0x08 /* Private */,
      12,    0,  239,    2, 0x08 /* Private */,
      13,    1,  240,    2, 0x08 /* Private */,
      15,    0,  243,    2, 0x08 /* Private */,
      16,    2,  244,    2, 0x08 /* Private */,
      19,    0,  249,    2, 0x08 /* Private */,
      20,    0,  250,    2, 0x08 /* Private */,
      21,    0,  251,    2, 0x08 /* Private */,
      22,    0,  252,    2, 0x08 /* Private */,
      23,    0,  253,    2, 0x08 /* Private */,
      24,    0,  254,    2, 0x08 /* Private */,
      25,    0,  255,    2, 0x08 /* Private */,
      26,    0,  256,    2, 0x08 /* Private */,
      27,    1,  257,    2, 0x08 /* Private */,
      29,    1,  260,    2, 0x08 /* Private */,
      30,    0,  263,    2, 0x08 /* Private */,
      31,    0,  264,    2, 0x08 /* Private */,
      32,    1,  265,    2, 0x08 /* Private */,
      34,    0,  268,    2, 0x08 /* Private */,
      35,    0,  269,    2, 0x08 /* Private */,
      36,    0,  270,    2, 0x08 /* Private */,
      37,    0,  271,    2, 0x08 /* Private */,
      38,    0,  272,    2, 0x08 /* Private */,
      39,    0,  273,    2, 0x08 /* Private */,
      40,    0,  274,    2, 0x08 /* Private */,
      41,    0,  275,    2, 0x08 /* Private */,
      42,    0,  276,    2, 0x08 /* Private */,
      43,    0,  277,    2, 0x08 /* Private */,
      44,    0,  278,    2, 0x08 /* Private */,
      45,    0,  279,    2, 0x08 /* Private */,
      46,    1,  280,    2, 0x08 /* Private */,
      47,    0,  283,    2, 0x08 /* Private */,
      48,    1,  284,    2, 0x08 /* Private */,
      49,    1,  287,    2, 0x08 /* Private */,
      52,    0,  290,    2, 0x08 /* Private */,
      53,    0,  291,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   33,
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
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   33,
    QMetaType::Void, 0x80000000 | 50,   51,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Yamitracker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Yamitracker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onNoteOnReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onNoteOffReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onPlayClicked(); break;
        case 4: _t->onPauseClicked(); break;
        case 5: _t->onStopClicked(); break;
        case 6: _t->onConvertToBrrClicked(); break;
        case 7: _t->clearStatusMessage(); break;
        case 8: _t->onConnectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->attemptReconnect(); break;
        case 10: _t->onDeviceInfoReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->onRefreshDevicesClicked(); break;
        case 12: _t->onLoadMidiFileClicked(); break;
        case 13: _t->onSelectFileForConversion(); break;
        case 14: _t->onSelectFileForPlayback(); break;
        case 15: _t->onSaveRecordingClicked(); break;
        case 16: _t->onPlayMidiFileClicked(); break;
        case 17: _t->onSelectedMidiFileChanged(); break;
        case 18: _t->onRecordFromMidiFile(); break;
        case 19: _t->onPlaybackSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onBassVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->updateSpeedLabel(); break;
        case 22: _t->updateVolumeLabel(); break;
        case 23: _t->onCurrentTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->onSaveMusicFileClicked(); break;
        case 25: _t->onAddTrackClicked(); break;
        case 26: _t->onRemoveTrackClicked(); break;
        case 27: _t->onClearTrackClicked(); break;
        case 28: _t->onPlayTrackClicked(); break;
        case 29: _t->onPlayAllTracksClicked(); break;
        case 30: _t->onStopPlaybackClicked(); break;
        case 31: _t->onLoadWavToTrackClicked(); break;
        case 32: _t->onMergeTracksClicked(); break;
        case 33: _t->onCopyTrackClicked(); break;
        case 34: _t->onMuteTrackClicked(); break;
        case 35: _t->onSoloTrackClicked(); break;
        case 36: _t->onTrackVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->onTrackNameChanged(); break;
        case 38: _t->onTrackInstrumentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->handleAudioStateChanged((*reinterpret_cast< QAudio::State(*)>(_a[1]))); break;
        case 40: _t->onAudioOutputNotify(); break;
        case 41: _t->onPlaybackFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 39:
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
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
