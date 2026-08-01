/****************************************************************************
** Meta object code from reading C++ file 'DfxQSceneView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxQSceneView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxQSceneView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxQSceneView_t {
    QByteArrayData data[15];
    char stringdata0[267];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxQSceneView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxQSceneView_t qt_meta_stringdata_DfxQSceneView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DfxQSceneView"
QT_MOC_LITERAL(1, 14, 15), // "updateViewports"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "switchToSingle"
QT_MOC_LITERAL(4, 46, 12), // "switchToQuad"
QT_MOC_LITERAL(5, 59, 22), // "switchToDoubleVertical"
QT_MOC_LITERAL(6, 82, 24), // "switchToDoubleHorizontal"
QT_MOC_LITERAL(7, 107, 19), // "switchToTop2Bottom1"
QT_MOC_LITERAL(8, 127, 19), // "switchToTop1Bottom2"
QT_MOC_LITERAL(9, 147, 19), // "switchToTop3Bottom1"
QT_MOC_LITERAL(10, 167, 19), // "switchToTop1Bottom3"
QT_MOC_LITERAL(11, 187, 19), // "switchToLeft2Right1"
QT_MOC_LITERAL(12, 207, 19), // "switchToLeft1Right2"
QT_MOC_LITERAL(13, 227, 19), // "switchToLeft3Right1"
QT_MOC_LITERAL(14, 247, 19) // "switchToLeft1Right3"

    },
    "DfxQSceneView\0updateViewports\0\0"
    "switchToSingle\0switchToQuad\0"
    "switchToDoubleVertical\0switchToDoubleHorizontal\0"
    "switchToTop2Bottom1\0switchToTop1Bottom2\0"
    "switchToTop3Bottom1\0switchToTop1Bottom3\0"
    "switchToLeft2Right1\0switchToLeft1Right2\0"
    "switchToLeft3Right1\0switchToLeft1Right3"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxQSceneView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    0,   81,    2, 0x0a /* Public */,
       5,    0,   82,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x0a /* Public */,
       7,    0,   84,    2, 0x0a /* Public */,
       8,    0,   85,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x0a /* Public */,
      10,    0,   87,    2, 0x0a /* Public */,
      11,    0,   88,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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

void DfxQSceneView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxQSceneView *_t = static_cast<DfxQSceneView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateViewports(); break;
        case 1: _t->switchToSingle(); break;
        case 2: _t->switchToQuad(); break;
        case 3: _t->switchToDoubleVertical(); break;
        case 4: _t->switchToDoubleHorizontal(); break;
        case 5: _t->switchToTop2Bottom1(); break;
        case 6: _t->switchToTop1Bottom2(); break;
        case 7: _t->switchToTop3Bottom1(); break;
        case 8: _t->switchToTop1Bottom3(); break;
        case 9: _t->switchToLeft2Right1(); break;
        case 10: _t->switchToLeft1Right2(); break;
        case 11: _t->switchToLeft3Right1(); break;
        case 12: _t->switchToLeft1Right3(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DfxQSceneView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DfxQSceneView.data,
      qt_meta_data_DfxQSceneView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxQSceneView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxQSceneView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxQSceneView.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int DfxQSceneView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
