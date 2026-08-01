/****************************************************************************
** Meta object code from reading C++ file 'DfxRenderView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxRenderView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxRenderView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxRenderView_t {
    QByteArrayData data[13];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxRenderView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxRenderView_t qt_meta_stringdata_DfxRenderView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DfxRenderView"
QT_MOC_LITERAL(1, 14, 15), // "updateRenderers"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "rendererChanged"
QT_MOC_LITERAL(4, 47, 1), // "i"
QT_MOC_LITERAL(5, 49, 13), // "updateBuffers"
QT_MOC_LITERAL(6, 63, 13), // "bufferUpdated"
QT_MOC_LITERAL(7, 77, 14), // "buffersCleared"
QT_MOC_LITERAL(8, 92, 13), // "refreshBuffer"
QT_MOC_LITERAL(9, 106, 14), // "updateProgress"
QT_MOC_LITERAL(10, 121, 10), // "stopRender"
QT_MOC_LITERAL(11, 132, 11), // "resizeImage"
QT_MOC_LITERAL(12, 144, 26) // "releaseCurrentRenderBuffer"

    },
    "DfxRenderView\0updateRenderers\0\0"
    "rendererChanged\0i\0updateBuffers\0"
    "bufferUpdated\0buffersCleared\0refreshBuffer\0"
    "updateProgress\0stopRender\0resizeImage\0"
    "releaseCurrentRenderBuffer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxRenderView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    1,   65,    2, 0x0a /* Public */,
       5,    0,   68,    2, 0x0a /* Public */,
       6,    1,   69,    2, 0x0a /* Public */,
       7,    0,   72,    2, 0x0a /* Public */,
       8,    0,   73,    2, 0x0a /* Public */,
       9,    1,   74,    2, 0x0a /* Public */,
      10,    0,   77,    2, 0x0a /* Public */,
      11,    1,   78,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void DfxRenderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxRenderView *_t = static_cast<DfxRenderView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateRenderers(); break;
        case 1: _t->rendererChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateBuffers(); break;
        case 3: _t->bufferUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->buffersCleared(); break;
        case 5: _t->refreshBuffer(); break;
        case 6: _t->updateProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->stopRender(); break;
        case 8: _t->resizeImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->releaseCurrentRenderBuffer(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DfxRenderView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DfxRenderView.data,
      qt_meta_data_DfxRenderView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxRenderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxRenderView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxRenderView.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int DfxRenderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
