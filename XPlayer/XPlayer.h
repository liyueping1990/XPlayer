#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlayer.h"

class XPlayer : public QWidget
{
	Q_OBJECT

public:
	XPlayer(QWidget *parent = Q_NULLPTR);
	~XPlayer();
	void resizeEvent(QResizeEvent *e);
	void timerEvent(QTimerEvent *e);
public slots:
	void open();
	void play();
	void sliderPress();
	void sliderRelease();
private:
	Ui::XPlayerClass ui;
};
