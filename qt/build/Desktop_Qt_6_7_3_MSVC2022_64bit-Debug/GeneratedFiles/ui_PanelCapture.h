/********************************************************************************
** Form generated from reading UI file 'PanelCapture.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANELCAPTURE_H
#define UI_PANELCAPTURE_H

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
#include "ImgGLWidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_PanelCapture
{
public:
    QVBoxLayout *verticalLayoutPanelCapture;
    QProgressBar *progressBar;
    QCheckBox *scannerGroupCheck;
    QGroupBox *scannerGroup;
    QGridLayout *gridLayoutScannerGroup;
    QLabel *scannerHostnameLabel;
    QLineEdit *scannerHostname;
    QLabel *scannerIpAddressLabel;
    QLineEdit *scannerIpAddress;
    QLabel *scannerCommandLabel;
    QLineEdit *scannerCommandLine;
    QPushButton *scannerTestButton;
    QLabel *scannerResponseLabel;
    QLineEdit *scannerResponseLine;
    QCheckBox *cameraGroupCheck;
    QGroupBox *cameraGroup;
    QGridLayout *gridLayoutCamera;
    QLabel *pixelFormatLabel;
    QComboBox *pixelFormatCombo;
    QLabel *frameSizeLabel;
    QComboBox *frameSizeCombo;
    QLabel *imageGroupFillSpace;
    QPushButton *cameraPreviewButton;
    QCheckBox *captureGroupCheck;
    QGroupBox *captureGroup;
    QGridLayout *gridLayout_1;
    QLabel *stepsPerRevolutionLabel;
    QLineEdit *stepsPerRevolutionLine;
    QLabel *stepsPerFrameLabel;
    QSpinBox *stepsPerFrameSpin;
    QLabel *anglePerFrameLabel;
    QLineEdit *anglePerFrameLine;
    QLabel *countLabel;
    QSpinBox *countSpin;
    QLabel *imagesInFrameLabel;
    QHBoxLayout *imagesInFrameLayout;
    QCheckBox *blankFrameCheck;
    QCheckBox *laser1FrameCheck;
    QCheckBox *laser2FrameCheck;
    QLineEdit *imagesPerFrameLine;
    QCheckBox *serverSendImagesCheck;
    QCheckBox *serverSaveImagesCheck;
    QPushButton *captureButton;
    QCheckBox *imagesGroupCheck;
    QGroupBox *imagesGroup;
    QGridLayout *gridLayoutImagesGroup;
    QPushButton *imagesUpdateButton;
    QPushButton *imagesFirstButton;
    QPushButton *imagesPreviousButton;
    QPushButton *imagesNextButton;
    QPushButton *imagesLastButton;
    QLabel *imageRotLabel;
    QComboBox *imageRotCombo;
    QListView *imageList;
    QSpacerItem *verticalSpacerPanelCapture;

    void setupUi(QWidget *PanelCapture)
    {
        if (PanelCapture->objectName().isEmpty())
            PanelCapture->setObjectName("PanelCapture");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PanelCapture->sizePolicy().hasHeightForWidth());
        PanelCapture->setSizePolicy(sizePolicy);
        PanelCapture->setMinimumSize(QSize(400, 100));
        PanelCapture->setMaximumSize(QSize(400, 4000));
        verticalLayoutPanelCapture = new QVBoxLayout(PanelCapture);
        verticalLayoutPanelCapture->setSpacing(1);
        verticalLayoutPanelCapture->setContentsMargins(1, 1, 1, 1);
        verticalLayoutPanelCapture->setObjectName("verticalLayoutPanelCapture");
        progressBar = new QProgressBar(PanelCapture);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayoutPanelCapture->addWidget(progressBar);

        scannerGroupCheck = new QCheckBox(PanelCapture);
        scannerGroupCheck->setObjectName("scannerGroupCheck");
        scannerGroupCheck->setMinimumSize(QSize(100, 22));
        scannerGroupCheck->setChecked(false);

        verticalLayoutPanelCapture->addWidget(scannerGroupCheck);

        scannerGroup = new QGroupBox(PanelCapture);
        scannerGroup->setObjectName("scannerGroup");
        scannerGroup->setFlat(true);
        gridLayoutScannerGroup = new QGridLayout(scannerGroup);
        gridLayoutScannerGroup->setSpacing(5);
        gridLayoutScannerGroup->setContentsMargins(5, 5, 5, 5);
        gridLayoutScannerGroup->setObjectName("gridLayoutScannerGroup");
        scannerHostnameLabel = new QLabel(scannerGroup);
        scannerHostnameLabel->setObjectName("scannerHostnameLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scannerHostnameLabel->sizePolicy().hasHeightForWidth());
        scannerHostnameLabel->setSizePolicy(sizePolicy1);
        scannerHostnameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        scannerHostnameLabel->setMaximumSize(QSize(75, 22));

        gridLayoutScannerGroup->addWidget(scannerHostnameLabel, 0, 0, 1, 1);

        scannerHostname = new QLineEdit(scannerGroup);
        scannerHostname->setObjectName("scannerHostname");
        sizePolicy1.setHeightForWidth(scannerHostname->sizePolicy().hasHeightForWidth());
        scannerHostname->setSizePolicy(sizePolicy1);
        scannerHostname->setEnabled(true);
        scannerHostname->setFrame(false);
        scannerHostname->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        gridLayoutScannerGroup->addWidget(scannerHostname, 0, 1, 1, 2);

        scannerIpAddressLabel = new QLabel(scannerGroup);
        scannerIpAddressLabel->setObjectName("scannerIpAddressLabel");
        sizePolicy1.setHeightForWidth(scannerIpAddressLabel->sizePolicy().hasHeightForWidth());
        scannerIpAddressLabel->setSizePolicy(sizePolicy1);
        scannerIpAddressLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        scannerIpAddressLabel->setMaximumSize(QSize(75, 22));

        gridLayoutScannerGroup->addWidget(scannerIpAddressLabel, 0, 3, 1, 1);

        scannerIpAddress = new QLineEdit(scannerGroup);
        scannerIpAddress->setObjectName("scannerIpAddress");
        sizePolicy1.setHeightForWidth(scannerIpAddress->sizePolicy().hasHeightForWidth());
        scannerIpAddress->setSizePolicy(sizePolicy1);
        scannerIpAddress->setEnabled(true);
        scannerIpAddress->setFrame(false);
        scannerIpAddress->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        gridLayoutScannerGroup->addWidget(scannerIpAddress, 0, 4, 1, 2);

        scannerCommandLabel = new QLabel(scannerGroup);
        scannerCommandLabel->setObjectName("scannerCommandLabel");
        sizePolicy1.setHeightForWidth(scannerCommandLabel->sizePolicy().hasHeightForWidth());
        scannerCommandLabel->setSizePolicy(sizePolicy1);
        scannerCommandLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        scannerCommandLabel->setMaximumSize(QSize(75, 22));

        gridLayoutScannerGroup->addWidget(scannerCommandLabel, 2, 0, 1, 1);

        scannerCommandLine = new QLineEdit(scannerGroup);
        scannerCommandLine->setObjectName("scannerCommandLine");
        sizePolicy1.setHeightForWidth(scannerCommandLine->sizePolicy().hasHeightForWidth());
        scannerCommandLine->setSizePolicy(sizePolicy1);
        scannerCommandLine->setFrame(false);
        scannerCommandLine->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        scannerCommandLine->setReadOnly(true);

        gridLayoutScannerGroup->addWidget(scannerCommandLine, 2, 1, 1, 4);

        scannerTestButton = new QPushButton(scannerGroup);
        scannerTestButton->setObjectName("scannerTestButton");
        scannerTestButton->setEnabled(true);
        scannerTestButton->setCheckable(false);

        gridLayoutScannerGroup->addWidget(scannerTestButton, 2, 5, 1, 1);

        scannerResponseLabel = new QLabel(scannerGroup);
        scannerResponseLabel->setObjectName("scannerResponseLabel");
        sizePolicy1.setHeightForWidth(scannerResponseLabel->sizePolicy().hasHeightForWidth());
        scannerResponseLabel->setSizePolicy(sizePolicy1);
        scannerResponseLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        scannerResponseLabel->setMaximumSize(QSize(75, 22));

        gridLayoutScannerGroup->addWidget(scannerResponseLabel, 3, 0, 1, 1);

        scannerResponseLine = new QLineEdit(scannerGroup);
        scannerResponseLine->setObjectName("scannerResponseLine");
        sizePolicy1.setHeightForWidth(scannerResponseLine->sizePolicy().hasHeightForWidth());
        scannerResponseLine->setSizePolicy(sizePolicy1);
        scannerResponseLine->setFrame(false);
        scannerResponseLine->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        scannerResponseLine->setReadOnly(true);

        gridLayoutScannerGroup->addWidget(scannerResponseLine, 3, 1, 1, 5);


        verticalLayoutPanelCapture->addWidget(scannerGroup);

        cameraGroupCheck = new QCheckBox(PanelCapture);
        cameraGroupCheck->setObjectName("cameraGroupCheck");
        cameraGroupCheck->setMinimumSize(QSize(100, 22));
        cameraGroupCheck->setChecked(false);

        verticalLayoutPanelCapture->addWidget(cameraGroupCheck);

        cameraGroup = new QGroupBox(PanelCapture);
        cameraGroup->setObjectName("cameraGroup");
        cameraGroup->setFlat(true);
        gridLayoutCamera = new QGridLayout(cameraGroup);
        gridLayoutCamera->setContentsMargins(5, 5, 5, 5);
        gridLayoutCamera->setObjectName("gridLayoutCamera");
        gridLayoutCamera->setHorizontalSpacing(5);
        gridLayoutCamera->setVerticalSpacing(1);
        pixelFormatLabel = new QLabel(cameraGroup);
        pixelFormatLabel->setObjectName("pixelFormatLabel");
        sizePolicy1.setHeightForWidth(pixelFormatLabel->sizePolicy().hasHeightForWidth());
        pixelFormatLabel->setSizePolicy(sizePolicy1);
        pixelFormatLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutCamera->addWidget(pixelFormatLabel, 0, 0, 1, 1);

        pixelFormatCombo = new QComboBox(cameraGroup);
        pixelFormatCombo->setObjectName("pixelFormatCombo");
        sizePolicy1.setHeightForWidth(pixelFormatCombo->sizePolicy().hasHeightForWidth());
        pixelFormatCombo->setSizePolicy(sizePolicy1);

        gridLayoutCamera->addWidget(pixelFormatCombo, 0, 1, 1, 1);

        frameSizeLabel = new QLabel(cameraGroup);
        frameSizeLabel->setObjectName("frameSizeLabel");
        sizePolicy1.setHeightForWidth(frameSizeLabel->sizePolicy().hasHeightForWidth());
        frameSizeLabel->setSizePolicy(sizePolicy1);
        frameSizeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutCamera->addWidget(frameSizeLabel, 0, 2, 1, 1);

        frameSizeCombo = new QComboBox(cameraGroup);
        frameSizeCombo->setObjectName("frameSizeCombo");
        sizePolicy1.setHeightForWidth(frameSizeCombo->sizePolicy().hasHeightForWidth());
        frameSizeCombo->setSizePolicy(sizePolicy1);

        gridLayoutCamera->addWidget(frameSizeCombo, 0, 3, 1, 1);

        imageGroupFillSpace = new QLabel(cameraGroup);
        imageGroupFillSpace->setObjectName("imageGroupFillSpace");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(imageGroupFillSpace->sizePolicy().hasHeightForWidth());
        imageGroupFillSpace->setSizePolicy(sizePolicy2);
        imageGroupFillSpace->setAlignment(Qt::AlignCenter);

        gridLayoutCamera->addWidget(imageGroupFillSpace, 1, 0, 1, 2);

        cameraPreviewButton = new QPushButton(cameraGroup);
        cameraPreviewButton->setObjectName("cameraPreviewButton");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cameraPreviewButton->sizePolicy().hasHeightForWidth());
        cameraPreviewButton->setSizePolicy(sizePolicy3);
        cameraPreviewButton->setEnabled(true);
        cameraPreviewButton->setCheckable(true);

        gridLayoutCamera->addWidget(cameraPreviewButton, 1, 3, 1, 1);


        verticalLayoutPanelCapture->addWidget(cameraGroup);

        captureGroupCheck = new QCheckBox(PanelCapture);
        captureGroupCheck->setObjectName("captureGroupCheck");
        captureGroupCheck->setMinimumSize(QSize(100, 22));
        captureGroupCheck->setChecked(false);

        verticalLayoutPanelCapture->addWidget(captureGroupCheck);

        captureGroup = new QGroupBox(PanelCapture);
        captureGroup->setObjectName("captureGroup");
        captureGroup->setFlat(true);
        gridLayout_1 = new QGridLayout(captureGroup);
        gridLayout_1->setSpacing(5);
        gridLayout_1->setContentsMargins(5, 5, 5, 5);
        gridLayout_1->setObjectName("gridLayout_1");
        stepsPerRevolutionLabel = new QLabel(captureGroup);
        stepsPerRevolutionLabel->setObjectName("stepsPerRevolutionLabel");
        stepsPerRevolutionLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(stepsPerRevolutionLabel, 0, 0, 1, 2);

        stepsPerRevolutionLine = new QLineEdit(captureGroup);
        stepsPerRevolutionLine->setObjectName("stepsPerRevolutionLine");
        stepsPerRevolutionLine->setMaximumSize(QSize(100, 40));
        stepsPerRevolutionLine->setEnabled(true);
        stepsPerRevolutionLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(stepsPerRevolutionLine, 0, 2, 1, 1);

        stepsPerFrameLabel = new QLabel(captureGroup);
        stepsPerFrameLabel->setObjectName("stepsPerFrameLabel");
        stepsPerFrameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(stepsPerFrameLabel, 1, 0, 1, 2);

        stepsPerFrameSpin = new QSpinBox(captureGroup);
        stepsPerFrameSpin->setObjectName("stepsPerFrameSpin");
        stepsPerFrameSpin->setMaximumSize(QSize(100, 40));
        stepsPerFrameSpin->setValue(68);
        stepsPerFrameSpin->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(stepsPerFrameSpin, 1, 2, 1, 1);

        anglePerFrameLabel = new QLabel(captureGroup);
        anglePerFrameLabel->setObjectName("anglePerFrameLabel");
        anglePerFrameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(anglePerFrameLabel, 2, 0, 1, 2);

        anglePerFrameLine = new QLineEdit(captureGroup);
        anglePerFrameLine->setObjectName("anglePerFrameLine");
        anglePerFrameLine->setMaximumSize(QSize(100, 40));
        anglePerFrameLine->setEnabled(true);
        anglePerFrameLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(anglePerFrameLine, 2, 2, 1, 1);

        countLabel = new QLabel(captureGroup);
        countLabel->setObjectName("countLabel");
        countLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(countLabel, 3, 0, 1, 2);

        countSpin = new QSpinBox(captureGroup);
        countSpin->setObjectName("countSpin");
        countSpin->setMaximumSize(QSize(100, 40));
        countSpin->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(countSpin, 3, 2, 1, 1);

        imagesInFrameLabel = new QLabel(captureGroup);
        imagesInFrameLabel->setObjectName("imagesInFrameLabel");
        imagesInFrameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout_1->addWidget(imagesInFrameLabel, 4, 0, 1, 1);

        imagesInFrameLayout = new QHBoxLayout();
        imagesInFrameLayout->setObjectName("imagesInFrameLayout");
        blankFrameCheck = new QCheckBox(captureGroup);
        blankFrameCheck->setObjectName("blankFrameCheck");
        blankFrameCheck->setChecked(true);

        imagesInFrameLayout->addWidget(blankFrameCheck);

        laser1FrameCheck = new QCheckBox(captureGroup);
        laser1FrameCheck->setObjectName("laser1FrameCheck");
        laser1FrameCheck->setChecked(false);

        imagesInFrameLayout->addWidget(laser1FrameCheck);

        laser2FrameCheck = new QCheckBox(captureGroup);
        laser2FrameCheck->setObjectName("laser2FrameCheck");
        laser2FrameCheck->setChecked(false);

        imagesInFrameLayout->addWidget(laser2FrameCheck);

        imagesPerFrameLine = new QLineEdit(captureGroup);
        imagesPerFrameLine->setObjectName("imagesPerFrameLine");
        imagesPerFrameLine->setMaximumSize(QSize(30, 40));
        imagesPerFrameLine->setReadOnly(true);
        imagesPerFrameLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        imagesInFrameLayout->addWidget(imagesPerFrameLine);


        gridLayout_1->addLayout(imagesInFrameLayout, 4, 1, 1, 2);

        serverSendImagesCheck = new QCheckBox(captureGroup);
        serverSendImagesCheck->setObjectName("serverSendImagesCheck");
        serverSendImagesCheck->setChecked(true);

        gridLayout_1->addWidget(serverSendImagesCheck, 5, 0, 1, 1);

        serverSaveImagesCheck = new QCheckBox(captureGroup);
        serverSaveImagesCheck->setObjectName("serverSaveImagesCheck");
        serverSaveImagesCheck->setChecked(false);

        gridLayout_1->addWidget(serverSaveImagesCheck, 5, 1, 1, 1);

        captureButton = new QPushButton(captureGroup);
        captureButton->setObjectName("captureButton");
        captureButton->setMaximumSize(QSize(100, 40));
        captureButton->setEnabled(true);
        captureButton->setCheckable(true);

        gridLayout_1->addWidget(captureButton, 5, 2, 1, 1);


        verticalLayoutPanelCapture->addWidget(captureGroup);

        imagesGroupCheck = new QCheckBox(PanelCapture);
        imagesGroupCheck->setObjectName("imagesGroupCheck");
        imagesGroupCheck->setMinimumSize(QSize(100, 22));
        imagesGroupCheck->setChecked(false);

        verticalLayoutPanelCapture->addWidget(imagesGroupCheck);

        imagesGroup = new QGroupBox(PanelCapture);
        imagesGroup->setObjectName("imagesGroup");
        imagesGroup->setFlat(true);
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(imagesGroup->sizePolicy().hasHeightForWidth());
        imagesGroup->setSizePolicy(sizePolicy4);
        gridLayoutImagesGroup = new QGridLayout(imagesGroup);
        gridLayoutImagesGroup->setSpacing(5);
        gridLayoutImagesGroup->setContentsMargins(5, 5, 5, 5);
        gridLayoutImagesGroup->setObjectName("gridLayoutImagesGroup");
        imagesUpdateButton = new QPushButton(imagesGroup);
        imagesUpdateButton->setObjectName("imagesUpdateButton");
        imagesUpdateButton->setEnabled(true);
        imagesUpdateButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesUpdateButton, 0, 0, 1, 1);

        imagesFirstButton = new QPushButton(imagesGroup);
        imagesFirstButton->setObjectName("imagesFirstButton");
        imagesFirstButton->setEnabled(false);
        imagesFirstButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesFirstButton, 0, 1, 1, 1);

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

        imagesLastButton = new QPushButton(imagesGroup);
        imagesLastButton->setObjectName("imagesLastButton");
        imagesLastButton->setEnabled(false);
        imagesLastButton->setCheckable(false);

        gridLayoutImagesGroup->addWidget(imagesLastButton, 0, 4, 1, 1);

        imageRotLabel = new QLabel(imagesGroup);
        imageRotLabel->setObjectName("imageRotLabel");
        sizePolicy1.setHeightForWidth(imageRotLabel->sizePolicy().hasHeightForWidth());
        imageRotLabel->setSizePolicy(sizePolicy1);
        imageRotLabel->setMaximumSize(QSize(75, 22));
        imageRotLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayoutImagesGroup->addWidget(imageRotLabel, 0, 5, 1, 1);

        imageRotCombo = new QComboBox(imagesGroup);
        imageRotCombo->setObjectName("imageRotCombo");
        sizePolicy1.setHeightForWidth(imageRotCombo->sizePolicy().hasHeightForWidth());
        imageRotCombo->setSizePolicy(sizePolicy1);

        gridLayoutImagesGroup->addWidget(imageRotCombo, 0, 6, 1, 1);

        imageList = new QListView(imagesGroup);
        imageList->setObjectName("imageList");

        gridLayoutImagesGroup->addWidget(imageList, 1, 0, 1, 7);


        verticalLayoutPanelCapture->addWidget(imagesGroup);

        verticalSpacerPanelCapture = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutPanelCapture->addItem(verticalSpacerPanelCapture);


        retranslateUi(PanelCapture);

        QMetaObject::connectSlotsByName(PanelCapture);
    } // setupUi

    void retranslateUi(QWidget *PanelCapture)
    {
        scannerGroupCheck->setText(QCoreApplication::translate("PanelCapture", "3D Scanner", nullptr));
        scannerHostnameLabel->setText(QCoreApplication::translate("PanelCapture", "Hostname", nullptr));
        scannerHostname->setPlaceholderText(QCoreApplication::translate("PanelCapture", " Hostname", nullptr));
        scannerIpAddressLabel->setText(QCoreApplication::translate("PanelCapture", "IP Address ", nullptr));
        scannerIpAddress->setPlaceholderText(QCoreApplication::translate("PanelCapture", " IP Address", nullptr));
        scannerCommandLabel->setText(QCoreApplication::translate("PanelCapture", "Command", nullptr));
        scannerCommandLine->setPlaceholderText(QCoreApplication::translate("PanelCapture", " Command", nullptr));
        scannerTestButton->setText(QCoreApplication::translate("PanelCapture", "TIME", nullptr));
        scannerResponseLabel->setText(QCoreApplication::translate("PanelCapture", "Response", nullptr));
        scannerResponseLine->setPlaceholderText(QCoreApplication::translate("PanelCapture", " Response", nullptr));
        cameraGroupCheck->setText(QCoreApplication::translate("PanelCapture", "Camera", nullptr));
        pixelFormatLabel->setText(QCoreApplication::translate("PanelCapture", "PixelFormat", nullptr));
        frameSizeLabel->setText(QCoreApplication::translate("PanelCapture", "FrameSize", nullptr));
        imageGroupFillSpace->setText(QString());
        cameraPreviewButton->setText(QCoreApplication::translate("PanelCapture", "PREVIEW", nullptr));
        captureGroupCheck->setText(QCoreApplication::translate("PanelCapture", "Capture", nullptr));
        stepsPerRevolutionLabel->setText(QCoreApplication::translate("PanelCapture", "StepsPerRevolution", nullptr));
        stepsPerRevolutionLine->setText(QCoreApplication::translate("PanelCapture", "24480", nullptr));
        stepsPerFrameLabel->setText(QCoreApplication::translate("PanelCapture", "StepsPerFrame", nullptr));
        anglePerFrameLabel->setText(QCoreApplication::translate("PanelCapture", "AnglePerFrame (deg)", nullptr));
        anglePerFrameLine->setPlaceholderText(QCoreApplication::translate("PanelCapture", " Angle ", nullptr));
        countLabel->setText(QCoreApplication::translate("PanelCapture", "Number Of Frames", nullptr));
        imagesInFrameLabel->setText(QCoreApplication::translate("PanelCapture", "Images In Frame", nullptr));
        blankFrameCheck->setText(QCoreApplication::translate("PanelCapture", "Blank ", nullptr));
        laser1FrameCheck->setText(QCoreApplication::translate("PanelCapture", "Laser 1 ", nullptr));
        laser2FrameCheck->setText(QCoreApplication::translate("PanelCapture", "Laser 2 ", nullptr));
        imagesPerFrameLine->setText(QCoreApplication::translate("PanelCapture", "1", nullptr));
        serverSendImagesCheck->setText(QCoreApplication::translate("PanelCapture", "Send Images", nullptr));
        serverSaveImagesCheck->setText(QCoreApplication::translate("PanelCapture", "Save Images", nullptr));
        captureButton->setText(QCoreApplication::translate("PanelCapture", "CAPTURE", nullptr));
        imagesGroupCheck->setText(QCoreApplication::translate("PanelCapture", "Images", nullptr));
        imagesUpdateButton->setText(QCoreApplication::translate("PanelCapture", "Update", nullptr));
        imagesFirstButton->setText(QCoreApplication::translate("PanelCapture", "First", nullptr));
        imagesPreviousButton->setText(QCoreApplication::translate("PanelCapture", "Up", nullptr));
        imagesNextButton->setText(QCoreApplication::translate("PanelCapture", "Down", nullptr));
        imagesLastButton->setText(QCoreApplication::translate("PanelCapture", "Down", nullptr));
        imageRotLabel->setText(QCoreApplication::translate("PanelCapture", "Rot", nullptr));
        (void)PanelCapture;
    } // retranslateUi

};

namespace Ui {
    class PanelCapture: public Ui_PanelCapture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANELCAPTURE_H
