/****************************************************************************
** Meta object code from reading C++ file 'PanelScan.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/app/PanelScan.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PanelScan.hpp' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPanelScanENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPanelScanENDCLASS = QtMocHelpers::stringData(
    "PanelScan",
    "setInfoLabel",
    "",
    "text",
    "abort",
    "_on_workDirectoryChanged",
    "on_bboxGroupCheck_stateChanged",
    "state",
    "on_bboxP0XLine_editingFinished",
    "on_bboxP0YLine_editingFinished",
    "on_bboxP1XLine_editingFinished",
    "on_bboxP1YLine_editingFinished",
    "on_bboxP2XLine_editingFinished",
    "on_bboxP2YLine_editingFinished",
    "on_bboxP3XLine_editingFinished",
    "on_bboxP3YLine_editingFinished",
    "on_bboxFileLoadButton_clicked",
    "on_bboxFileSaveButton_clicked",
    "on_imagesGroupCheck_stateChanged",
    "_on_currentImageChanged",
    "QModelIndex",
    "current",
    "previous",
    "on_imagesUpdateButton_clicked",
    "on_imagesNextButton_clicked",
    "on_imagesPreviousButton_clicked",
    "on_imageRotCombo_currentIndexChanged",
    "index",
    "on_triangulationGroupCheck_stateChanged",
    "on_triangulationRunButton_clicked",
    "checked",
    "on_view3DGroupCheck_stateChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPanelScanENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  152,    2, 0x06,    1 /* Public */,
       4,    0,  155,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  156,    2, 0x09,    4 /* Protected */,
       6,    1,  157,    2, 0x09,    5 /* Protected */,
       8,    0,  160,    2, 0x09,    7 /* Protected */,
       9,    0,  161,    2, 0x09,    8 /* Protected */,
      10,    0,  162,    2, 0x09,    9 /* Protected */,
      11,    0,  163,    2, 0x09,   10 /* Protected */,
      12,    0,  164,    2, 0x09,   11 /* Protected */,
      13,    0,  165,    2, 0x09,   12 /* Protected */,
      14,    0,  166,    2, 0x09,   13 /* Protected */,
      15,    0,  167,    2, 0x09,   14 /* Protected */,
      16,    0,  168,    2, 0x09,   15 /* Protected */,
      17,    0,  169,    2, 0x09,   16 /* Protected */,
      18,    1,  170,    2, 0x09,   17 /* Protected */,
      19,    2,  173,    2, 0x09,   19 /* Protected */,
      23,    0,  178,    2, 0x09,   22 /* Protected */,
      24,    0,  179,    2, 0x09,   23 /* Protected */,
      25,    0,  180,    2, 0x09,   24 /* Protected */,
      26,    1,  181,    2, 0x09,   25 /* Protected */,
      28,    1,  184,    2, 0x09,   27 /* Protected */,
      29,    1,  187,    2, 0x09,   29 /* Protected */,
      31,    1,  190,    2, 0x09,   31 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
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
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 20,   21,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,   30,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject PanelScan::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPanelScanENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPanelScanENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPanelScanENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PanelScan, std::true_type>,
        // method 'setInfoLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'abort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_workDirectoryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_bboxP0XLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP0YLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP1XLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP1YLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP2XLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP2YLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP3XLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxP3YLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxFileLoadButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bboxFileSaveButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method '_on_currentImageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_imagesUpdateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesNextButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesPreviousButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imageRotCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_triangulationGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_triangulationRunButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_view3DGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void PanelScan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PanelScan *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setInfoLabel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->abort(); break;
        case 2: _t->_on_workDirectoryChanged(); break;
        case 3: _t->on_bboxGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_bboxP0XLine_editingFinished(); break;
        case 5: _t->on_bboxP0YLine_editingFinished(); break;
        case 6: _t->on_bboxP1XLine_editingFinished(); break;
        case 7: _t->on_bboxP1YLine_editingFinished(); break;
        case 8: _t->on_bboxP2XLine_editingFinished(); break;
        case 9: _t->on_bboxP2YLine_editingFinished(); break;
        case 10: _t->on_bboxP3XLine_editingFinished(); break;
        case 11: _t->on_bboxP3YLine_editingFinished(); break;
        case 12: _t->on_bboxFileLoadButton_clicked(); break;
        case 13: _t->on_bboxFileSaveButton_clicked(); break;
        case 14: _t->on_imagesGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->_on_currentImageChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 16: _t->on_imagesUpdateButton_clicked(); break;
        case 17: _t->on_imagesNextButton_clicked(); break;
        case 18: _t->on_imagesPreviousButton_clicked(); break;
        case 19: _t->on_imageRotCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->on_triangulationGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->on_triangulationRunButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->on_view3DGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PanelScan::*)(QString );
            if (_t _q_method = &PanelScan::setInfoLabel; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PanelScan::*)();
            if (_t _q_method = &PanelScan::abort; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *PanelScan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PanelScan::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPanelScanENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui_PanelScan"))
        return static_cast< Ui_PanelScan*>(this);
    return QWidget::qt_metacast(_clname);
}

int PanelScan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void PanelScan::setInfoLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PanelScan::abort()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
