/****************************************************************************
** Meta object code from reading C++ file 'yamitracker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
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
#error "This file was generated using the moc from 5.15.13. It"
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
    QByteArrayData data[31];
    char stringdata0[512];
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
QT_MOC_LITERAL(9, 92, 13), // "onStopClicked"
QT_MOC_LITERAL(10, 106, 21), // "onConvertToBrrClicked"
QT_MOC_LITERAL(11, 128, 18), // "clearStatusMessage"
QT_MOC_LITERAL(12, 147, 25), // "onConnectionStatusChanged"
QT_MOC_LITERAL(13, 173, 9), // "connected"
QT_MOC_LITERAL(14, 183, 16), // "attemptReconnect"
QT_MOC_LITERAL(15, 200, 20), // "onDeviceInfoReceived"
QT_MOC_LITERAL(16, 221, 12), // "manufacturer"
QT_MOC_LITERAL(17, 234, 5), // "model"
QT_MOC_LITERAL(18, 240, 23), // "onRefreshDevicesClicked"
QT_MOC_LITERAL(19, 264, 21), // "onLoadMidiFileClicked"
QT_MOC_LITERAL(20, 286, 25), // "onSelectFileForConversion"
QT_MOC_LITERAL(21, 312, 23), // "onSelectFileForPlayback"
QT_MOC_LITERAL(22, 336, 22), // "onSaveRecordingClicked"
QT_MOC_LITERAL(23, 359, 21), // "onPlayMidiFileClicked"
QT_MOC_LITERAL(24, 381, 25), // "onSelectedMidiFileChanged"
QT_MOC_LITERAL(25, 407, 20), // "onRecordFromMidiFile"
QT_MOC_LITERAL(26, 428, 22), // "onPlaybackSpeedChanged"
QT_MOC_LITERAL(27, 451, 5), // "value"
QT_MOC_LITERAL(28, 457, 19), // "onBassVolumeChanged"
QT_MOC_LITERAL(29, 477, 16), // "updateSpeedLabel"
QT_MOC_LITERAL(30, 494, 17) // "updateVolumeLabel"

    },
    "Yamitracker\0onNoteOnReceived\0\0note\0"
    "velocity\0onNoteOffReceived\0onError\0"
    "message\0onPlayClicked\0onStopClicked\0"
    "onConvertToBrrClicked\0clearStatusMessage\0"
    "onConnectionStatusChanged\0connected\0"
    "attemptReconnect\0onDeviceInfoReceived\0"
    "manufacturer\0model\0onRefreshDevicesClicked\0"
    "onLoadMidiFileClicked\0onSelectFileForConversion\0"
    "onSelectFileForPlayback\0onSaveRecordingClicked\0"
    "onPlayMidiFileClicked\0onSelectedMidiFileChanged\0"
    "onRecordFromMidiFile\0onPlaybackSpeedChanged\0"
    "value\0onBassVolumeChanged\0updateSpeedLabel\0"
    "updateVolumeLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Yamitracker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  124,    2, 0x08 /* Private */,
       5,    1,  129,    2, 0x08 /* Private */,
       6,    1,  132,    2, 0x08 /* Private */,
       8,    0,  135,    2, 0x08 /* Private */,
       9,    0,  136,    2, 0x08 /* Private */,
      10,    0,  137,    2, 0x08 /* Private */,
      11,    0,  138,    2, 0x08 /* Private */,
      12,    1,  139,    2, 0x08 /* Private */,
      14,    0,  142,    2, 0x08 /* Private */,
      15,    2,  143,    2, 0x08 /* Private */,
      18,    0,  148,    2, 0x08 /* Private */,
      19,    0,  149,    2, 0x08 /* Private */,
      20,    0,  150,    2, 0x08 /* Private */,
      21,    0,  151,    2, 0x08 /* Private */,
      22,    0,  152,    2, 0x08 /* Private */,
      23,    0,  153,    2, 0x08 /* Private */,
      24,    0,  154,    2, 0x08 /* Private */,
      25,    0,  155,    2, 0x08 /* Private */,
      26,    1,  156,    2, 0x08 /* Private */,
      28,    1,  159,    2, 0x08 /* Private */,
      29,    0,  162,    2, 0x08 /* Private */,
      30,    0,  163,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
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
        case 4: _t->onStopClicked(); break;
        case 5: _t->onConvertToBrrClicked(); break;
        case 6: _t->clearStatusMessage(); break;
        case 7: _t->onConnectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->attemptReconnect(); break;
        case 9: _t->onDeviceInfoReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->onRefreshDevicesClicked(); break;
        case 11: _t->onLoadMidiFileClicked(); break;
        case 12: _t->onSelectFileForConversion(); break;
        case 13: _t->onSelectFileForPlayback(); break;
        case 14: _t->onSaveRecordingClicked(); break;
        case 15: _t->onPlayMidiFileClicked(); break;
        case 16: _t->onSelectedMidiFileChanged(); break;
        case 17: _t->onRecordFromMidiFile(); break;
        case 18: _t->onPlaybackSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onBassVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->updateSpeedLabel(); break;
        case 21: _t->updateVolumeLabel(); break;
        default: ;
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
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
