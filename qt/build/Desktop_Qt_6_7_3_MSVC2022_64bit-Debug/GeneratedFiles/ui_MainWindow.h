/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ImgGLWidget.hpp"
#include "WrlGLWidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *workDirCombo;
    QLineEdit *workDirLine;
    QPushButton *workDirLoadButton;
    QSplitter *splitter;
    WrlGLWidget *wrlGLWidget;
    ImgGLWidget *imgGLWidget;
    QWidget *panelsWidget;
    QVBoxLayout *verticalLayout_2;
    QComboBox *panelCombo;
    QWidget *currPanelWidget;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName("mainWindow");
        mainWindow->resize(615, 393);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/camera.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        mainWindow->setWindowIcon(icon);
        actionAbout = new QAction(mainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        workDirCombo = new QComboBox(centralwidget);
        workDirCombo->setObjectName("workDirCombo");
        workDirCombo->setMaximumSize(QSize(200, 22));

        horizontalLayout->addWidget(workDirCombo);

        workDirLine = new QLineEdit(centralwidget);
        workDirLine->setObjectName("workDirLine");
        workDirLine->setEnabled(true);
        workDirLine->setMaximumSize(QSize(16777215, 22));
        workDirLine->setFocusPolicy(Qt::ClickFocus);
        workDirLine->setFrame(false);

        horizontalLayout->addWidget(workDirLine);

        workDirLoadButton = new QPushButton(centralwidget);
        workDirLoadButton->setObjectName("workDirLoadButton");

        horizontalLayout->addWidget(workDirLoadButton);


        verticalLayout->addLayout(horizontalLayout);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        wrlGLWidget = new WrlGLWidget(splitter);
        wrlGLWidget->setObjectName("wrlGLWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(wrlGLWidget->sizePolicy().hasHeightForWidth());
        wrlGLWidget->setSizePolicy(sizePolicy1);
        splitter->addWidget(wrlGLWidget);
        imgGLWidget = new ImgGLWidget(splitter);
        imgGLWidget->setObjectName("imgGLWidget");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(7);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(imgGLWidget->sizePolicy().hasHeightForWidth());
        imgGLWidget->setSizePolicy(sizePolicy2);
        splitter->addWidget(imgGLWidget);
        panelsWidget = new QWidget(splitter);
        panelsWidget->setObjectName("panelsWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(panelsWidget->sizePolicy().hasHeightForWidth());
        panelsWidget->setSizePolicy(sizePolicy3);
        verticalLayout_2 = new QVBoxLayout(panelsWidget);
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        panelCombo = new QComboBox(panelsWidget);
        panelCombo->setObjectName("panelCombo");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(panelCombo->sizePolicy().hasHeightForWidth());
        panelCombo->setSizePolicy(sizePolicy4);

        verticalLayout_2->addWidget(panelCombo);

        currPanelWidget = new QWidget(panelsWidget);
        currPanelWidget->setObjectName("currPanelWidget");
        sizePolicy.setHeightForWidth(currPanelWidget->sizePolicy().hasHeightForWidth());
        currPanelWidget->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(currPanelWidget);

        splitter->addWidget(panelsWidget);

        verticalLayout->addWidget(splitter);

        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 615, 21));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName("statusbar");
        mainWindow->setStatusBar(statusbar);

        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About...", nullptr));
        workDirLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "Work Directory", nullptr));
        workDirLoadButton->setText(QCoreApplication::translate("MainWindow", "LOAD", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
