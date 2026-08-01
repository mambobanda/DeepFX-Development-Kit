/****************************************************************************
** Meta object code from reading C++ file 'DfxChannelGraphView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxChannelGraphView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxChannelGraphView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxChannelGraphView_t {
    QByteArrayData data[10];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxChannelGraphView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxChannelGraphView_t qt_meta_stringdata_DfxChannelGraphView = {
    {
QT_MOC_LITERAL(0, 0, 19), // "DfxChannelGraphView"
QT_MOC_LITERAL(1, 20, 15), // "channelsUpdated"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 17), // "newChannelCreated"
QT_MOC_LITERAL(4, 55, 12), // "updateCurves"
QT_MOC_LITERAL(5, 68, 43), // "std::vector<std::pair<std::st..."
QT_MOC_LITERAL(6, 112, 5), // "paths"
QT_MOC_LITERAL(7, 118, 23), // "updateInterpolationType"
QT_MOC_LITERAL(8, 142, 4), // "type"
QT_MOC_LITERAL(9, 147, 13) // "resetTangents"

    },
    "DfxChannelGraphView\0channelsUpdated\0"
    "\0newChannelCreated\0updateCurves\0"
    "std::vector<std::pair<std::string,QColor> >\0"
    "paths\0updateInterpolationType\0type\0"
    "resetTangents"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxChannelGraphView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    1,   41,    2, 0x0a /* Public */,
       7,    1,   44,    2, 0x0a /* Public */,
       9,    0,   47,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

void DfxChannelGraphView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxChannelGraphView *_t = static_cast<DfxChannelGraphView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->channelsUpdated(); break;
        case 1: _t->newChannelCreated(); break;
        case 2: _t->updateCurves((*reinterpret_cast< std::vector<std::pair<std::string,QColor> >(*)>(_a[1]))); break;
        case 3: _t->updateInterpolationType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->resetTangents(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxChannelGraphView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DfxChannelGraphView.data,
      qt_meta_data_DfxChannelGraphView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxChannelGraphView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxChannelGraphView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxChannelGraphView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DfxChannelGraphView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
