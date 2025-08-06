/********************************************************************************
** Form generated from reading UI file 'PanelScan.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANELSCAN_H
#define UI_PANELSCAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ImgGLWidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_PanelScan
{
public:
    QVBoxLayout *verticalLayoutPanelScan;
    QLineEdit *statusBarLine;
    QProgressBar *scanningProgress;
    QCheckBox *bboxGroupCheck;
    QGroupBox *bboxGroup;
    QGridLayout *gridLayoutBboxGroup;
    QLabel *bboxP0Label;
    QLineEdit *bboxP0XLine;
    QLineEdit *bboxP0YLine;
    QLabel *bboxP3Label;
    QLineEdit *bboxP3XLine;
    QLineEdit *bboxP3YLine;
    QPushButton *bboxFileLoadButton;
    QLabel *bboxP1Label;
    QLineEdit *bboxP1XLine;
    QLineEdit *bboxP1YLine;
    QLabel *bboxP2Label;
    QLineEdit *bboxP2XLine;
    QLineEdit *bboxP2YLine;
    QPushButton *bboxFileSaveButton;
    QCheckBox *imagesGroupCheck;
    QGroupBox *imagesGroup;
    QGridLayout *gridLayoutImagesGroup;
    QPushButton *imagesUpdateButton;
    QPushButton *imagesPreviousButton;
    QPushButton *imagesNextButton;
    QLabel *imageRotLabel;
    QComboBox *imageRotCombo;
    QListView *imageList;
    QCheckBox *triangulationGroupCheck;
    QGroupBox *triangulationGroup;
    QVBoxLayout *verticalLayoutTriangulationGroup;
    QGridLayout *gridLayoutTriangulation;
    QLabel *triangulateChoiceLabel;
    QComboBox *triangulateChoiceCombo;
    QCheckBox *triangulateWithColorCheck;
    QLabel *anglePerFrameLabel;
    QLineEdit *anglePerFrameLine;
    QLabel *frameNumberLabel;
    QLineEdit *frameNumberLine;
    QPushButton *triangulationRunButton;
    QCheckBox *view3DGroupCheck;
    QGroupBox *view3DGroup;
    QVBoxLayout *verticalLayoutView3DGroup;
    QGridLayout *gridLayoutView3DGroup;
    QPushButton *wrlFileSaveButton;
    QLabel *view3DFillSpace_0;
    QSpacerItem *verticalSpacerPanelScan;

    void setupUi(QWidget *PanelScan)
    {
        if (PanelScan->objectName().isEmpty())
            PanelScan->setObjectName("PanelScan");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PanelScan->sizePolicy().hasHeightForWidth());
        PanelScan->setSizePolicy(sizePolicy);
        PanelScan->setMinimumSize(QSize(400, 100));
        PanelScan->setMaximumSize(QSize(400, 4000));
        verticalLayoutPanelScan = new QVBoxLayout(PanelScan);
        verticalLayoutPanelScan->setSpacing(1);
        verticalLayoutPanelScan->setContentsMargins(1, 1, 1, 1);
        verticalLayoutPanelScan->setObjectName("verticalLayoutPanelScan");
        statusBarLine = new QLineEdit(PanelScan);
        statusBarLine->setObjectName("statusBarLine");
        statusBarLine->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        statusBarLine->setReadOnly(true);

        verticalLayoutPanelScan->addWidget(statusBarLine);

        scanningProgress = new QProgressBar(PanelScan);
        scanningProgress->setObjectName("scanningProgress");
        scanningProgress->setValue(0);

        verticalLayoutPanelScan->addWidget(scanningProgress);

        bboxGroupCheck = new QCheckBox(PanelScan);
        bboxGroupCheck->setObjectName("bboxGroupCheck");
        bboxGroupCheck->setMinimumSize(QSize(100, 22));
        bboxGroupCheck->setChecked(true);

        verticalLayoutPanelScan->addWidget(bboxGroupCheck);

        bboxGroup = new QGroupBox(PanelScan);
        bboxGroup->setObjectName("bboxGroup");
        bboxGroup->setFlat(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(bboxGroup->sizePolicy().hasHeightForWidth());
        bboxGroup->setSizePolicy(sizePolicy1);
        gridLayoutBboxGroup = new QGridLayout(bboxGroup);
        gridLayoutBboxGroup->setSpacing(2);
        gridLayoutBboxGroup->setContentsMargins(5, 5, 5, 5);
        gridLayoutBboxGroup->setObjectName("gridLayoutBboxGroup");
        bboxP0Label = new QLabel(bboxGroup);
        bboxP0Label->setObjectName("bboxP0Label");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bboxP0Label->sizePolicy().hasHeightForWidth());
        bboxP0Label->setSizePolicy(sizePolicy2);
        bboxP0Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP0Label, 0, 0, 1, 1);

        bboxP0XLine = new QLineEdit(bboxGroup);
        bboxP0XLine->setObjectName("bboxP0XLine");
        sizePolicy2.setHeightForWidth(bboxP0XLine->sizePolicy().hasHeightForWidth());
        bboxP0XLine->setSizePolicy(sizePolicy2);
        bboxP0XLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP0XLine, 0, 1, 1, 1);

        bboxP0YLine = new QLineEdit(bboxGroup);
        bboxP0YLine->setObjectName("bboxP0YLine");
        sizePolicy2.setHeightForWidth(bboxP0YLine->sizePolicy().hasHeightForWidth());
        bboxP0YLine->setSizePolicy(sizePolicy2);
        bboxP0YLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP0YLine, 0, 2, 1, 1);

        bboxP3Label = new QLabel(bboxGroup);
        bboxP3Label->setObjectName("bboxP3Label");
        sizePolicy2.setHeightForWidth(bboxP3Label->sizePolicy().hasHeightForWidth());
        bboxP3Label->setSizePolicy(sizePolicy2);
        bboxP3Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP3Label, 0, 3, 1, 1);

        bboxP3XLine = new QLineEdit(bboxGroup);
        bboxP3XLine->setObjectName("bboxP3XLine");
        sizePolicy2.setHeightForWidth(bboxP3XLine->sizePolicy().hasHeightForWidth());
        bboxP3XLine->setSizePolicy(sizePolicy2);
        bboxP3XLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP3XLine, 0, 4, 1, 1);

        bboxP3YLine = new QLineEdit(bboxGroup);
        bboxP3YLine->setObjectName("bboxP3YLine");
        sizePolicy2.setHeightForWidth(bboxP3YLine->sizePolicy().hasHeightForWidth());
        bboxP3YLine->setSizePolicy(sizePolicy2);
        bboxP3YLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP3YLine, 0, 5, 1, 1);

        bboxFileLoadButton = new QPushButton(bboxGroup);
        bboxFileLoadButton->setObjectName("bboxFileLoadButton");
        sizePolicy2.setHeightForWidth(bboxFileLoadButton->sizePolicy().hasHeightForWidth());
        bboxFileLoadButton->setSizePolicy(sizePolicy2);

        gridLayoutBboxGroup->addWidget(bboxFileLoadButton, 0, 6, 1, 1);

        bboxP1Label = new QLabel(bboxGroup);
        bboxP1Label->setObjectName("bboxP1Label");
        sizePolicy2.setHeightForWidth(bboxP1Label->sizePolicy().hasHeightForWidth());
        bboxP1Label->setSizePolicy(sizePolicy2);
        bboxP1Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP1Label, 1, 0, 1, 1);

        bboxP1XLine = new QLineEdit(bboxGroup);
        bboxP1XLine->setObjectName("bboxP1XLine");
        sizePolicy2.setHeightForWidth(bboxP1XLine->sizePolicy().hasHeightForWidth());
        bboxP1XLine->setSizePolicy(sizePolicy2);
        bboxP1XLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP1XLine, 1, 1, 1, 1);

        bboxP1YLine = new QLineEdit(bboxGroup);
        bboxP1YLine->setObjectName("bboxP1YLine");
        sizePolicy2.setHeightForWidth(bboxP1YLine->sizePolicy().hasHeightForWidth());
        bboxP1YLine->setSizePolicy(sizePolicy2);
        bboxP1YLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP1YLine, 1, 2, 1, 1);

        bboxP2Label = new QLabel(bboxGroup);
        bboxP2Label->setObjectName("bboxP2Label");
        sizePolicy2.setHeightForWidth(bboxP2Label->sizePolicy().hasHeightForWidth());
        bboxP2Label->setSizePolicy(sizePolicy2);
        bboxP2Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP2Label, 1, 3, 1, 1);

        bboxP2XLine = new QLineEdit(bboxGroup);
        bboxP2XLine->setObjectName("bboxP2XLine");
        sizePolicy2.setHeightForWidth(bboxP2XLine->sizePolicy().hasHeightForWidth());
        bboxP2XLine->setSizePolicy(sizePolicy2);
        bboxP2XLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP2XLine, 1, 4, 1, 1);

        bboxP2YLine = new QLineEdit(bboxGroup);
        bboxP2YLine->setObjectName("bboxP2YLine");
        sizePolicy2.setHeightForWidth(bboxP2YLine->sizePolicy().hasHeightForWidth());
        bboxP2YLine->setSizePolicy(sizePolicy2);
        bboxP2YLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutBboxGroup->addWidget(bboxP2YLine, 1, 5, 1, 1);

        bboxFileSaveButton = new QPushButton(bboxGroup);
        bboxFileSaveButton->setObjectName("bboxFileSaveButton");
        sizePolicy2.setHeightForWidth(bboxFileSaveButton->sizePolicy().hasHeightForWidth());
        bboxFileSaveButton->setSizePolicy(sizePolicy2);

        gridLayoutBboxGroup->addWidget(bboxFileSaveButton, 1, 6, 1, 1);


        verticalLayoutPanelScan->addWidget(bboxGroup);

        imagesGroupCheck = new QCheckBox(PanelScan);
        imagesGroupCheck->setObjectName("imagesGroupCheck");
        imagesGroupCheck->setMinimumSize(QSize(100, 22));
        imagesGroupCheck->setChecked(false);

        verticalLayoutPanelScan->addWidget(imagesGroupCheck);

        imagesGroup = new QGroupBox(PanelScan);
        imagesGroup->setObjectName("imagesGroup");
        imagesGroup->setFlat(true);
        sizePolicy1.setHeightForWidth(imagesGroup->sizePolicy().hasHeightForWidth());
        imagesGroup->setSizePolicy(sizePolicy1);
        gridLayoutImagesGroup = new QGridLayout(imagesGroup);
        gridLayoutImagesGroup->setSpacing(5);
        gridLayoutImagesGroup->setContentsMargins(5, 5, 5, 5);
        gridLayoutImagesGroup->setObjectName("gridLayoutImagesGroup");
        imagesUpdateButton = new QPushButton(imagesGroup);
        imagesUpdateButton->setObjectName("imagesUpdateButton");
        imagesUpdateButton->setEnabled(true);
        imagesUpdateButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesUpdateButton, 0, 0, 1, 1);

        imagesPreviousButton = new QPushButton(imagesGroup);
        imagesPreviousButton->setObjectName("imagesPreviousButton");
        imagesPreviousButton->setEnabled(true);
        imagesPreviousButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesPreviousButton, 0, 2, 1, 1);

        imagesNextButton = new QPushButton(imagesGroup);
        imagesNextButton->setObjectName("imagesNextButton");
        imagesNextButton->setEnabled(true);
        imagesNextButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesNextButton, 0, 3, 1, 1);

        imageRotLabel = new QLabel(imagesGroup);
        imageRotLabel->setObjectName("imageRotLabel");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(imageRotLabel->sizePolicy().hasHeightForWidth());
        imageRotLabel->setSizePolicy(sizePolicy3);
        imageRotLabel->setMaximumSize(QSize(75, 22));
        imageRotLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutImagesGroup->addWidget(imageRotLabel, 0, 7, 1, 1);

        imageRotCombo = new QComboBox(imagesGroup);
        imageRotCombo->setObjectName("imageRotCombo");
        sizePolicy3.setHeightForWidth(imageRotCombo->sizePolicy().hasHeightForWidth());
        imageRotCombo->setSizePolicy(sizePolicy3);

        gridLayoutImagesGroup->addWidget(imageRotCombo, 0, 8, 1, 1);

        imageList = new QListView(imagesGroup);
        imageList->setObjectName("imageList");

        gridLayoutImagesGroup->addWidget(imageList, 2, 0, 1, 9);


        verticalLayoutPanelScan->addWidget(imagesGroup);

        triangulationGroupCheck = new QCheckBox(PanelScan);
        triangulationGroupCheck->setObjectName("triangulationGroupCheck");
        triangulationGroupCheck->setMinimumSize(QSize(100, 22));
        triangulationGroupCheck->setChecked(false);

        verticalLayoutPanelScan->addWidget(triangulationGroupCheck);

        triangulationGroup = new QGroupBox(PanelScan);
        triangulationGroup->setObjectName("triangulationGroup");
        triangulationGroup->setFlat(true);
        verticalLayoutTriangulationGroup = new QVBoxLayout(triangulationGroup);
        verticalLayoutTriangulationGroup->setSpacing(1);
        verticalLayoutTriangulationGroup->setContentsMargins(1, 1, 1, 1);
        verticalLayoutTriangulationGroup->setObjectName("verticalLayoutTriangulationGroup");
        gridLayoutTriangulation = new QGridLayout();
        gridLayoutTriangulation->setSpacing(5);
        gridLayoutTriangulation->setContentsMargins(5, 5, 5, 5);
        gridLayoutTriangulation->setObjectName("gridLayoutTriangulation");
        triangulateChoiceLabel = new QLabel(triangulationGroup);
        triangulateChoiceLabel->setObjectName("triangulateChoiceLabel");
        sizePolicy2.setHeightForWidth(triangulateChoiceLabel->sizePolicy().hasHeightForWidth());
        triangulateChoiceLabel->setSizePolicy(sizePolicy2);
        triangulateChoiceLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutTriangulation->addWidget(triangulateChoiceLabel, 0, 0, 1, 1);

        triangulateChoiceCombo = new QComboBox(triangulationGroup);
        triangulateChoiceCombo->setObjectName("triangulateChoiceCombo");
        sizePolicy3.setHeightForWidth(triangulateChoiceCombo->sizePolicy().hasHeightForWidth());
        triangulateChoiceCombo->setSizePolicy(sizePolicy3);

        gridLayoutTriangulation->addWidget(triangulateChoiceCombo, 0, 1, 1, 1);

        triangulateWithColorCheck = new QCheckBox(triangulationGroup);
        triangulateWithColorCheck->setObjectName("triangulateWithColorCheck");
        sizePolicy2.setHeightForWidth(triangulateWithColorCheck->sizePolicy().hasHeightForWidth());
        triangulateWithColorCheck->setSizePolicy(sizePolicy2);

        gridLayoutTriangulation->addWidget(triangulateWithColorCheck, 0, 2, 1, 2);

        anglePerFrameLabel = new QLabel(triangulationGroup);
        anglePerFrameLabel->setObjectName("anglePerFrameLabel");
        sizePolicy2.setHeightForWidth(anglePerFrameLabel->sizePolicy().hasHeightForWidth());
        anglePerFrameLabel->setSizePolicy(sizePolicy2);
        anglePerFrameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutTriangulation->addWidget(anglePerFrameLabel, 1, 0, 1, 1);

        anglePerFrameLine = new QLineEdit(triangulationGroup);
        anglePerFrameLine->setObjectName("anglePerFrameLine");
        sizePolicy2.setHeightForWidth(anglePerFrameLine->sizePolicy().hasHeightForWidth());
        anglePerFrameLine->setSizePolicy(sizePolicy2);
        anglePerFrameLine->setReadOnly(true);
        anglePerFrameLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutTriangulation->addWidget(anglePerFrameLine, 1, 1, 1, 1);

        frameNumberLabel = new QLabel(triangulationGroup);
        frameNumberLabel->setObjectName("frameNumberLabel");
        sizePolicy2.setHeightForWidth(frameNumberLabel->sizePolicy().hasHeightForWidth());
        frameNumberLabel->setSizePolicy(sizePolicy2);
        frameNumberLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutTriangulation->addWidget(frameNumberLabel, 1, 2, 1, 1);

        frameNumberLine = new QLineEdit(triangulationGroup);
        frameNumberLine->setObjectName("frameNumberLine");
        sizePolicy2.setHeightForWidth(frameNumberLine->sizePolicy().hasHeightForWidth());
        frameNumberLine->setSizePolicy(sizePolicy2);
        frameNumberLine->setReadOnly(true);
        frameNumberLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutTriangulation->addWidget(frameNumberLine, 1, 3, 1, 1);

        triangulationRunButton = new QPushButton(triangulationGroup);
        triangulationRunButton->setObjectName("triangulationRunButton");
        sizePolicy3.setHeightForWidth(triangulationRunButton->sizePolicy().hasHeightForWidth());
        triangulationRunButton->setSizePolicy(sizePolicy3);
        triangulationRunButton->setEnabled(true);
        triangulationRunButton->setCheckable(true);

        gridLayoutTriangulation->addWidget(triangulationRunButton, 2, 0, 1, 2);


        verticalLayoutTriangulationGroup->addLayout(gridLayoutTriangulation);


        verticalLayoutPanelScan->addWidget(triangulationGroup);

        view3DGroupCheck = new QCheckBox(PanelScan);
        view3DGroupCheck->setObjectName("view3DGroupCheck");
        view3DGroupCheck->setMinimumSize(QSize(100, 22));
        view3DGroupCheck->setChecked(false);

        verticalLayoutPanelScan->addWidget(view3DGroupCheck);

        view3DGroup = new QGroupBox(PanelScan);
        view3DGroup->setObjectName("view3DGroup");
        view3DGroup->setFlat(true);
        verticalLayoutView3DGroup = new QVBoxLayout(view3DGroup);
        verticalLayoutView3DGroup->setSpacing(1);
        verticalLayoutView3DGroup->setContentsMargins(1, 1, 1, 1);
        verticalLayoutView3DGroup->setObjectName("verticalLayoutView3DGroup");
        gridLayoutView3DGroup = new QGridLayout();
        gridLayoutView3DGroup->setSpacing(5);
        gridLayoutView3DGroup->setContentsMargins(5, 5, 5, 5);
        gridLayoutView3DGroup->setObjectName("gridLayoutView3DGroup");
        wrlFileSaveButton = new QPushButton(view3DGroup);
        wrlFileSaveButton->setObjectName("wrlFileSaveButton");
        sizePolicy2.setHeightForWidth(wrlFileSaveButton->sizePolicy().hasHeightForWidth());
        wrlFileSaveButton->setSizePolicy(sizePolicy2);

        gridLayoutView3DGroup->addWidget(wrlFileSaveButton, 0, 0, 1, 1);

        view3DFillSpace_0 = new QLabel(view3DGroup);
        view3DFillSpace_0->setObjectName("view3DFillSpace_0");
        sizePolicy2.setHeightForWidth(view3DFillSpace_0->sizePolicy().hasHeightForWidth());
        view3DFillSpace_0->setSizePolicy(sizePolicy2);
        view3DFillSpace_0->setAlignment(Qt::AlignCenter);

        gridLayoutView3DGroup->addWidget(view3DFillSpace_0, 0, 1, 1, 1);


        verticalLayoutView3DGroup->addLayout(gridLayoutView3DGroup);


        verticalLayoutPanelScan->addWidget(view3DGroup);

        verticalSpacerPanelScan = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutPanelScan->addItem(verticalSpacerPanelScan);


        retranslateUi(PanelScan);

        QMetaObject::connectSlotsByName(PanelScan);
    } // setupUi

    void retranslateUi(QWidget *PanelScan)
    {
        bboxGroupCheck->setText(QCoreApplication::translate("PanelScan", "Bounding Box", nullptr));
        bboxP0Label->setText(QCoreApplication::translate("PanelScan", "p0", nullptr));
        bboxP0XLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p0.x", nullptr));
        bboxP0YLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p0.y", nullptr));
        bboxP3Label->setText(QCoreApplication::translate("PanelScan", "p3", nullptr));
        bboxP3XLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p3.x", nullptr));
        bboxP3YLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p3.y", nullptr));
        bboxFileLoadButton->setText(QCoreApplication::translate("PanelScan", "Load", nullptr));
        bboxP1Label->setText(QCoreApplication::translate("PanelScan", "p1", nullptr));
        bboxP1XLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p1.x", nullptr));
        bboxP1YLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p1.y", nullptr));
        bboxP2Label->setText(QCoreApplication::translate("PanelScan", "p2", nullptr));
        bboxP2XLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p2.x", nullptr));
        bboxP2YLine->setPlaceholderText(QCoreApplication::translate("PanelScan", "p2.y", nullptr));
        bboxFileSaveButton->setText(QCoreApplication::translate("PanelScan", "Save", nullptr));
        imagesGroupCheck->setText(QCoreApplication::translate("PanelScan", "Images", nullptr));
        imagesUpdateButton->setText(QCoreApplication::translate("PanelScan", "Update", nullptr));
        imagesPreviousButton->setText(QCoreApplication::translate("PanelScan", "Previous", nullptr));
        imagesNextButton->setText(QCoreApplication::translate("PanelScan", "Next", nullptr));
        imageRotLabel->setText(QCoreApplication::translate("PanelScan", "Rot", nullptr));
        triangulationGroupCheck->setText(QCoreApplication::translate("PanelScan", "Triangulation", nullptr));
        triangulateChoiceLabel->setText(QCoreApplication::translate("PanelScan", "Triangulate ", nullptr));
        triangulateWithColorCheck->setText(QCoreApplication::translate("PanelScan", "With Color", nullptr));
        anglePerFrameLabel->setText(QCoreApplication::translate("PanelScan", "Angle Per Frame ", nullptr));
        frameNumberLabel->setText(QCoreApplication::translate("PanelScan", "Frame # ", nullptr));
        triangulationRunButton->setText(QCoreApplication::translate("PanelScan", "RUN", nullptr));
        view3DGroupCheck->setText(QCoreApplication::translate("PanelScan", "3D View", nullptr));
        wrlFileSaveButton->setText(QCoreApplication::translate("PanelScan", "Save Point Cloud", nullptr));
        view3DFillSpace_0->setText(QString());
        (void)PanelScan;
    } // retranslateUi

};

namespace Ui {
    class PanelScan: public Ui_PanelScan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANELSCAN_H
