/****************************************************************************
** Meta object code from reading C++ file 'DfxTimelineManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxTimelineManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxTimelineManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxTimelineManager_t {
    QByteArrayData data[19];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxTimelineManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxTimelineManager_t qt_meta_stringdata_DfxTimelineManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "DfxTimelineManager"
QT_MOC_LITERAL(1, 19, 13), // "playTriggered"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "stopTriggered"
QT_MOC_LITERAL(4, 48, 9), // "goToFrame"
QT_MOC_LITERAL(5, 58, 2), // "ff"
QT_MOC_LITERAL(6, 61, 20), // "goToFrameNoInterrupt"
QT_MOC_LITERAL(7, 82, 13), // "goToNextFrame"
QT_MOC_LITERAL(8, 96, 24), // "goToNextFrameNoInterrupt"
QT_MOC_LITERAL(9, 121, 17), // "goToPreviousFrame"
QT_MOC_LITERAL(10, 139, 28), // "goToPreviousFrameNoInterrupt"
QT_MOC_LITERAL(11, 168, 14), // "goTofirstFrame"
QT_MOC_LITERAL(12, 183, 25), // "goTofirstFrameNoInterrupt"
QT_MOC_LITERAL(13, 209, 13), // "goToLastFrame"
QT_MOC_LITERAL(14, 223, 24), // "goToLastFrameNoInterrupt"
QT_MOC_LITERAL(15, 248, 4), // "play"
QT_MOC_LITERAL(16, 253, 11), // "recordCache"
QT_MOC_LITERAL(17, 265, 4), // "stop"
QT_MOC_LITERAL(18, 270, 16) // "playTaskFinished"

    },
    "DfxTimelineManager\0playTriggered\0\0"
    "stopTriggered\0goToFrame\0ff\0"
    "goToFrameNoInterrupt\0goToNextFrame\0"
    "goToNextFrameNoInterrupt\0goToPreviousFrame\0"
    "goToPreviousFrameNoInterrupt\0"
    "goTofirstFrame\0goTofirstFrameNoInterrupt\0"
    "goToLastFrame\0goToLastFrameNoInterrupt\0"
    "play\0recordCache\0stop\0playTaskFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxTimelineManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   96,    2, 0x0a /* Public */,
       6,    1,   99,    2, 0x0a /* Public */,
       7,    0,  102,    2, 0x0a /* Public */,
       8,    0,  103,    2, 0x0a /* Public */,
       9,    0,  104,    2, 0x0a /* Public */,
      10,    0,  105,    2, 0x0a /* Public */,
      11,    0,  106,    2, 0x0a /* Public */,
      12,    0,  107,    2, 0x0a /* Public */,
      13,    0,  108,    2, 0x0a /* Public */,
      14,    0,  109,    2, 0x0a /* Public */,
      15,    0,  110,    2, 0x0a /* Public */,
      16,    0,  111,    2, 0x0a /* Public */,
      17,    0,  112,    2, 0x0a /* Public */,
      18,    0,  113,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
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

       0        // eod
};

void DfxTimelineManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxTimelineManager *_t = static_cast<DfxTimelineManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playTriggered(); break;
        case 1: _t->stopTriggered(); break;
        case 2: _t->goToFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->goToFrameNoInterrupt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->goToNextFrame(); break;
        case 5: _t->goToNextFrameNoInterrupt(); break;
        case 6: _t->goToPreviousFrame(); break;
        case 7: _t->goToPreviousFrameNoInterrupt(); break;
        case 8: _t->goTofirstFrame(); break;
        case 9: _t->goTofirstFrameNoInterrupt(); break;
        case 10: _t->goToLastFrame(); break;
        case 11: _t->goToLastFrameNoInterrupt(); break;
        case 12: _t->play(); break;
        case 13: _t->recordCache(); break;
        case 14: _t->stop(); break;
        case 15: _t->playTaskFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxTimelineManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxTimelineManager::playTriggered)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DfxTimelineManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxTimelineManager::stopTriggered)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxTimelineManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxTimelineManager.data,
      qt_meta_data_DfxTimelineManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxTimelineManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxTimelineManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxTimelineManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxTimelineManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void DfxTimelineManager::playTriggered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DfxTimelineManager::stopTriggered()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
