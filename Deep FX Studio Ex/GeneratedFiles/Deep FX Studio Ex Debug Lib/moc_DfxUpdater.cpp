/****************************************************************************
** Meta object code from reading C++ file 'DfxUpdater.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxUpdater.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxUpdater.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxUpdater_t {
    QByteArrayData data[7];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxUpdater_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxUpdater_t qt_meta_stringdata_DfxUpdater = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DfxUpdater"
QT_MOC_LITERAL(1, 11, 13), // "_sceneUpdated"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 21), // "_nodeTransformUpdated"
QT_MOC_LITERAL(4, 48, 28), // "std::vector<deepsg::SgNode*>"
QT_MOC_LITERAL(5, 77, 5), // "nodes"
QT_MOC_LITERAL(6, 83, 16) // "_materialUpdated"

    },
    "DfxUpdater\0_sceneUpdated\0\0"
    "_nodeTransformUpdated\0"
    "std::vector<deepsg::SgNode*>\0nodes\0"
    "_materialUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxUpdater[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,
       6,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void DfxUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxUpdater *_t = static_cast<DfxUpdater *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->_sceneUpdated(); break;
        case 1: _t->_nodeTransformUpdated((*reinterpret_cast< std::vector<deepsg::SgNode*>(*)>(_a[1]))); break;
        case 2: _t->_materialUpdated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxUpdater::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxUpdater::_sceneUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DfxUpdater::*_t)(std::vector<deepsg::SgNode*> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxUpdater::_nodeTransformUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DfxUpdater::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxUpdater::_materialUpdated)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxUpdater::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxUpdater.data,
      qt_meta_data_DfxUpdater,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxUpdater.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DfxUpdater::_sceneUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DfxUpdater::_nodeTransformUpdated(std::vector<deepsg::SgNode*> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DfxUpdater::_materialUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
