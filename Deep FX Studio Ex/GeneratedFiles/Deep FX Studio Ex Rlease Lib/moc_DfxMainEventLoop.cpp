/****************************************************************************
** Meta object code from reading C++ file 'DfxMainEventLoop.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxMainEventLoop.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxMainEventLoop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxMainEventLoop_t {
    QByteArrayData data[8];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxMainEventLoop_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxMainEventLoop_t qt_meta_stringdata_DfxMainEventLoop = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DfxMainEventLoop"
QT_MOC_LITERAL(1, 17, 23), // "finshedProcessingEvents"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 13), // "processEvents"
QT_MOC_LITERAL(4, 56, 5), // "frame"
QT_MOC_LITERAL(5, 62, 4), // "type"
QT_MOC_LITERAL(6, 67, 17), // "std::vector<uint>"
QT_MOC_LITERAL(7, 85, 11) // "ignore_list"

    },
    "DfxMainEventLoop\0finshedProcessingEvents\0"
    "\0processEvents\0frame\0type\0std::vector<uint>\0"
    "ignore_list"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxMainEventLoop[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    3,   30,    2, 0x0a /* Public */,
       3,    2,   37,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, 0x80000000 | 6,    4,    5,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt,    4,    5,

       0        // eod
};

void DfxMainEventLoop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxMainEventLoop *_t = static_cast<DfxMainEventLoop *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finshedProcessingEvents(); break;
        case 1: _t->processEvents((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< std::vector<uint>(*)>(_a[3]))); break;
        case 2: _t->processEvents((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxMainEventLoop::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxMainEventLoop::finshedProcessingEvents)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxMainEventLoop::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxMainEventLoop.data,
      qt_meta_data_DfxMainEventLoop,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxMainEventLoop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxMainEventLoop::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxMainEventLoop.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxMainEventLoop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DfxMainEventLoop::finshedProcessingEvents()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
