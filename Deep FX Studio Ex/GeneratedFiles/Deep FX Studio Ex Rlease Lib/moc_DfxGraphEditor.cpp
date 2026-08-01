/****************************************************************************
** Meta object code from reading C++ file 'DfxGraphEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxGraphEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxGraphEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxGraphEditor_t {
    QByteArrayData data[18];
    char stringdata0[330];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxGraphEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxGraphEditor_t qt_meta_stringdata_DfxGraphEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DfxGraphEditor"
QT_MOC_LITERAL(1, 15, 16), // "selectionChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 43), // "std::vector<std::pair<std::st..."
QT_MOC_LITERAL(4, 77, 5), // "paths"
QT_MOC_LITERAL(5, 83, 13), // "expandAllTree"
QT_MOC_LITERAL(6, 97, 23), // "channelSelectionChanged"
QT_MOC_LITERAL(7, 121, 14), // "QItemSelection"
QT_MOC_LITERAL(8, 136, 8), // "selected"
QT_MOC_LITERAL(9, 145, 10), // "deselected"
QT_MOC_LITERAL(10, 156, 19), // "newChannelSelection"
QT_MOC_LITERAL(11, 176, 19), // "channelModelChanged"
QT_MOC_LITERAL(12, 196, 24), // "useConstantInterpolation"
QT_MOC_LITERAL(13, 221, 22), // "useLinearInterpolation"
QT_MOC_LITERAL(14, 244, 22), // "useBeizerInterpolation"
QT_MOC_LITERAL(15, 267, 21), // "switchToSelectionMode"
QT_MOC_LITERAL(16, 289, 18), // "switchToAddKeyMode"
QT_MOC_LITERAL(17, 308, 21) // "switchToRemoveKeyMode"

    },
    "DfxGraphEditor\0selectionChanged\0\0"
    "std::vector<std::pair<std::string,QColor> >\0"
    "paths\0expandAllTree\0channelSelectionChanged\0"
    "QItemSelection\0selected\0deselected\0"
    "newChannelSelection\0channelModelChanged\0"
    "useConstantInterpolation\0"
    "useLinearInterpolation\0useBeizerInterpolation\0"
    "switchToSelectionMode\0switchToAddKeyMode\0"
    "switchToRemoveKeyMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxGraphEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   72,    2, 0x0a /* Public */,
       6,    2,   73,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    0,   81,    2, 0x0a /* Public */,
      14,    0,   82,    2, 0x0a /* Public */,
      15,    0,   83,    2, 0x0a /* Public */,
      16,    0,   84,    2, 0x0a /* Public */,
      17,    0,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7,    8,    9,
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

void DfxGraphEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxGraphEditor *_t = static_cast<DfxGraphEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast< std::vector<std::pair<std::string,QColor> >(*)>(_a[1]))); break;
        case 1: _t->expandAllTree(); break;
        case 2: _t->channelSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 3: _t->newChannelSelection(); break;
        case 4: _t->channelModelChanged(); break;
        case 5: _t->useConstantInterpolation(); break;
        case 6: _t->useLinearInterpolation(); break;
        case 7: _t->useBeizerInterpolation(); break;
        case 8: _t->switchToSelectionMode(); break;
        case 9: _t->switchToAddKeyMode(); break;
        case 10: _t->switchToRemoveKeyMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QItemSelection >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DfxGraphEditor::*_t)(std::vector<std::pair<std::string,QColor>> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DfxGraphEditor::selectionChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxGraphEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DfxGraphEditor.data,
      qt_meta_data_DfxGraphEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxGraphEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxGraphEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxGraphEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DfxGraphEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DfxGraphEditor::selectionChanged(std::vector<std::pair<std::string,QColor>> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
