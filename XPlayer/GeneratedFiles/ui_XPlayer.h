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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XPlayerClass
{
public:

    void setupUi(QWidget *XPlayerClass)
    {
        if (XPlayerClass->objectName().isEmpty())
            XPlayerClass->setObjectName(QStringLiteral("XPlayerClass"));
        XPlayerClass->resize(600, 400);

        retranslateUi(XPlayerClass);

        QMetaObject::connectSlotsByName(XPlayerClass);
    } // setupUi

    void retranslateUi(QWidget *XPlayerClass)
    {
        XPlayerClass->setWindowTitle(QApplication::translate("XPlayerClass", "XPlayer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XPlayerClass: public Ui_XPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLAYER_H
