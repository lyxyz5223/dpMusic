#include "MyMusicsListViewParentWidget.h"
#include <QPainter>

MyMusicsListViewParentWidget::MyMusicsListViewParentWidget(QWidget* parent):QWidget(parent)
{
	roundX = roundY = (size().width() > size().height() ? size().height() / 20 : size().width() / 20);

}

void MyMusicsListViewParentWidget::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.setBrush(QBrush(QColor(255,0,0,255)));
	p.setPen(Qt::NoPen);
	qreal DivideBy = 50;
	roundX = roundY = (size().width() > size().height() ? size().height()/DivideBy : size().width()/DivideBy);
	p.drawRoundedRect(rect(), roundX, roundY, Qt::SizeMode::AbsoluteSize);
	QWidget::paintEvent(e);
}
