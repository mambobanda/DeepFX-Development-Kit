/****************************************************************************
** Meta object code from reading C++ file 'DfxQPropertiesHub.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxQPropertiesHub.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxQPropertiesHub.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxQPropertiesHub_t {
    QByteArrayData data[8];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxQPropertiesHub_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxQPropertiesHub_t qt_meta_stringdata_DfxQPropertiesHub = {
    {
QT_MOC_LITERAL(0, 0, 17), // "DfxQPropertiesHub"
QT_MOC_LITERAL(1, 18, 27), // "newSceneNodeSelected_signal"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 28), // "std::vector<deepsg::SgNode*>"
QT_MOC_LITERAL(4, 76, 5), // "nodes"
QT_MOC_LITERAL(5, 82, 28), // "newDataObjectSelected_signal"
QT_MOC_LITERAL(6, 111, 11), // "std::string"
QT_MOC_LITERAL(7, 123, 4) // "path"

    },
    "DfxQPropertiesHub\0newSceneNodeSelected_signal\0"
    "\0std::vector<deepsg::SgNode*>\0nodes\0"
    "newDataObjectSelected_signal\0std::string\0"
    "path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxQPropertiesHub[] = {

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
       1,    1,   24,    2, 0x06 /* Public */,
       5,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void DfxQPropertiesHub::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxQPropertiesHub *_t = static_cast<DfxQPropertiesHub *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newSceneNodeSelected_signal((*reinterpret_cast< std::vector<deepsg::SgNode*>(*)>(_a[1]))); break;
        case 1: _t->newDataObjectSelected_signal((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxQPropertiesHub::*_t)(std::vector<deepsg::SgNode*> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxQPropertiesHub::newSceneNodeSelected_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DfxQPropertiesHub::*_t)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxQPropertiesHub::newDataObjectSelected_signal)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxQPropertiesHub::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxQPropertiesHub.data,
      qt_meta_data_DfxQPropertiesHub,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxQPropertiesHub::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxQPropertiesHub::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxQPropertiesHub.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxQPropertiesHub::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DfxQPropertiesHub::newSceneNodeSelected_signal(std::vector<deepsg::SgNode*> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DfxQPropertiesHub::newDataObjectSelected_signal(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
