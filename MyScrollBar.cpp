#include "MyScrollBar.h"
#include <QPainter>

MyScrollBar::MyScrollBar(QWidget* parent) : QScrollBar(parent)
{
	setAttribute(Qt::WA_TranslucentBackground,true);
	setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void MyScrollBar::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.fillRect(rect(), QBrush(QColor(100, 110, 110, 75)));
	QWidget::paintEvent(e);
	//QScrollBar::paintEvent(e);
}
