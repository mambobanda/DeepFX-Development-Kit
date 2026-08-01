/****************************************************************************
** Meta object code from reading C++ file 'DfxPropertyConduit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxPropertyConduit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxPropertyConduit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxPropertyConduit_t {
    QByteArrayData data[6];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxPropertyConduit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxPropertyConduit_t qt_meta_stringdata_DfxPropertyConduit = {
    {
QT_MOC_LITERAL(0, 0, 18), // "DfxPropertyConduit"
QT_MOC_LITERAL(1, 19, 18), // "newKeyFrameCreated"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 15), // "keyFrameUpdated"
QT_MOC_LITERAL(4, 55, 11), // "std::string"
QT_MOC_LITERAL(5, 67, 4) // "path"

    },
    "DfxPropertyConduit\0newKeyFrameCreated\0"
    "\0keyFrameUpdated\0std::string\0path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxPropertyConduit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    1,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void DfxPropertyConduit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxPropertyConduit *_t = static_cast<DfxPropertyConduit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newKeyFrameCreated(); break;
        case 1: _t->keyFrameUpdated((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxPropertyConduit::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxPropertyConduit::newKeyFrameCreated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DfxPropertyConduit::*_t)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxPropertyConduit::keyFrameUpdated)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxPropertyConduit::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxPropertyConduit.data,
      qt_meta_data_DfxPropertyConduit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxPropertyConduit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxPropertyConduit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxPropertyConduit.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxPropertyConduit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DfxPropertyConduit::newKeyFrameCreated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DfxPropertyConduit::keyFrameUpdated(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
