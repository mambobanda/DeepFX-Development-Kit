/****************************************************************************
** Meta object code from reading C++ file 'DfxQHotkeyEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxQHotkeyEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxQHotkeyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HotKeyButton_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HotKeyButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HotKeyButton_t qt_meta_stringdata_HotKeyButton = {
    {
QT_MOC_LITERAL(0, 0, 12), // "HotKeyButton"
QT_MOC_LITERAL(1, 13, 8), // "_clicked"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "HotKeyButton\0_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HotKeyButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void HotKeyButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HotKeyButton *_t = static_cast<HotKeyButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject HotKeyButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_HotKeyButton.data,
      qt_meta_data_HotKeyButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HotKeyButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HotKeyButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HotKeyButton.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "DfxQKeyBuffer"))
        return static_cast< DfxQKeyBuffer*>(this);
    return QPushButton::qt_metacast(_clname);
}

int HotKeyButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_DfxQHotkeyEditor_t {
    QByteArrayData data[12];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxQHotkeyEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxQHotkeyEditor_t qt_meta_stringdata_DfxQHotkeyEditor = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DfxQHotkeyEditor"
QT_MOC_LITERAL(1, 17, 15), // "commandSelected"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 51, 4), // "item"
QT_MOC_LITERAL(5, 56, 6), // "column"
QT_MOC_LITERAL(6, 63, 22), // "removedSelectedCommand"
QT_MOC_LITERAL(7, 86, 23), // "updateNavigationHotkeys"
QT_MOC_LITERAL(8, 110, 5), // "index"
QT_MOC_LITERAL(9, 116, 10), // "saveToFile"
QT_MOC_LITERAL(10, 127, 12), // "loadFromFile"
QT_MOC_LITERAL(11, 140, 14) // "resetShortcuts"

    },
    "DfxQHotkeyEditor\0commandSelected\0\0"
    "QTreeWidgetItem*\0item\0column\0"
    "removedSelectedCommand\0updateNavigationHotkeys\0"
    "index\0saveToFile\0loadFromFile\0"
    "resetShortcuts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxQHotkeyEditor[] = {

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
       1,    2,   44,    2, 0x0a /* Public */,
       6,    1,   49,    2, 0x0a /* Public */,
       7,    1,   52,    2, 0x0a /* Public */,
       9,    1,   55,    2, 0x0a /* Public */,
      10,    1,   58,    2, 0x0a /* Public */,
      11,    1,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void DfxQHotkeyEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxQHotkeyEditor *_t = static_cast<DfxQHotkeyEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commandSelected((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->removedSelectedCommand((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updateNavigationHotkeys((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->saveToFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->loadFromFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->resetShortcuts((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxQHotkeyEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DfxQHotkeyEditor.data,
      qt_meta_data_DfxQHotkeyEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxQHotkeyEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxQHotkeyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxQHotkeyEditor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DfxQHotkeyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
