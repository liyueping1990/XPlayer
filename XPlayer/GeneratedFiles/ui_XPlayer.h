/********************************************************************************
** Form generated from reading UI file 'XPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XPLAYER_H
#define UI_XPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "VideoWidget.h"
#include "XSlider.h"

QT_BEGIN_NAMESPACE

class Ui_XPlayerClass
{
public:
    VideoWidget *openGLWidget;
    QPushButton *openButton;
    QPushButton *playButton;
    QLabel *playTime;
    QLabel *totalTime;
    XSlider *playSlider;

    void setupUi(QWidget *XPlayerClass)
    {
        if (XPlayerClass->objectName().isEmpty())
            XPlayerClass->setObjectName(QStringLiteral("XPlayerClass"));
        XPlayerClass->resize(800, 600);
        QIcon icon;
        icon.addFile(QStringLiteral(":/XPlayer/Resources/logo1.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XPlayerClass->setWindowIcon(icon);
        openGLWidget = new VideoWidget(XPlayerClass);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 0, 800, 600));
        openGLWidget->setStyleSheet(QStringLiteral(""));
        openButton = new QPushButton(XPlayerClass);
        openButton->setObjectName(QStringLiteral("openButton"));
        openButton->setGeometry(QRect(320, 530, 50, 50));
        openButton->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/XPlayer/Resources/open_normal.png);}\n"
"QPushButton:hover{border-image: url(:/XPlayer/Resources/open_hot.png);}"));
        playButton = new QPushButton(XPlayerClass);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setGeometry(QRect(400, 530, 50, 50));
        playButton->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/XPlayer/Resources/play_normal.png);}\n"
"QPushButton:hover{border-image: url(:/XPlayer/Resources/play_hot.png);}"));
        playTime = new QLabel(XPlayerClass);
        playTime->setObjectName(QStringLiteral("playTime"));
        playTime->setGeometry(QRect(40, 540, 71, 31));
        playTime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 10pt \"\345\256\213\344\275\223\";"));
        totalTime = new QLabel(XPlayerClass);
        totalTime->setObjectName(QStringLiteral("totalTime"));
        totalTime->setGeometry(QRect(100, 540, 71, 31));
        totalTime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 10pt \"\345\256\213\344\275\223\";"));
        playSlider = new XSlider(XPlayerClass);
        playSlider->setObjectName(QStringLiteral("playSlider"));
        playSlider->setGeometry(QRect(40, 500, 700, 20));
        playSlider->setStyleSheet(QStringLiteral(""));
        playSlider->setMaximum(999);
        playSlider->setOrientation(Qt::Horizontal);

        retranslateUi(XPlayerClass);
        QObject::connect(openButton, SIGNAL(clicked()), XPlayerClass, SLOT(open()));
        QObject::connect(playSlider, SIGNAL(sliderPressed()), XPlayerClass, SLOT(sliderPress()));
        QObject::connect(playSlider, SIGNAL(sliderReleased()), XPlayerClass, SLOT(sliderRelease()));
        QObject::connect(playButton, SIGNAL(clicked()), XPlayerClass, SLOT(play()));

        QMetaObject::connectSlotsByName(XPlayerClass);
    } // setupUi

    void retranslateUi(QWidget *XPlayerClass)
    {
        XPlayerClass->setWindowTitle(QApplication::translate("XPlayerClass", "XPlayer", nullptr));
        openButton->setText(QString());
        playButton->setText(QString());
        playTime->setText(QApplication::translate("XPlayerClass", "00:00:00", nullptr));
        totalTime->setText(QApplication::translate("XPlayerClass", "/00:00:00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XPlayerClass: public Ui_XPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLAYER_H
