#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGui.h"

class QtGui : public QMainWindow
{
	Q_OBJECT

public:
	QtGui(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiClass ui;
};
