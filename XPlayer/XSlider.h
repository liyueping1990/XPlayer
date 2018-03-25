#pragma once
#include <qslider.h>
#include <QMouseEvent>

class XSlider : public QSlider
{
	Q_OBJECT;
public:	
	XSlider(QWidget *p = nullptr);
	virtual ~XSlider();
	void mousePressEvent(QMouseEvent *e);
};

