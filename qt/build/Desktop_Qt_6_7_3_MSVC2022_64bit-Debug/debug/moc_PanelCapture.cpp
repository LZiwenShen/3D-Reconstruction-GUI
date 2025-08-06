/****************************************************************************
** Meta object code from reading C++ file 'PanelCapture.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/app/PanelCapture.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PanelCapture.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPanelCaptureENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPanelCaptureENDCLASS = QtMocHelpers::stringData(
    "PanelCapture",
    "setInfoLabel",
    "",
    "text",
    "abort",
    "_on_captureEnable",
    "value",
    "_on_captureStarted",
    "const char*",
    "localScanDir",
    "_on_captureProgress",
    "imageFileName",
    "QByteArray*",
    "imageByteArray",
    "_on_captureFinished",
    "_on_streamEnable",
    "_on_streamStarted",
    "_on_streamProgress",
    "imageNumber",
    "_on_streamFinished",
    "_on_workDirectoryChanged",
    "on_scannerGroupCheck_stateChanged",
    "state",
    "on_scannerHostname_editingFinished",
    "on_scannerIpAdress_editingFinished",
    "on_scannerTestButton_clicked",
    "on_cameraGroupCheck_stateChanged",
    "on_cameraPreviewButton_clicked",
    "checked",
    "on_pixelFormatCombo_currentIndexChanged",
    "index",
    "on_captureGroupCheck_stateChanged",
    "on_stepsPerFrameSpin_valueChanged",
    "on_anglePerFrameLine_editingFinished",
    "on_blankFrameCheck_stateChanged",
    "on_laser1FrameCheck_stateChanged",
    "on_laser2FrameCheck_stateChanged",
    "on_captureButton_clicked",
    "on_serverSendImagesCheck_stateChanged",
    "on_serverSaveImagesCheck_stateChanged",
    "on_imagesGroupCheck_stateChanged",
    "_on_currentImageChanged",
    "QModelIndex",
    "current",
    "previous",
    "on_imagesUpdateButton_clicked",
    "on_imagesFirstButton_clicked",
    "on_imagesPreviousButton_clicked",
    "on_imagesNextButton_clicked",
    "on_imagesLastButton_clicked",
    "on_imageRotCombo_currentIndexChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPanelCaptureENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  224,    2, 0x06,    1 /* Public */,
       4,    0,  227,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,  228,    2, 0x0a,    4 /* Public */,
       7,    1,  231,    2, 0x0a,    6 /* Public */,
      10,    2,  234,    2, 0x0a,    8 /* Public */,
      14,    0,  239,    2, 0x0a,   11 /* Public */,
      15,    1,  240,    2, 0x0a,   12 /* Public */,
      16,    0,  243,    2, 0x0a,   14 /* Public */,
      17,    2,  244,    2, 0x0a,   15 /* Public */,
      19,    0,  249,    2, 0x0a,   18 /* Public */,
      20,    0,  250,    2, 0x09,   19 /* Protected */,
      21,    1,  251,    2, 0x09,   20 /* Protected */,
      23,    0,  254,    2, 0x09,   22 /* Protected */,
      24,    0,  255,    2, 0x09,   23 /* Protected */,
      25,    0,  256,    2, 0x09,   24 /* Protected */,
      26,    1,  257,    2, 0x09,   25 /* Protected */,
      27,    1,  260,    2, 0x09,   27 /* Protected */,
      29,    1,  263,    2, 0x09,   29 /* Protected */,
      31,    1,  266,    2, 0x09,   31 /* Protected */,
      32,    1,  269,    2, 0x09,   33 /* Protected */,
      33,    0,  272,    2, 0x09,   35 /* Protected */,
      34,    0,  273,    2, 0x09,   36 /* Protected */,
      35,    0,  274,    2, 0x09,   37 /* Protected */,
      36,    0,  275,    2, 0x09,   38 /* Protected */,
      37,    1,  276,    2, 0x09,   39 /* Protected */,
      38,    1,  279,    2, 0x09,   41 /* Protected */,
      39,    1,  282,    2, 0x09,   43 /* Protected */,
      40,    1,  285,    2, 0x09,   45 /* Protected */,
      41,    2,  288,    2, 0x09,   47 /* Protected */,
      45,    0,  293,    2, 0x09,   50 /* Protected */,
      46,    0,  294,    2, 0x09,   51 /* Protected */,
      47,    0,  295,    2, 0x09,   52 /* Protected */,
      48,    0,  296,    2, 0x09,   53 /* Protected */,
      49,    0,  297,    2, 0x09,   54 /* Protected */,
      50,    1,  298,    2, 0x09,   55 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 12,   11,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,   18,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Bool,   28,
    QMetaType::Void, QMetaType::Int,   30,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   28,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, 0x80000000 | 42, 0x80000000 | 42,   43,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   30,

       0        // eod
};

