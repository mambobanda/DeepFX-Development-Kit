/****************************************************************************
** Meta object code from reading C++ file 'DfxTimelineSimulationPlayTask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxTimelineSimulationPlayTask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxTimelineSimulationPlayTask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxTimelineSimulationPlayTask_t {
    QByteArrayData data[7];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxTimelineSimulationPlayTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxTimelineSimulationPlayTask_t qt_meta_stringdata_DfxTimelineSimulationPlayTask = {
    {
QT_MOC_LITERAL(0, 0, 29), // "DfxTimelineSimulationPlayTask"
QT_MOC_LITERAL(1, 30, 18), // "nextFrameTriggered"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 18), // "goToFrameTriggered"
QT_MOC_LITERAL(4, 69, 8), // "finished"
QT_MOC_LITERAL(5, 78, 4), // "stop"
QT_MOC_LITERAL(6, 83, 9) // "nextFrame"

    },
    "DfxTimelineSimulationPlayTask\0"
    "nextFrameTriggered\0\0goToFrameTriggered\0"
    "finished\0stop\0nextFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxTimelineSimulationPlayTask[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       4,    0,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   44,    2, 0x0a /* Public */,
       6,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DfxTimelineSimulationPlayTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxTimelineSimulationPlayTask *_t = static_cast<DfxTimelineSimulationPlayTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nextFrameTriggered(); break;
        case 1: _t->goToFrameTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->finished(); break;
        case 3: _t->stop(); break;
        case 4: _t->nextFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxTimelineSimulationPlayTask::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxTimelineSimulationPlayTask::nextFrameTriggered)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DfxTimelineSimulationPlayTask::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxTimelineSimulationPlayTask::goToFrameTriggered)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DfxTimelineSimulationPlayTask::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxTimelineSimulationPlayTask::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxTimelineSimulationPlayTask::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DfxTimelineSimulationPlayTask.data,
      qt_meta_data_DfxTimelineSimulationPlayTask,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxTimelineSimulationPlayTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxTimelineSimulationPlayTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxTimelineSimulationPlayTask.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_3_1"))
        return static_cast< QOpenGLFunctions_3_1*>(this);
    return QThread::qt_metacast(_clname);
}

int DfxTimelineSimulationPlayTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DfxTimelineSimulationPlayTask::nextFrameTriggered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DfxTimelineSimulationPlayTask::goToFrameTriggered(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DfxTimelineSimulationPlayTask::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
