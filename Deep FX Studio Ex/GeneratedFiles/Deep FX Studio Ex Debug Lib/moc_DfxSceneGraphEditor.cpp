/****************************************************************************
** Meta object code from reading C++ file 'DfxSceneGraphEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxSceneGraphEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxSceneGraphEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxSceneGraphEditor_t {
    QByteArrayData data[10];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxSceneGraphEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxSceneGraphEditor_t qt_meta_stringdata_DfxSceneGraphEditor = {
    {
QT_MOC_LITERAL(0, 0, 19), // "DfxSceneGraphEditor"
QT_MOC_LITERAL(1, 20, 14), // "autoExpandRoot"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 6), // "parent"
QT_MOC_LITERAL(4, 43, 5), // "first"
QT_MOC_LITERAL(5, 49, 4), // "last"
QT_MOC_LITERAL(6, 54, 12), // "resizeHeader"
QT_MOC_LITERAL(7, 67, 15), // "toggleVisiblity"
QT_MOC_LITERAL(8, 83, 9), // "itemindex"
QT_MOC_LITERAL(9, 93, 16) // "toggleRenderable"

    },
    "DfxSceneGraphEditor\0autoExpandRoot\0\0"
    "parent\0first\0last\0resizeHeader\0"
    "toggleVisiblity\0itemindex\0toggleRenderable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxSceneGraphEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x0a /* Public */,
       6,    0,   41,    2, 0x0a /* Public */,
       7,    1,   42,    2, 0x0a /* Public */,
       9,    1,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void, QMetaType::QModelIndex,    8,

       0        // eod
};

void DfxSceneGraphEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxSceneGraphEditor *_t = static_cast<DfxSceneGraphEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->autoExpandRoot((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->resizeHeader(); break;
        case 2: _t->toggleVisiblity((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->toggleRenderable((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxSceneGraphEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DfxSceneGraphEditor.data,
      qt_meta_data_DfxSceneGraphEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxSceneGraphEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxSceneGraphEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxSceneGraphEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DfxSceneGraphEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
