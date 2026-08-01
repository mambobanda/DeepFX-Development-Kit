/****************************************************************************
** Meta object code from reading C++ file 'DfxQViewManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxQViewManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxQViewManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxQViewManager_t {
    QByteArrayData data[17];
    char stringdata0[184];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxQViewManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxQViewManager_t qt_meta_stringdata_DfxQViewManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "DfxQViewManager"
QT_MOC_LITERAL(1, 16, 18), // "splitViewContainer"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 15), // "Qt::Orientation"
QT_MOC_LITERAL(4, 52, 3), // "dir"
QT_MOC_LITERAL(5, 56, 18), // "DfxQViewContainer*"
QT_MOC_LITERAL(6, 75, 9), // "container"
QT_MOC_LITERAL(7, 85, 9), // "DfxQView*"
QT_MOC_LITERAL(8, 95, 5), // "other"
QT_MOC_LITERAL(9, 101, 10), // "DfxQViewID"
QT_MOC_LITERAL(10, 112, 3), // "vid"
QT_MOC_LITERAL(11, 116, 5), // "ratio"
QT_MOC_LITERAL(12, 122, 19), // "mergeViewContainerA"
QT_MOC_LITERAL(13, 142, 19), // "mergeViewContainerB"
QT_MOC_LITERAL(14, 162, 5), // "merge"
QT_MOC_LITERAL(15, 168, 4), // "view"
QT_MOC_LITERAL(16, 173, 10) // "minMaxView"

    },
    "DfxQViewManager\0splitViewContainer\0\0"
    "Qt::Orientation\0dir\0DfxQViewContainer*\0"
    "container\0DfxQView*\0other\0DfxQViewID\0"
    "vid\0ratio\0mergeViewContainerA\0"
    "mergeViewContainerB\0merge\0view\0"
    "minMaxView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxQViewManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    5,   44,    2, 0x0a /* Public */,
       1,    4,   55,    2, 0x2a /* Public | MethodCloned */,
      12,    1,   64,    2, 0x0a /* Public */,
      13,    1,   67,    2, 0x0a /* Public */,
      14,    1,   70,    2, 0x0a /* Public */,
      16,    1,   73,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 9, QMetaType::QSize,    4,    6,    8,   10,   11,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 9,    4,    6,    8,   10,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 7,   15,
    QMetaType::Void, 0x80000000 | 7,   15,

       0        // eod
};

void DfxQViewManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxQViewManager *_t = static_cast<DfxQViewManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->splitViewContainer((*reinterpret_cast< Qt::Orientation(*)>(_a[1])),(*reinterpret_cast< DfxQViewContainer*(*)>(_a[2])),(*reinterpret_cast< DfxQView*(*)>(_a[3])),(*reinterpret_cast< DfxQViewID(*)>(_a[4])),(*reinterpret_cast< QSize(*)>(_a[5]))); break;
        case 1: _t->splitViewContainer((*reinterpret_cast< Qt::Orientation(*)>(_a[1])),(*reinterpret_cast< DfxQViewContainer*(*)>(_a[2])),(*reinterpret_cast< DfxQView*(*)>(_a[3])),(*reinterpret_cast< DfxQViewID(*)>(_a[4]))); break;
        case 2: _t->mergeViewContainerA((*reinterpret_cast< DfxQViewContainer*(*)>(_a[1]))); break;
        case 3: _t->mergeViewContainerB((*reinterpret_cast< DfxQViewContainer*(*)>(_a[1]))); break;
        case 4: _t->merge((*reinterpret_cast< DfxQView*(*)>(_a[1]))); break;
        case 5: _t->minMaxView((*reinterpret_cast< DfxQView*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQView* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQViewContainer* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQView* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQViewContainer* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQViewContainer* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQViewContainer* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQView* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DfxQView* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxQViewManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DfxQViewManager.data,
      qt_meta_data_DfxQViewManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxQViewManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxQViewManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxQViewManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DfxQViewManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
