/****************************************************************************
** Meta object code from reading C++ file 'PanelCalibrate.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/app/PanelCalibrate.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PanelCalibrate.hpp' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPanelCalibrateENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPanelCalibrateENDCLASS = QtMocHelpers::stringData(
    "PanelCalibrate",
    "_on_workDirectoryChanged",
    "",
    "_on_currentImageChanged",
    "QModelIndex",
    "current",
    "previous",
    "setGroupsEnabled",
    "value",
    "on_imagesGroupCheck_stateChanged",
    "state",
    "on_imagesUpdateButton_clicked",
    "on_imagesNextButton_clicked",
    "on_imagesPreviousButton_clicked",
    "on_anglePerFrameLine_editingFinished",
    "on_chessboardGroupCheck_stateChanged",
    "on_chessboardColsSpin_valueChanged",
    "on_chessboardRowsSpin_valueChanged",
    "on_chessboardCellWidthValue_textEdited",
    "text",
    "on_chessboardCellHeightValue_textEdited",
    "on_chessboardListAddButton_clicked",
    "on_chessboardListDeleteButton_clicked",
    "on_chessboardsCombo_currentIndexChanged",
    "index",
    "on_chessboardDrawPointLine0_editingFinished",
    "on_chessboardDrawPointLine1_editingFinished",
    "on_chessboardDrawPointLine2_editingFinished",
    "on_chessboardDrawPointCheck_clicked",
    "on_cameraGroupCheck_stateChanged",
    "on_drawCornersCheck_stateChanged",
    "on_drawChessboardPosesCheck_stateChanged",
    "on_chessboardDetectAndCalibrateButton_clicked",
    "checked",
    "on_clearCalibrationButton_clicked",
    "on_imageRotCombo_currentIndexChanged",
    "on_indxMinSpin_valueChanged",
    "i",
    "on_indxMaxSpin_valueChanged",
    "on_turntableGroupCheck_stateChanged",
    "on_drawTurntableFrameCheck_stateChanged",
    "on_turntableCalibrateButton_clicked",
    "on_lasersGroupCheck_stateChanged",
    "on_drawLaserBoundingQuadCheck_stateChanged",
    "on_drawLaserDetectedPointsCheck_stateChanged",
    "on_drawLaserTriangulatedPointsCheck_stateChanged",
    "on_drawLaserPlaneCheck_stateChanged",
    "on_laserEstimatePoseDetectButton_clicked",
    "on_laserTriangulateButton_clicked",
    "on_laserFitPlanesButton_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPanelCalibrateENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  248,    2, 0x09,    1 /* Protected */,
       3,    2,  249,    2, 0x09,    2 /* Protected */,
       7,    1,  254,    2, 0x09,    5 /* Protected */,
       9,    1,  257,    2, 0x09,    7 /* Protected */,
      11,    0,  260,    2, 0x09,    9 /* Protected */,
      12,    0,  261,    2, 0x09,   10 /* Protected */,
      13,    0,  262,    2, 0x09,   11 /* Protected */,
      14,    0,  263,    2, 0x09,   12 /* Protected */,
      15,    1,  264,    2, 0x09,   13 /* Protected */,
      16,    1,  267,    2, 0x09,   15 /* Protected */,
      17,    1,  270,    2, 0x09,   17 /* Protected */,
      18,    1,  273,    2, 0x09,   19 /* Protected */,
      20,    1,  276,    2, 0x09,   21 /* Protected */,
      21,    0,  279,    2, 0x09,   23 /* Protected */,
      22,    0,  280,    2, 0x09,   24 /* Protected */,
      23,    1,  281,    2, 0x09,   25 /* Protected */,
      25,    0,  284,    2, 0x09,   27 /* Protected */,
      26,    0,  285,    2, 0x09,   28 /* Protected */,
      27,    0,  286,    2, 0x09,   29 /* Protected */,
      28,    0,  287,    2, 0x09,   30 /* Protected */,
      29,    1,  288,    2, 0x09,   31 /* Protected */,
      30,    0,  291,    2, 0x09,   33 /* Protected */,
      31,    0,  292,    2, 0x09,   34 /* Protected */,
      32,    1,  293,    2, 0x09,   35 /* Protected */,
      34,    0,  296,    2, 0x09,   37 /* Protected */,
      35,    1,  297,    2, 0x09,   38 /* Protected */,
      36,    1,  300,    2, 0x09,   40 /* Protected */,
      38,    1,  303,    2, 0x09,   42 /* Protected */,
      39,    1,  306,    2, 0x09,   44 /* Protected */,
      40,    1,  309,    2, 0x09,   46 /* Protected */,
      41,    0,  312,    2, 0x09,   48 /* Protected */,
      42,    1,  313,    2, 0x09,   49 /* Protected */,
      43,    1,  316,    2, 0x09,   51 /* Protected */,
      44,    1,  319,    2, 0x09,   53 /* Protected */,
      45,    1,  322,    2, 0x09,   55 /* Protected */,
      46,    1,  325,    2, 0x09,   57 /* Protected */,
      47,    1,  328,    2, 0x09,   59 /* Protected */,
      48,    1,  331,    2, 0x09,   61 /* Protected */,
      49,    0,  334,    2, 0x09,   63 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 4,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PanelCalibrate::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSPanelCalibrateENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPanelCalibrateENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPanelCalibrateENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PanelCalibrate, std::true_type>,
        // method '_on_workDirectoryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method '_on_currentImageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'setGroupsEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const bool, std::false_type>,
        // method 'on_imagesGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_imagesUpdateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesNextButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imagesPreviousButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_anglePerFrameLine_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_chessboardColsSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_chessboardRowsSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_chessboardCellWidthValue_textEdited'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_chessboardCellHeightValue_textEdited'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_chessboardListAddButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardListDeleteButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardsCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_chessboardDrawPointLine0_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardDrawPointLine1_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardDrawPointLine2_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardDrawPointCheck_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cameraGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawCornersCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_drawChessboardPosesCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chessboardDetectAndCalibrateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_clearCalibrationButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_imageRotCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_indxMinSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_indxMaxSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_turntableGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawTurntableFrameCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_turntableCalibrateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_lasersGroupCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawLaserBoundingQuadCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawLaserDetectedPointsCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawLaserTriangulatedPointsCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_drawLaserPlaneCheck_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_laserEstimatePoseDetectButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_laserTriangulateButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_laserFitPlanesButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PanelCalibrate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PanelCalibrate *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->_on_workDirectoryChanged(); break;
        case 1: _t->_on_currentImageChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 2: _t->setGroupsEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->on_imagesGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_imagesUpdateButton_clicked(); break;
        case 5: _t->on_imagesNextButton_clicked(); break;
        case 6: _t->on_imagesPreviousButton_clicked(); break;
        case 7: _t->on_anglePerFrameLine_editingFinished(); break;
        case 8: _t->on_chessboardGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->on_chessboardColsSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->on_chessboardRowsSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->on_chessboardCellWidthValue_textEdited((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->on_chessboardCellHeightValue_textEdited((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->on_chessboardListAddButton_clicked(); break;
        case 14: _t->on_chessboardListDeleteButton_clicked(); break;
        case 15: _t->on_chessboardsCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->on_chessboardDrawPointLine0_editingFinished(); break;
        case 17: _t->on_chessboardDrawPointLine1_editingFinished(); break;
        case 18: _t->on_chessboardDrawPointLine2_editingFinished(); break;
        case 19: _t->on_chessboardDrawPointCheck_clicked(); break;
        case 20: _t->on_cameraGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->on_drawCornersCheck_stateChanged(); break;
        case 22: _t->on_drawChessboardPosesCheck_stateChanged(); break;
        case 23: _t->on_chessboardDetectAndCalibrateButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 24: _t->on_clearCalibrationButton_clicked(); break;
        case 25: _t->on_imageRotCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->on_indxMinSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->on_indxMaxSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->on_turntableGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->on_drawTurntableFrameCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->on_turntableCalibrateButton_clicked(); break;
        case 31: _t->on_lasersGroupCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->on_drawLaserBoundingQuadCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->on_drawLaserDetectedPointsCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->on_drawLaserTriangulatedPointsCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->on_drawLaserPlaneCheck_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 36: _t->on_laserEstimatePoseDetectButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 37: _t->on_laserTriangulateButton_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 38: _t->on_laserFitPlanesButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *PanelCalibrate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PanelCalibrate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPanelCalibrateENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui_PanelCalibrate"))
        return static_cast< Ui_PanelCalibrate*>(this);
    return QWidget::qt_metacast(_clname);
}

int PanelCalibrate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 39;
    }
    return _id;
}
QT_WARNING_POP