Q_CONSTINIT const QMetaObject PanelCapture::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPanelCaptureENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPanelCaptureENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPanelCaptureENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PanelCapture, std::true_type>,
        // method 'setInfoLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'abort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_captureEnable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const bool, std::false_type>,
        // method '_on_captureStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const char *, std::false_type>,
        // method '_on_captureProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const char *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray *, std::false_type>,
        // method '_on_captureFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_streamEnable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const bool, std::false_type>,
        // method '_on_streamStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_streamProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray *, std::false_type>,
        // method '_on_streamFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_workDirectoryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_scannerGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_scannerHostname_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_scannerIpAdress_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_scannerTestButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cameraGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_cameraPreviewButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_pixelFormatCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_captureGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_stepsPerFrameSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_anglePerFrameLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_blankFrameCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_laser1FrameCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_laser2FrameCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_captureButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_serverSendImagesCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_serverSaveImagesCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_imagesGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method '_on_currentImageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_imagesUpdateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesFirstButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesPreviousButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesNextButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesLastButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imageRotCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void PanelCapture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PanelCapture *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setInfoLabel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->abort(); break;
        case 2: _t->_on_captureEnable((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->_on_captureStarted((*reinterpret_cast< std::add_pointer_t<const char*>>(_a[1]))); break;
        case 4: _t->_on_captureProgress((*reinterpret_cast< std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray*>>(_a[2]))); break;
        case 5: _t->_on_captureFinished(); break;
        case 6: _t->_on_streamEnable((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->_on_streamStarted(); break;
        case 8: _t->_on_streamProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray*>>(_a[2]))); break;
        case 9: _t->_on_streamFinished(); break;
        case 10: _t->_on_workDirectoryChanged(); break;
        case 11: _t->on_scannerGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_scannerHostname_editingFinished(); break;
        case 13: _t->on_scannerIpAdress_editingFinished(); break;
        case 14: _t->on_scannerTestButton_clicked(); break;
        case 15: _t->on_cameraGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->on_cameraPreviewButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 17: _t->on_pixelFormatCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->on_captureGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->on_stepsPerFrameSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->on_anglePerFrameLine_editingFinished(); break;
        case 21: _t->on_blankFrameCheck_stateChanged(); break;
        case 22: _t->on_laser1FrameCheck_stateChanged(); break;
        case 23: _t->on_laser2FrameCheck_stateChanged(); break;
        case 24: _t->on_captureButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 25: _t->on_serverSendImagesCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->on_serverSaveImagesCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->on_imagesGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->_on_currentImageChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 29: _t->on_imagesUpdateButton_clicked(); break;
        case 30: _t->on_imagesFirstButton_clicked(); break;
        case 31: _t->on_imagesPreviousButton_clicked(); break;
        case 32: _t->on_imagesNextButton_clicked(); break;
        case 33: _t->on_imagesLastButton_clicked(); break;
        case 34: _t->on_imageRotCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PanelCapture::*)(QString );
            if (_t _q_method = &PanelCapture::setInfoLabel; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PanelCapture::*)();
            if (_t _q_method = &PanelCapture::abort; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *PanelCapture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PanelCapture::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPanelCaptureENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui_PanelCapture"))
        return static_cast< Ui_PanelCapture*>(this);
    return QWidget::qt_metacast(_clname);
}

int PanelCapture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void PanelCapture::setInfoLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PanelCapture::abort()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
