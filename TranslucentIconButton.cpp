#include "TranslucentIconButton.h"
#include <QPainter>

TranslucentIconButton::TranslucentIconButton(QWidget* parent) :QPushButton(parent)
{
}

void TranslucentIconButton::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	QRect r = rect();
	r.setSize(size() * 3 / 4);
	r.moveTo((size().width() - r.width()) / 2, (size().height() - r.height()) / 2);
	p.drawPixmap(r, icon().pixmap(r.size()));
	//QPen pen;
	//pen.setWidth(3);
	//pen.setColor(QColor(0, 0, 0, 255));
	//p.setPen(pen);
	//p.drawRect(rect());
	QWidget::paintEvent(e);
}
