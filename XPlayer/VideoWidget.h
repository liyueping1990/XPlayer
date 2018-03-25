#pragma once
#include <QtWidgets\qwidget.h>
#include <qopenglwidget.h>
#include <qpainter.h>
#include "XFFmpeg.h"

class VideoWidget : public QOpenGLWidget
{
public:
	VideoWidget();
	VideoWidget(QWidget *p = nullptr);
	void paintEvent(QPaintEvent *e);
	void timerEvent(QTimerEvent *e);
	virtual ~VideoWidget();
};

