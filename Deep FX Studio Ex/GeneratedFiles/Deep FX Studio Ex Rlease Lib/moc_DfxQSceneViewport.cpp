/****************************************************************************
** Meta object code from reading C++ file 'DfxQSceneViewport.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DfxQSceneViewport.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DfxQSceneViewport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DfxQSceneViewport_t {
    QByteArrayData data[18];
    char stringdata0[308];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DfxQSceneViewport_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DfxQSceneViewport_t qt_meta_stringdata_DfxQSceneViewport = {
    {
QT_MOC_LITERAL(0, 0, 17), // "DfxQSceneViewport"
QT_MOC_LITERAL(1, 18, 19), // "switchToPerspective"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "switchToTop"
QT_MOC_LITERAL(4, 51, 14), // "switchToBottom"
QT_MOC_LITERAL(5, 66, 13), // "switchToFront"
QT_MOC_LITERAL(6, 80, 12), // "switchToBack"
QT_MOC_LITERAL(7, 93, 12), // "switchToLeft"
QT_MOC_LITERAL(8, 106, 13), // "switchToRight"
QT_MOC_LITERAL(9, 120, 21), // "switchToWireframeMode"
QT_MOC_LITERAL(10, 142, 26), // "switchToWireframeSolidMode"
QT_MOC_LITERAL(11, 169, 17), // "switchToSolidMode"
QT_MOC_LITERAL(12, 187, 18), // "populateCameraMenu"
QT_MOC_LITERAL(13, 206, 26), // "populateCameraFromViewMenu"
QT_MOC_LITERAL(14, 233, 21), // "beginTransformHistory"
QT_MOC_LITERAL(15, 255, 19), // "endTransformHistory"
QT_MOC_LITERAL(16, 275, 21), // "clearTransformHistory"
QT_MOC_LITERAL(17, 297, 10) // "resetViews"

    },
    "DfxQSceneViewport\0switchToPerspective\0"
    "\0switchToTop\0switchToBottom\0switchToFront\0"
    "switchToBack\0switchToLeft\0switchToRight\0"
    "switchToWireframeMode\0switchToWireframeSolidMode\0"
    "switchToSolidMode\0populateCameraMenu\0"
    "populateCameraFromViewMenu\0"
    "beginTransformHistory\0endTransformHistory\0"
    "clearTransformHistory\0resetViews"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfxQSceneViewport[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    0,   95,    2, 0x0a /* Public */,
       4,    0,   96,    2, 0x0a /* Public */,
       5,    0,   97,    2, 0x0a /* Public */,
       6,    0,   98,    2, 0x0a /* Public */,
       7,    0,   99,    2, 0x0a /* Public */,
       8,    0,  100,    2, 0x0a /* Public */,
       9,    0,  101,    2, 0x0a /* Public */,
      10,    0,  102,    2, 0x0a /* Public */,
      11,    0,  103,    2, 0x0a /* Public */,
      12,    0,  104,    2, 0x0a /* Public */,
      13,    0,  105,    2, 0x0a /* Public */,
      14,    0,  106,    2, 0x0a /* Public */,
      15,    0,  107,    2, 0x0a /* Public */,
      16,    0,  108,    2, 0x0a /* Public */,
      17,    0,  109,    2, 0x0a /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DfxQSceneViewport::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfxQSceneViewport *_t = static_cast<DfxQSceneViewport *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->switchToPerspective(); break;
        case 1: _t->switchToTop(); break;
        case 2: _t->switchToBottom(); break;
        case 3: _t->switchToFront(); break;
        case 4: _t->switchToBack(); break;
        case 5: _t->switchToLeft(); break;
        case 6: _t->switchToRight(); break;
        case 7: _t->switchToWireframeMode(); break;
        case 8: _t->switchToWireframeSolidMode(); break;
        case 9: _t->switchToSolidMode(); break;
        case 10: _t->populateCameraMenu(); break;
        case 11: _t->populateCameraFromViewMenu(); break;
        case 12: _t->beginTransformHistory(); break;
        case 13: _t->endTransformHistory(); break;
        case 14: _t->clearTransformHistory(); break;
        case 15: _t->resetViews(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DfxQSceneViewport::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_DfxQSceneViewport.data,
      qt_meta_data_DfxQSceneViewport,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DfxQSceneViewport::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfxQSceneViewport::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DfxQSceneViewport.stringdata0))
        return static_cast<void*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int DfxQSceneViewport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
