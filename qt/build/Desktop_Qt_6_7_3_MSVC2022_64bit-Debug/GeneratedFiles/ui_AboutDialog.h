/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLabel *title_label;
    QLabel *course_label;
    QFrame *frame;
    QFormLayout *formLayout;
    QLabel *url_title_label;
    QLabel *url_label;
    QLabel *qt_version_title_label;
    QLabel *qt_version_label;
    QLabel *opencv_title_label;
    QLabel *opencv_version_label;
    QLabel *built_date_title_label;
    QLabel *built_date_label;
    QLabel *student_title_label;
    QLabel *student_label;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *author_label;
    QSpacerItem *horizontalSpacer;
    QPushButton *close_button;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName("AboutDialog");
        AboutDialog->resize(546, 303);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/camera.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        AboutDialog->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(AboutDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(AboutDialog);
        label->setObjectName("label");
        label->setMaximumSize(QSize(100, 100));
        label->setPixmap(QPixmap(QString::fromUtf8(":/camera.svg")));
        label->setScaledContents(true);

        horizontalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        title_label = new QLabel(AboutDialog);
        title_label->setObjectName("title_label");
        QFont font;
        font.setPointSize(16);
        title_label->setFont(font);
        title_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title_label);

        course_label = new QLabel(AboutDialog);
        course_label->setObjectName("course_label");
        QFont font1;
        font1.setPointSize(12);
        course_label->setFont(font1);
        course_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(course_label);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        frame = new QFrame(AboutDialog);
        frame->setObjectName("frame");
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName("formLayout");
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        url_title_label = new QLabel(frame);
        url_title_label->setObjectName("url_title_label");
        QFont font2;
        font2.setPointSize(10);
        url_title_label->setFont(font2);

        formLayout->setWidget(1, QFormLayout::LabelRole, url_title_label);

        url_label = new QLabel(frame);
        url_label->setObjectName("url_label");
        url_label->setFont(font2);
        url_label->setTextFormat(Qt::RichText);
        url_label->setOpenExternalLinks(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, url_label);

        qt_version_title_label = new QLabel(frame);
        qt_version_title_label->setObjectName("qt_version_title_label");
        qt_version_title_label->setFont(font2);

        formLayout->setWidget(2, QFormLayout::LabelRole, qt_version_title_label);

        qt_version_label = new QLabel(frame);
        qt_version_label->setObjectName("qt_version_label");
        qt_version_label->setFont(font2);

        formLayout->setWidget(2, QFormLayout::FieldRole, qt_version_label);

        opencv_title_label = new QLabel(frame);
        opencv_title_label->setObjectName("opencv_title_label");
        opencv_title_label->setFont(font2);

        formLayout->setWidget(3, QFormLayout::LabelRole, opencv_title_label);

        opencv_version_label = new QLabel(frame);
        opencv_version_label->setObjectName("opencv_version_label");
        opencv_version_label->setFont(font2);

        formLayout->setWidget(3, QFormLayout::FieldRole, opencv_version_label);

        built_date_title_label = new QLabel(frame);
        built_date_title_label->setObjectName("built_date_title_label");
        built_date_title_label->setFont(font2);

        formLayout->setWidget(4, QFormLayout::LabelRole, built_date_title_label);

        built_date_label = new QLabel(frame);
        built_date_label->setObjectName("built_date_label");
        built_date_label->setFont(font2);

        formLayout->setWidget(4, QFormLayout::FieldRole, built_date_label);

        student_title_label = new QLabel(frame);
        student_title_label->setObjectName("student_title_label");
        student_title_label->setFont(font2);

        formLayout->setWidget(5, QFormLayout::LabelRole, student_title_label);

        student_label = new QLabel(frame);
        student_label->setObjectName("student_label");
        student_label->setFont(font2);

        formLayout->setWidget(5, QFormLayout::FieldRole, student_label);


        verticalLayout_2->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        author_label = new QLabel(AboutDialog);
        author_label->setObjectName("author_label");
        author_label->setFont(font2);

        horizontalLayout->addWidget(author_label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        close_button = new QPushButton(AboutDialog);
        close_button->setObjectName("close_button");

        horizontalLayout->addWidget(close_button);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QCoreApplication::translate("AboutDialog", "About Camera-Projector Calibration", nullptr));
        label->setText(QString());
        title_label->setText(QCoreApplication::translate("AboutDialog", "3D Photography Software", nullptr));
        course_label->setText(QCoreApplication::translate("AboutDialog", "Brown University - ENGN2502", nullptr));
        url_title_label->setText(QCoreApplication::translate("AboutDialog", "URL:", nullptr));
        url_label->setText(QCoreApplication::translate("AboutDialog", "<html><head/><body><p><a href=\"http://mesh.brown.edu/3DP-2016/\"><span style=\" text-decoration: underline; color:#0000ff;\">http://mesh.brown.edu/3DP-2016/</span></a></p></body></html>", nullptr));
        qt_version_title_label->setText(QCoreApplication::translate("AboutDialog", "Qt Version:", nullptr));
        qt_version_label->setText(QCoreApplication::translate("AboutDialog", "qt ver", nullptr));
        opencv_title_label->setText(QCoreApplication::translate("AboutDialog", "OpenCV Version:", nullptr));
        opencv_version_label->setText(QCoreApplication::translate("AboutDialog", "opencv ver", nullptr));
        built_date_title_label->setText(QCoreApplication::translate("AboutDialog", "Built on:", nullptr));
        built_date_label->setText(QCoreApplication::translate("AboutDialog", "build date time", nullptr));
        student_title_label->setText(QCoreApplication::translate("AboutDialog", "Student:", nullptr));
        student_label->setText(QCoreApplication::translate("AboutDialog", "not set", nullptr));
        author_label->setText(QCoreApplication::translate("AboutDialog", "(c) 2016 Daniel Moreno and Gabriel Taubin", nullptr));
        close_button->setText(QCoreApplication::translate("AboutDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
