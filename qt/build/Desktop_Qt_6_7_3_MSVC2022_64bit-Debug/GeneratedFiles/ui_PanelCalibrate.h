/********************************************************************************
** Form generated from reading UI file 'PanelCalibrate.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANELCALIBRATE_H
#define UI_PANELCALIBRATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PanelCalibrate
{
public:
    QVBoxLayout *panelCalibrationVerticalLayout;
    QLineEdit *statusBarLine;
    QProgressBar *calibrationProgress;
    QCheckBox *imagesGroupCheck;
    QGroupBox *imagesGroup;
    QVBoxLayout *imagesVLayout;
    QHBoxLayout *imagesHLayout1;
    QPushButton *imagesUpdateButton;
    QLabel *imagesFiller1;
    QPushButton *imagesPreviousButton;
    QPushButton *imagesNextButton;
    QHBoxLayout *imagesHLayout2;
    QLabel *imageCountLabel;
    QLineEdit *imageCountLine;
    QLabel *imageSizeLabel;
    QLineEdit *imageSizeLine;
    QLabel *imagesFiller2;
    QLabel *imageRotLabel;
    QComboBox *imageRotCombo;
    QListView *imageList;
    QCheckBox *chessboardGroupCheck;
    QGroupBox *chessboardGroup;
    QVBoxLayout *chessboardGroupVLayout;
    QHBoxLayout *chessboardHLayout1;
    QLabel *chessboardFill1;
    QLabel *chessboardColsLabel;
    QSpinBox *chessboardColsSpin;
    QLabel *chessboardRowsLabel;
    QSpinBox *chessboardRowsSpin;
    QLabel *chessboardCellWidthLabel;
    QLineEdit *chessboardCellWidthValue;
    QLabel *chessboardCellHeightLabel;
    QLineEdit *chessboardCellHeightValue;
    QHBoxLayout *chessboardHLayout2;
    QLabel *chessboardFill2;
    QComboBox *chessboardsCombo;
    QPushButton *chessboardListAddButton;
    QPushButton *chessboardListDeleteButton;
    QHBoxLayout *chessboardHLayout3;
    QLineEdit *chessboardDrawPointLine0;
    QLineEdit *chessboardDrawPointLine1;
    QLineEdit *chessboardDrawPointLine2;
    QCheckBox *chessboardDrawPointCheck;
    QCheckBox *cameraGroupCheck;
    QGroupBox *cameraGroup;
    QGridLayout *cameraCalibrateLayout;
    QPushButton *chessboardDetectAndCalibrateButton;
    QPushButton *clearCalibrationButton;
    QLabel *chesboardDetectedLabel;
    QLineEdit *chesboardDetectedLine;
    QCheckBox *drawCornersCheck;
    QCheckBox *drawChessboardPosesCheck;
    QLabel *KLabel;
    QLineEdit *K00Line;
    QLineEdit *K01Line;
    QLineEdit *K02Line;
    QLineEdit *K10Line;
    QLineEdit *K11Line;
    QLineEdit *K12Line;
    QLineEdit *K20Line;
    QLineEdit *K21Line;
    QLineEdit *K22Line;
    QLabel *kcLabel;
    QLineEdit *kc0Line;
    QLineEdit *kc1Line;
    QLineEdit *kc2Line;
    QLineEdit *kc3Line;
    QLineEdit *kc4Line;
    QLabel *calibErrorLabel;
    QLineEdit *calibErrorLine;
    QCheckBox *turntableGroupCheck;
    QGroupBox *turntableGroup;
    QGridLayout *turntableGroupGridLayout;
    QLabel *turntableFrameRangeLabel;
    QSpinBox *indxMinSpin;
    QSpinBox *indxMaxSpin;
    QPushButton *turntableCalibrateButton;
    QLabel *turntablePatternCenterLabel;
    QLineEdit *turntablePatternCenter0;
    QLineEdit *turntablePatternCenter1;
    QLineEdit *turntablePatternCenter2;
    QLabel *turntableCenterLabel;
    QLineEdit *turntableCenter0;
    QLineEdit *turntableCenter1;
    QLineEdit *turntableCenter2;
    QLabel *turntableAxisLabel;
    QLineEdit *turntableAxis0;
    QLineEdit *turntableAxis1;
    QLineEdit *turntableAxis2;
    QCheckBox *drawTurntableFrameCheck;
    QLabel *anglePerFrameLabel;
    QLineEdit *anglePerFrameLine;
    QCheckBox *lasersGroupCheck;
    QGroupBox *lasersGroup;
    QGridLayout *gridLayoutLaser;
    QCheckBox *drawLaserDetectedPointsCheck;
    QCheckBox *drawLaserBoundingQuadCheck;
    QCheckBox *drawLaserTriangulatedPointsCheck;
    QCheckBox *drawLaserPlaneCheck;
    QPushButton *laserEstimatePoseDetectButton;
    QPushButton *laserTriangulateButton;
    QPushButton *laserFitPlanesButton;
    QGridLayout *gridLayoutLaserPlanes;
    QLabel *laserPlane1Label;
    QLineEdit *laserPlane10;
    QLineEdit *laserPlane11;
    QLineEdit *laserPlane12;
    QLineEdit *laserPlane13;
    QLabel *laserPlane2Label;
    QLineEdit *laserPlane20;
    QLineEdit *laserPlane21;
    QLineEdit *laserPlane22;
    QLineEdit *laserPlane23;
    QSpacerItem *verticalSpacerPanelCalibrate;

    void setupUi(QWidget *PanelCalibrate)
    {
        if (PanelCalibrate->objectName().isEmpty())
            PanelCalibrate->setObjectName("PanelCalibrate");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PanelCalibrate->sizePolicy().hasHeightForWidth());
        PanelCalibrate->setSizePolicy(sizePolicy);
        PanelCalibrate->setMinimumSize(QSize(450, 100));
        PanelCalibrate->setMaximumSize(QSize(450, 4000));
        panelCalibrationVerticalLayout = new QVBoxLayout(PanelCalibrate);
        panelCalibrationVerticalLayout->setSpacing(5);
        panelCalibrationVerticalLayout->setContentsMargins(5, 5, 5, 5);
        panelCalibrationVerticalLayout->setObjectName("panelCalibrationVerticalLayout");
        statusBarLine = new QLineEdit(PanelCalibrate);
        statusBarLine->setObjectName("statusBarLine");
        statusBarLine->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        statusBarLine->setReadOnly(true);

        panelCalibrationVerticalLayout->addWidget(statusBarLine);

        calibrationProgress = new QProgressBar(PanelCalibrate);
        calibrationProgress->setObjectName("calibrationProgress");
        calibrationProgress->setValue(0);

        panelCalibrationVerticalLayout->addWidget(calibrationProgress);

        imagesGroupCheck = new QCheckBox(PanelCalibrate);
        imagesGroupCheck->setObjectName("imagesGroupCheck");
        imagesGroupCheck->setMinimumSize(QSize(100, 22));
        imagesGroupCheck->setChecked(true);

        panelCalibrationVerticalLayout->addWidget(imagesGroupCheck);

        imagesGroup = new QGroupBox(PanelCalibrate);
        imagesGroup->setObjectName("imagesGroup");
        imagesGroup->setFlat(true);
        imagesVLayout = new QVBoxLayout(imagesGroup);
        imagesVLayout->setSpacing(1);
        imagesVLayout->setContentsMargins(1, 1, 1, 1);
        imagesVLayout->setObjectName("imagesVLayout");
        imagesHLayout1 = new QHBoxLayout();
        imagesHLayout1->setSpacing(5);
        imagesHLayout1->setContentsMargins(1, 1, 1, 1);
        imagesHLayout1->setObjectName("imagesHLayout1");
        imagesUpdateButton = new QPushButton(imagesGroup);
        imagesUpdateButton->setObjectName("imagesUpdateButton");
        imagesUpdateButton->setEnabled(true);
        imagesUpdateButton->setCheckable(false);

        imagesHLayout1->addWidget(imagesUpdateButton);

        imagesFiller1 = new QLabel(imagesGroup);
        imagesFiller1->setObjectName("imagesFiller1");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(imagesFiller1->sizePolicy().hasHeightForWidth());
        imagesFiller1->setSizePolicy(sizePolicy1);
        imagesFiller1->setAlignment(Qt::AlignCenter);

        imagesHLayout1->addWidget(imagesFiller1);

        imagesPreviousButton = new QPushButton(imagesGroup);
        imagesPreviousButton->setObjectName("imagesPreviousButton");
        imagesPreviousButton->setEnabled(true);
        imagesPreviousButton->setCheckable(false);

        imagesHLayout1->addWidget(imagesPreviousButton);

        imagesNextButton = new QPushButton(imagesGroup);
        imagesNextButton->setObjectName("imagesNextButton");
        imagesNextButton->setEnabled(true);
        imagesNextButton->setCheckable(false);

        imagesHLayout1->addWidget(imagesNextButton);


        imagesVLayout->addLayout(imagesHLayout1);

        imagesHLayout2 = new QHBoxLayout();
        imagesHLayout2->setSpacing(5);
        imagesHLayout2->setContentsMargins(1, 1, 1, 1);
        imagesHLayout2->setObjectName("imagesHLayout2");
        imageCountLabel = new QLabel(imagesGroup);
        imageCountLabel->setObjectName("imageCountLabel");

        imagesHLayout2->addWidget(imageCountLabel);

        imageCountLine = new QLineEdit(imagesGroup);
        imageCountLine->setObjectName("imageCountLine");
        imageCountLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        imageCountLine->setReadOnly(true);
        imageCountLine->setMaximumSize(QSize(40, 22));

        imagesHLayout2->addWidget(imageCountLine);

        imageSizeLabel = new QLabel(imagesGroup);
        imageSizeLabel->setObjectName("imageSizeLabel");

        imagesHLayout2->addWidget(imageSizeLabel);

        imageSizeLine = new QLineEdit(imagesGroup);
        imageSizeLine->setObjectName("imageSizeLine");
        imageSizeLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        imageSizeLine->setReadOnly(true);
        imageSizeLine->setMaximumSize(QSize(90, 22));

        imagesHLayout2->addWidget(imageSizeLine);

        imagesFiller2 = new QLabel(imagesGroup);
        imagesFiller2->setObjectName("imagesFiller2");
        sizePolicy1.setHeightForWidth(imagesFiller2->sizePolicy().hasHeightForWidth());
        imagesFiller2->setSizePolicy(sizePolicy1);
        imagesFiller2->setAlignment(Qt::AlignCenter);

        imagesHLayout2->addWidget(imagesFiller2);

        imageRotLabel = new QLabel(imagesGroup);
        imageRotLabel->setObjectName("imageRotLabel");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(imageRotLabel->sizePolicy().hasHeightForWidth());
        imageRotLabel->setSizePolicy(sizePolicy2);
        imageRotLabel->setMaximumSize(QSize(75, 22));

        imagesHLayout2->addWidget(imageRotLabel);

        imageRotCombo = new QComboBox(imagesGroup);
        imageRotCombo->setObjectName("imageRotCombo");
        sizePolicy2.setHeightForWidth(imageRotCombo->sizePolicy().hasHeightForWidth());
        imageRotCombo->setSizePolicy(sizePolicy2);

        imagesHLayout2->addWidget(imageRotCombo);


        imagesVLayout->addLayout(imagesHLayout2);

        imageList = new QListView(imagesGroup);
        imageList->setObjectName("imageList");

        imagesVLayout->addWidget(imageList);


        panelCalibrationVerticalLayout->addWidget(imagesGroup);

        chessboardGroupCheck = new QCheckBox(PanelCalibrate);
        chessboardGroupCheck->setObjectName("chessboardGroupCheck");
        chessboardGroupCheck->setMinimumSize(QSize(100, 22));
        chessboardGroupCheck->setChecked(true);

        panelCalibrationVerticalLayout->addWidget(chessboardGroupCheck);

        chessboardGroup = new QGroupBox(PanelCalibrate);
        chessboardGroup->setObjectName("chessboardGroup");
        chessboardGroup->setFlat(true);
        chessboardGroupVLayout = new QVBoxLayout(chessboardGroup);
        chessboardGroupVLayout->setSpacing(1);
        chessboardGroupVLayout->setContentsMargins(1, 1, 1, 1);
        chessboardGroupVLayout->setObjectName("chessboardGroupVLayout");
        chessboardHLayout1 = new QHBoxLayout();
        chessboardHLayout1->setSpacing(5);
        chessboardHLayout1->setContentsMargins(1, 1, 1, 1);
        chessboardHLayout1->setObjectName("chessboardHLayout1");
        chessboardFill1 = new QLabel(chessboardGroup);
        chessboardFill1->setObjectName("chessboardFill1");
        sizePolicy1.setHeightForWidth(chessboardFill1->sizePolicy().hasHeightForWidth());
        chessboardFill1->setSizePolicy(sizePolicy1);
        chessboardFill1->setAlignment(Qt::AlignCenter);

        chessboardHLayout1->addWidget(chessboardFill1);

        chessboardColsLabel = new QLabel(chessboardGroup);
        chessboardColsLabel->setObjectName("chessboardColsLabel");
        chessboardColsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        chessboardHLayout1->addWidget(chessboardColsLabel);

        chessboardColsSpin = new QSpinBox(chessboardGroup);
        chessboardColsSpin->setObjectName("chessboardColsSpin");
        chessboardColsSpin->setMinimumSize(QSize(40, 22));
        chessboardColsSpin->setMaximumSize(QSize(40, 40));
        chessboardColsSpin->setValue(4);

        chessboardHLayout1->addWidget(chessboardColsSpin);

        chessboardRowsLabel = new QLabel(chessboardGroup);
        chessboardRowsLabel->setObjectName("chessboardRowsLabel");
        chessboardRowsLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        chessboardHLayout1->addWidget(chessboardRowsLabel);

        chessboardRowsSpin = new QSpinBox(chessboardGroup);
        chessboardRowsSpin->setObjectName("chessboardRowsSpin");
        chessboardRowsSpin->setMinimumSize(QSize(40, 22));
        chessboardRowsSpin->setMaximumSize(QSize(40, 40));
        chessboardRowsSpin->setValue(5);

        chessboardHLayout1->addWidget(chessboardRowsSpin);

        chessboardCellWidthLabel = new QLabel(chessboardGroup);
        chessboardCellWidthLabel->setObjectName("chessboardCellWidthLabel");
        chessboardCellWidthLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        chessboardHLayout1->addWidget(chessboardCellWidthLabel);

        chessboardCellWidthValue = new QLineEdit(chessboardGroup);
        chessboardCellWidthValue->setObjectName("chessboardCellWidthValue");
        chessboardCellWidthValue->setMinimumSize(QSize(60, 0));
        chessboardCellWidthValue->setMaximumSize(QSize(60, 40));
        chessboardCellWidthValue->setEnabled(true);

        chessboardHLayout1->addWidget(chessboardCellWidthValue);

        chessboardCellHeightLabel = new QLabel(chessboardGroup);
        chessboardCellHeightLabel->setObjectName("chessboardCellHeightLabel");
        chessboardCellHeightLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        chessboardHLayout1->addWidget(chessboardCellHeightLabel);

        chessboardCellHeightValue = new QLineEdit(chessboardGroup);
        chessboardCellHeightValue->setObjectName("chessboardCellHeightValue");
        chessboardCellHeightValue->setMinimumSize(QSize(60, 0));
        chessboardCellHeightValue->setMaximumSize(QSize(60, 40));
        chessboardCellHeightValue->setEnabled(true);

        chessboardHLayout1->addWidget(chessboardCellHeightValue);


        chessboardGroupVLayout->addLayout(chessboardHLayout1);

        chessboardHLayout2 = new QHBoxLayout();
        chessboardHLayout2->setSpacing(5);
        chessboardHLayout2->setContentsMargins(1, 1, 1, 1);
        chessboardHLayout2->setObjectName("chessboardHLayout2");
        chessboardFill2 = new QLabel(chessboardGroup);
        chessboardFill2->setObjectName("chessboardFill2");
        sizePolicy1.setHeightForWidth(chessboardFill2->sizePolicy().hasHeightForWidth());
        chessboardFill2->setSizePolicy(sizePolicy1);
        chessboardFill2->setAlignment(Qt::AlignCenter);

        chessboardHLayout2->addWidget(chessboardFill2);

        chessboardsCombo = new QComboBox(chessboardGroup);
        chessboardsCombo->setObjectName("chessboardsCombo");
        chessboardsCombo->setMinimumSize(QSize(200, 0));
        chessboardsCombo->setMaximumSize(QSize(200, 25));

        chessboardHLayout2->addWidget(chessboardsCombo);

        chessboardListAddButton = new QPushButton(chessboardGroup);
        chessboardListAddButton->setObjectName("chessboardListAddButton");

        chessboardHLayout2->addWidget(chessboardListAddButton);

        chessboardListDeleteButton = new QPushButton(chessboardGroup);
        chessboardListDeleteButton->setObjectName("chessboardListDeleteButton");

        chessboardHLayout2->addWidget(chessboardListDeleteButton);


        chessboardGroupVLayout->addLayout(chessboardHLayout2);

        chessboardHLayout3 = new QHBoxLayout();
        chessboardHLayout3->setSpacing(5);
        chessboardHLayout3->setContentsMargins(1, 1, 1, 1);
        chessboardHLayout3->setObjectName("chessboardHLayout3");
        chessboardDrawPointLine0 = new QLineEdit(chessboardGroup);
        chessboardDrawPointLine0->setObjectName("chessboardDrawPointLine0");

        chessboardHLayout3->addWidget(chessboardDrawPointLine0);

        chessboardDrawPointLine1 = new QLineEdit(chessboardGroup);
        chessboardDrawPointLine1->setObjectName("chessboardDrawPointLine1");

        chessboardHLayout3->addWidget(chessboardDrawPointLine1);

        chessboardDrawPointLine2 = new QLineEdit(chessboardGroup);
        chessboardDrawPointLine2->setObjectName("chessboardDrawPointLine2");

        chessboardHLayout3->addWidget(chessboardDrawPointLine2);

        chessboardDrawPointCheck = new QCheckBox(chessboardGroup);
        chessboardDrawPointCheck->setObjectName("chessboardDrawPointCheck");

        chessboardHLayout3->addWidget(chessboardDrawPointCheck);


        chessboardGroupVLayout->addLayout(chessboardHLayout3);


        panelCalibrationVerticalLayout->addWidget(chessboardGroup);

        cameraGroupCheck = new QCheckBox(PanelCalibrate);
        cameraGroupCheck->setObjectName("cameraGroupCheck");
        cameraGroupCheck->setMinimumSize(QSize(100, 22));
        cameraGroupCheck->setChecked(true);

        panelCalibrationVerticalLayout->addWidget(cameraGroupCheck);

        cameraGroup = new QGroupBox(PanelCalibrate);
        cameraGroup->setObjectName("cameraGroup");
        cameraGroup->setFlat(true);
        cameraCalibrateLayout = new QGridLayout(cameraGroup);
        cameraCalibrateLayout->setSpacing(2);
        cameraCalibrateLayout->setContentsMargins(0, 0, 0, 0);
        cameraCalibrateLayout->setObjectName("cameraCalibrateLayout");
        chessboardDetectAndCalibrateButton = new QPushButton(cameraGroup);
        chessboardDetectAndCalibrateButton->setObjectName("chessboardDetectAndCalibrateButton");
        chessboardDetectAndCalibrateButton->setCheckable(true);

        cameraCalibrateLayout->addWidget(chessboardDetectAndCalibrateButton, 0, 0, 1, 3);

        clearCalibrationButton = new QPushButton(cameraGroup);
        clearCalibrationButton->setObjectName("clearCalibrationButton");
        clearCalibrationButton->setCheckable(false);

        cameraCalibrateLayout->addWidget(clearCalibrationButton, 0, 3, 1, 1);

        chesboardDetectedLabel = new QLabel(cameraGroup);
        chesboardDetectedLabel->setObjectName("chesboardDetectedLabel");
        chesboardDetectedLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        cameraCalibrateLayout->addWidget(chesboardDetectedLabel, 1, 0, 1, 1);

        chesboardDetectedLine = new QLineEdit(cameraGroup);
        chesboardDetectedLine->setObjectName("chesboardDetectedLine");
        chesboardDetectedLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        chesboardDetectedLine->setReadOnly(true);

        cameraCalibrateLayout->addWidget(chesboardDetectedLine, 1, 1, 1, 1);

        drawCornersCheck = new QCheckBox(cameraGroup);
        drawCornersCheck->setObjectName("drawCornersCheck");

        cameraCalibrateLayout->addWidget(drawCornersCheck, 2, 0, 1, 2);

        drawChessboardPosesCheck = new QCheckBox(cameraGroup);
        drawChessboardPosesCheck->setObjectName("drawChessboardPosesCheck");

        cameraCalibrateLayout->addWidget(drawChessboardPosesCheck, 2, 2, 1, 2);

        KLabel = new QLabel(cameraGroup);
        KLabel->setObjectName("KLabel");
        KLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(KLabel->sizePolicy().hasHeightForWidth());
        KLabel->setSizePolicy(sizePolicy3);

        cameraCalibrateLayout->addWidget(KLabel, 4, 0, 1, 1);

        K00Line = new QLineEdit(cameraGroup);
        K00Line->setObjectName("K00Line");
        K00Line->setReadOnly(true);
        sizePolicy3.setHeightForWidth(K00Line->sizePolicy().hasHeightForWidth());
        K00Line->setSizePolicy(sizePolicy3);

        cameraCalibrateLayout->addWidget(K00Line, 4, 1, 1, 1);

        K01Line = new QLineEdit(cameraGroup);
        K01Line->setObjectName("K01Line");
        K01Line->setReadOnly(true);
        sizePolicy3.setHeightForWidth(K01Line->sizePolicy().hasHeightForWidth());
        K01Line->setSizePolicy(sizePolicy3);

        cameraCalibrateLayout->addWidget(K01Line, 4, 2, 1, 1);

        K02Line = new QLineEdit(cameraGroup);
        K02Line->setObjectName("K02Line");
        K02Line->setReadOnly(true);
        sizePolicy3.setHeightForWidth(K02Line->sizePolicy().hasHeightForWidth());
        K02Line->setSizePolicy(sizePolicy3);

        cameraCalibrateLayout->addWidget(K02Line, 4, 3, 1, 1);

        K10Line = new QLineEdit(cameraGroup);
        K10Line->setObjectName("K10Line");
        K10Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K10Line, 5, 1, 1, 1);

        K11Line = new QLineEdit(cameraGroup);
        K11Line->setObjectName("K11Line");
        K11Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K11Line, 5, 2, 1, 1);

        K12Line = new QLineEdit(cameraGroup);
        K12Line->setObjectName("K12Line");
        K12Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K12Line, 5, 3, 1, 1);

        K20Line = new QLineEdit(cameraGroup);
        K20Line->setObjectName("K20Line");
        K20Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K20Line, 6, 1, 1, 1);

        K21Line = new QLineEdit(cameraGroup);
        K21Line->setObjectName("K21Line");
        K21Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K21Line, 6, 2, 1, 1);

        K22Line = new QLineEdit(cameraGroup);
        K22Line->setObjectName("K22Line");
        K22Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(K22Line, 6, 3, 1, 1);

        kcLabel = new QLabel(cameraGroup);
        kcLabel->setObjectName("kcLabel");
        kcLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        cameraCalibrateLayout->addWidget(kcLabel, 7, 0, 1, 1);

        kc0Line = new QLineEdit(cameraGroup);
        kc0Line->setObjectName("kc0Line");
        kc0Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(kc0Line, 7, 1, 1, 1);

        kc1Line = new QLineEdit(cameraGroup);
        kc1Line->setObjectName("kc1Line");
        kc1Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(kc1Line, 7, 2, 1, 1);

        kc2Line = new QLineEdit(cameraGroup);
        kc2Line->setObjectName("kc2Line");
        kc2Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(kc2Line, 8, 1, 1, 1);

        kc3Line = new QLineEdit(cameraGroup);
        kc3Line->setObjectName("kc3Line");
        kc3Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(kc3Line, 8, 2, 1, 1);

        kc4Line = new QLineEdit(cameraGroup);
        kc4Line->setObjectName("kc4Line");
        kc4Line->setReadOnly(true);

        cameraCalibrateLayout->addWidget(kc4Line, 8, 3, 1, 1);

        calibErrorLabel = new QLabel(cameraGroup);
        calibErrorLabel->setObjectName("calibErrorLabel");
        calibErrorLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        cameraCalibrateLayout->addWidget(calibErrorLabel, 9, 0, 1, 1);

        calibErrorLine = new QLineEdit(cameraGroup);
        calibErrorLine->setObjectName("calibErrorLine");
        calibErrorLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibErrorLine->setReadOnly(true);

        cameraCalibrateLayout->addWidget(calibErrorLine, 9, 1, 1, 1);


        panelCalibrationVerticalLayout->addWidget(cameraGroup);

        turntableGroupCheck = new QCheckBox(PanelCalibrate);
        turntableGroupCheck->setObjectName("turntableGroupCheck");
        turntableGroupCheck->setMinimumSize(QSize(100, 22));
        turntableGroupCheck->setChecked(true);

        panelCalibrationVerticalLayout->addWidget(turntableGroupCheck);

        turntableGroup = new QGroupBox(PanelCalibrate);
        turntableGroup->setObjectName("turntableGroup");
        turntableGroup->setFlat(true);
        turntableGroupGridLayout = new QGridLayout(turntableGroup);
        turntableGroupGridLayout->setSpacing(2);
        turntableGroupGridLayout->setContentsMargins(0, 0, 0, 0);
        turntableGroupGridLayout->setObjectName("turntableGroupGridLayout");
        turntableFrameRangeLabel = new QLabel(turntableGroup);
        turntableFrameRangeLabel->setObjectName("turntableFrameRangeLabel");
        turntableFrameRangeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        turntableGroupGridLayout->addWidget(turntableFrameRangeLabel, 0, 0, 1, 1);

        indxMinSpin = new QSpinBox(turntableGroup);
        indxMinSpin->setObjectName("indxMinSpin");
        indxMinSpin->setValue(0);

        turntableGroupGridLayout->addWidget(indxMinSpin, 0, 1, 1, 1);

        indxMaxSpin = new QSpinBox(turntableGroup);
        indxMaxSpin->setObjectName("indxMaxSpin");
        indxMaxSpin->setValue(0);

        turntableGroupGridLayout->addWidget(indxMaxSpin, 0, 2, 1, 1);

        turntableCalibrateButton = new QPushButton(turntableGroup);
        turntableCalibrateButton->setObjectName("turntableCalibrateButton");

        turntableGroupGridLayout->addWidget(turntableCalibrateButton, 0, 3, 1, 1);

        turntablePatternCenterLabel = new QLabel(turntableGroup);
        turntablePatternCenterLabel->setObjectName("turntablePatternCenterLabel");
        turntablePatternCenterLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        turntableGroupGridLayout->addWidget(turntablePatternCenterLabel, 2, 0, 1, 1);

        turntablePatternCenter0 = new QLineEdit(turntableGroup);
        turntablePatternCenter0->setObjectName("turntablePatternCenter0");
        turntablePatternCenter0->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntablePatternCenter0, 2, 1, 1, 1);

        turntablePatternCenter1 = new QLineEdit(turntableGroup);
        turntablePatternCenter1->setObjectName("turntablePatternCenter1");
        turntablePatternCenter1->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntablePatternCenter1, 2, 2, 1, 1);

        turntablePatternCenter2 = new QLineEdit(turntableGroup);
        turntablePatternCenter2->setObjectName("turntablePatternCenter2");
        turntablePatternCenter2->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntablePatternCenter2, 2, 3, 1, 1);

        turntableCenterLabel = new QLabel(turntableGroup);
        turntableCenterLabel->setObjectName("turntableCenterLabel");
        turntableCenterLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        turntableGroupGridLayout->addWidget(turntableCenterLabel, 3, 0, 1, 1);

        turntableCenter0 = new QLineEdit(turntableGroup);
        turntableCenter0->setObjectName("turntableCenter0");
        turntableCenter0->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableCenter0, 3, 1, 1, 1);

        turntableCenter1 = new QLineEdit(turntableGroup);
        turntableCenter1->setObjectName("turntableCenter1");
        turntableCenter1->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableCenter1, 3, 2, 1, 1);

        turntableCenter2 = new QLineEdit(turntableGroup);
        turntableCenter2->setObjectName("turntableCenter2");
        turntableCenter2->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableCenter2, 3, 3, 1, 1);

        turntableAxisLabel = new QLabel(turntableGroup);
        turntableAxisLabel->setObjectName("turntableAxisLabel");
        turntableAxisLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        turntableGroupGridLayout->addWidget(turntableAxisLabel, 4, 0, 1, 1);

        turntableAxis0 = new QLineEdit(turntableGroup);
        turntableAxis0->setObjectName("turntableAxis0");
        turntableAxis0->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableAxis0, 4, 1, 1, 1);

        turntableAxis1 = new QLineEdit(turntableGroup);
        turntableAxis1->setObjectName("turntableAxis1");
        turntableAxis1->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableAxis1, 4, 2, 1, 1);

        turntableAxis2 = new QLineEdit(turntableGroup);
        turntableAxis2->setObjectName("turntableAxis2");
        turntableAxis2->setReadOnly(true);

        turntableGroupGridLayout->addWidget(turntableAxis2, 4, 3, 1, 1);

        drawTurntableFrameCheck = new QCheckBox(turntableGroup);
        drawTurntableFrameCheck->setObjectName("drawTurntableFrameCheck");
        drawTurntableFrameCheck->setChecked(false);

        turntableGroupGridLayout->addWidget(drawTurntableFrameCheck, 5, 0, 1, 2);

        anglePerFrameLabel = new QLabel(turntableGroup);
        anglePerFrameLabel->setObjectName("anglePerFrameLabel");

        turntableGroupGridLayout->addWidget(anglePerFrameLabel, 5, 2, 1, 1);

        anglePerFrameLine = new QLineEdit(turntableGroup);
        anglePerFrameLine->setObjectName("anglePerFrameLine");
        anglePerFrameLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        turntableGroupGridLayout->addWidget(anglePerFrameLine, 5, 3, 1, 1);


        panelCalibrationVerticalLayout->addWidget(turntableGroup);

        lasersGroupCheck = new QCheckBox(PanelCalibrate);
        lasersGroupCheck->setObjectName("lasersGroupCheck");
        lasersGroupCheck->setMinimumSize(QSize(100, 22));
        lasersGroupCheck->setChecked(true);

        panelCalibrationVerticalLayout->addWidget(lasersGroupCheck);

        lasersGroup = new QGroupBox(PanelCalibrate);
        lasersGroup->setObjectName("lasersGroup");
        lasersGroup->setFlat(true);
        gridLayoutLaser = new QGridLayout(lasersGroup);
        gridLayoutLaser->setSpacing(5);
        gridLayoutLaser->setObjectName("gridLayoutLaser");
        gridLayoutLaser->setContentsMargins(0, 5, 0, 5);
        drawLaserDetectedPointsCheck = new QCheckBox(lasersGroup);
        drawLaserDetectedPointsCheck->setObjectName("drawLaserDetectedPointsCheck");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(drawLaserDetectedPointsCheck->sizePolicy().hasHeightForWidth());
        drawLaserDetectedPointsCheck->setSizePolicy(sizePolicy4);

        gridLayoutLaser->addWidget(drawLaserDetectedPointsCheck, 0, 0, 1, 2);

        drawLaserBoundingQuadCheck = new QCheckBox(lasersGroup);
        drawLaserBoundingQuadCheck->setObjectName("drawLaserBoundingQuadCheck");
        sizePolicy4.setHeightForWidth(drawLaserBoundingQuadCheck->sizePolicy().hasHeightForWidth());
        drawLaserBoundingQuadCheck->setSizePolicy(sizePolicy4);

        gridLayoutLaser->addWidget(drawLaserBoundingQuadCheck, 0, 2, 1, 2);

        drawLaserTriangulatedPointsCheck = new QCheckBox(lasersGroup);
        drawLaserTriangulatedPointsCheck->setObjectName("drawLaserTriangulatedPointsCheck");

        gridLayoutLaser->addWidget(drawLaserTriangulatedPointsCheck, 1, 0, 1, 2);

        drawLaserPlaneCheck = new QCheckBox(lasersGroup);
        drawLaserPlaneCheck->setObjectName("drawLaserPlaneCheck");

        gridLayoutLaser->addWidget(drawLaserPlaneCheck, 1, 2, 1, 2);

        laserEstimatePoseDetectButton = new QPushButton(lasersGroup);
        laserEstimatePoseDetectButton->setObjectName("laserEstimatePoseDetectButton");
        laserEstimatePoseDetectButton->setCheckable(true);

        gridLayoutLaser->addWidget(laserEstimatePoseDetectButton, 3, 0, 1, 2);

        laserTriangulateButton = new QPushButton(lasersGroup);
        laserTriangulateButton->setObjectName("laserTriangulateButton");
        laserTriangulateButton->setCheckable(true);

        gridLayoutLaser->addWidget(laserTriangulateButton, 3, 2, 1, 2);

        laserFitPlanesButton = new QPushButton(lasersGroup);
        laserFitPlanesButton->setObjectName("laserFitPlanesButton");

        gridLayoutLaser->addWidget(laserFitPlanesButton, 4, 0, 1, 2);

        gridLayoutLaserPlanes = new QGridLayout();
        gridLayoutLaserPlanes->setSpacing(5);
        gridLayoutLaserPlanes->setContentsMargins(0, 0, 0, 0);
        gridLayoutLaserPlanes->setObjectName("gridLayoutLaserPlanes");
        laserPlane1Label = new QLabel(lasersGroup);
        laserPlane1Label->setObjectName("laserPlane1Label");
        laserPlane1Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutLaserPlanes->addWidget(laserPlane1Label, 0, 0, 1, 1);

        laserPlane10 = new QLineEdit(lasersGroup);
        laserPlane10->setObjectName("laserPlane10");
        laserPlane10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane10->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane10, 0, 1, 1, 1);

        laserPlane11 = new QLineEdit(lasersGroup);
        laserPlane11->setObjectName("laserPlane11");
        laserPlane11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane11->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane11, 0, 2, 1, 1);

        laserPlane12 = new QLineEdit(lasersGroup);
        laserPlane12->setObjectName("laserPlane12");
        laserPlane12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane12->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane12, 0, 3, 1, 1);

        laserPlane13 = new QLineEdit(lasersGroup);
        laserPlane13->setObjectName("laserPlane13");
        laserPlane13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane13->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane13, 0, 4, 1, 1);

        laserPlane2Label = new QLabel(lasersGroup);
        laserPlane2Label->setObjectName("laserPlane2Label");
        laserPlane2Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutLaserPlanes->addWidget(laserPlane2Label, 1, 0, 1, 1);

        laserPlane20 = new QLineEdit(lasersGroup);
        laserPlane20->setObjectName("laserPlane20");
        laserPlane20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane20->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane20, 1, 1, 1, 1);

        laserPlane21 = new QLineEdit(lasersGroup);
        laserPlane21->setObjectName("laserPlane21");
        laserPlane21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane21->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane21, 1, 2, 1, 1);

        laserPlane22 = new QLineEdit(lasersGroup);
        laserPlane22->setObjectName("laserPlane22");
        laserPlane22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane22->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane22, 1, 3, 1, 1);

        laserPlane23 = new QLineEdit(lasersGroup);
        laserPlane23->setObjectName("laserPlane23");
        laserPlane23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        laserPlane23->setReadOnly(true);

        gridLayoutLaserPlanes->addWidget(laserPlane23, 1, 4, 1, 1);


        gridLayoutLaser->addLayout(gridLayoutLaserPlanes, 5, 0, 1, 4);


        panelCalibrationVerticalLayout->addWidget(lasersGroup);

        verticalSpacerPanelCalibrate = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        panelCalibrationVerticalLayout->addItem(verticalSpacerPanelCalibrate);


        retranslateUi(PanelCalibrate);

        QMetaObject::connectSlotsByName(PanelCalibrate);
    } // setupUi

    void retranslateUi(QWidget *PanelCalibrate)
    {
        imagesGroupCheck->setText(QCoreApplication::translate("PanelCalibrate", "Images", nullptr));
        imagesUpdateButton->setText(QCoreApplication::translate("PanelCalibrate", "Update", nullptr));
        imagesFiller1->setText(QString());
        imagesPreviousButton->setText(QCoreApplication::translate("PanelCalibrate", "Up", nullptr));
        imagesNextButton->setText(QCoreApplication::translate("PanelCalibrate", "Down", nullptr));
        imageCountLabel->setText(QCoreApplication::translate("PanelCalibrate", "Count", nullptr));
        imageSizeLabel->setText(QCoreApplication::translate("PanelCalibrate", "Size", nullptr));
        imagesFiller2->setText(QString());
        imageRotLabel->setText(QCoreApplication::translate("PanelCalibrate", "Rot", nullptr));
        chessboardGroupCheck->setText(QCoreApplication::translate("PanelCalibrate", "Chessboard", nullptr));
        chessboardFill1->setText(QString());
        chessboardColsLabel->setText(QCoreApplication::translate("PanelCalibrate", "nX", nullptr));
        chessboardRowsLabel->setText(QCoreApplication::translate("PanelCalibrate", "nY", nullptr));
        chessboardCellWidthLabel->setText(QCoreApplication::translate("PanelCalibrate", "dX", nullptr));
        chessboardCellWidthValue->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        chessboardCellHeightLabel->setText(QCoreApplication::translate("PanelCalibrate", "dY", nullptr));
        chessboardCellHeightValue->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        chessboardFill2->setText(QString());
        chessboardListAddButton->setText(QCoreApplication::translate("PanelCalibrate", "+", nullptr));
        chessboardListDeleteButton->setText(QCoreApplication::translate("PanelCalibrate", "-", nullptr));
        chessboardDrawPointLine0->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        chessboardDrawPointLine1->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        chessboardDrawPointLine2->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        chessboardDrawPointCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Point", nullptr));
        cameraGroupCheck->setText(QCoreApplication::translate("PanelCalibrate", "Camera Calibration", nullptr));
        chessboardDetectAndCalibrateButton->setText(QCoreApplication::translate("PanelCalibrate", "Detect And Calibrate", nullptr));
        clearCalibrationButton->setText(QCoreApplication::translate("PanelCalibrate", "Clear", nullptr));
        chesboardDetectedLabel->setText(QCoreApplication::translate("PanelCalibrate", "Detected ", nullptr));
        drawCornersCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Detected Corners", nullptr));
        drawChessboardPosesCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Chessboard Pose", nullptr));
        KLabel->setText(QCoreApplication::translate("PanelCalibrate", "K ", nullptr));
        kcLabel->setText(QCoreApplication::translate("PanelCalibrate", "kc ", nullptr));
        calibErrorLabel->setText(QCoreApplication::translate("PanelCalibrate", "Err ", nullptr));
        turntableGroupCheck->setText(QCoreApplication::translate("PanelCalibrate", "Turntable Calibration", nullptr));
        turntableFrameRangeLabel->setText(QCoreApplication::translate("PanelCalibrate", "Frame Range ", nullptr));
        turntableCalibrateButton->setText(QCoreApplication::translate("PanelCalibrate", "Calibrate", nullptr));
        turntablePatternCenterLabel->setText(QCoreApplication::translate("PanelCalibrate", "Pattern Center", nullptr));
        turntablePatternCenter0->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntablePatternCenter1->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntablePatternCenter2->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableCenterLabel->setText(QCoreApplication::translate("PanelCalibrate", "Camera Center ", nullptr));
        turntableCenter0->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableCenter1->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableCenter2->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableAxisLabel->setText(QCoreApplication::translate("PanelCalibrate", "Axis of Rotation", nullptr));
        turntableAxis0->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableAxis1->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        turntableAxis2->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        drawTurntableFrameCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Turntable Frame", nullptr));
        anglePerFrameLabel->setText(QCoreApplication::translate("PanelCalibrate", "Frame Angle", nullptr));
        anglePerFrameLine->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        lasersGroupCheck->setText(QCoreApplication::translate("PanelCalibrate", "Laser Plane Calibration", nullptr));
        drawLaserDetectedPointsCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Detected Points", nullptr));
        drawLaserBoundingQuadCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Bounding Quad", nullptr));
        drawLaserTriangulatedPointsCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Triangulated Points", nullptr));
        drawLaserPlaneCheck->setText(QCoreApplication::translate("PanelCalibrate", "Draw Laser Plane", nullptr));
        laserEstimatePoseDetectButton->setText(QCoreApplication::translate("PanelCalibrate", "Estimate Pose and Detect", nullptr));
        laserTriangulateButton->setText(QCoreApplication::translate("PanelCalibrate", "Triangulate", nullptr));
        laserFitPlanesButton->setText(QCoreApplication::translate("PanelCalibrate", "Fit Planes", nullptr));
        laserPlane1Label->setText(QCoreApplication::translate("PanelCalibrate", "Laser Plane 1", nullptr));
        laserPlane10->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane11->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane12->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane13->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane2Label->setText(QCoreApplication::translate("PanelCalibrate", "Laser Plane 2", nullptr));
        laserPlane20->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane21->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane22->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        laserPlane23->setText(QCoreApplication::translate("PanelCalibrate", "0", nullptr));
        (void)PanelCalibrate;
    } // retranslateUi

};

namespace Ui {
    class PanelCalibrate: public Ui_PanelCalibrate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANELCALIBRATE_H
