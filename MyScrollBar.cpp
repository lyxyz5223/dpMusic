#include "MyScrollBar.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionComplex>

MyScrollBar::MyScrollBar(QWidget* parent) : QScrollBar(parent)
{
	setAttribute(Qt::WA_TranslucentBackground,true);
	setAttribute(Qt::WA_OpaquePaintEvent, false);
	setStyle(mystyle);
}

void MyScrollBar::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.setPen(Qt::NoPen);
	p.setBrush(QBrush(QColor(250, 235, 215, 255)));
	//if (roundX > width())
	//	roundX = roundY = width() < height() ? width() : height();
	//if (roundY > height())
	//	roundX = roundY = width() < height() ? width() : height();
	p.setRenderHints(QPainter::Antialiasing);
	//p.drawRoundedRect(rect(), roundX, roundY, Qt::AbsoluteSize);
	//p.fillRect(rect(), QBrush(QColor(110, 100, 110, 255)));
	qreal corner_radius = radius.x;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.moveTo(0, 0);
	path.lineTo(0, height());
	path.lineTo(width()-corner_radius, height());
	//path.moveTo(width() - corner_radius, height() - corner_radius);
	path.arcTo(width() - 2 * corner_radius, height() - 2 * corner_radius, 2 * corner_radius, 2 * corner_radius, 270.0, 90.0);
	path.lineTo(width(), corner_radius);
	//path.moveTo(width() - corner_radius, corner_radius);
	path.arcTo(width() - 2 * corner_radius, 0, 2 * corner_radius, 2 * corner_radius, 0.0, 90.0);
	path.lineTo(0, 0);
	p.drawPath(path);
	//QWidget::paintEvent(e);
	QScrollBar::paintEvent(e);
}

void MyStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const 
{
	//printf("MyScrollBar---Draw!!!!!!\n");
	painter->save();
	painter->setPen(Qt::NoPen);
	switch (control)
	{
	case QStyle::CC_ScrollBar:
	{
		painter->setRenderHint(QPainter::Antialiasing);
		if (option->subControls & SC_ScrollBarSlider)
		{
			QRect r = subControlRect(control, option, SC_ScrollBarSlider, widget);
			int wid = 5;
			r.setRect((r.width() - wid) / 2, r.y(), wid, r.height());
			qreal circle_radius = r.width() > r.height() ? float(r.height()) / 2 : float(r.width())/2;
			painter->setBrush(QColor(188, 143, 143, 255));
			painter->drawRoundedRect(r, circle_radius, circle_radius);
		}
		//if (option->subControls & SC_ScrollBarSubPage)
		//{
		//	QRect r = subControlRect(control, option, SC_ScrollBarSubPage, widget);
		//  qreal circle_radius = r.width() > r.height() ? float(r.height()) / 2 : float(r.width()) / 2;
		//	painter->setBrush(QColor(255,240,245));
		//	painter->drawRoundedRect(r, circle_radius, circle_radius);
		//}
		//if (option->subControls & SC_ScrollBarAddPage)
		//{
		//	QRect r = subControlRect(control, option, SC_ScrollBarAddPage, widget);
		//  qreal circle_radius = r.width() > r.height() ? float(r.height()) / 2 : float(r.width()) / 2;
		//	painter->setBrush(QColor(255, 240, 245));
		//	painter->drawRoundedRect(r, circle_radius, circle_radius);
		//}
		if (option->subControls & SC_ScrollBarSubLine)
		{
			QRect r = subControlRect(control, option, SC_ScrollBarSubLine, widget);
			qreal circle_radius = r.width() > r.height() ? float(r.height()) / 2 : float(r.width()) / 2;
			painter->setBrush(QColor(238, 180, 180, 255));
			painter->drawRoundedRect(r, circle_radius, circle_radius);
		}
		if (option->subControls & SC_ScrollBarAddLine)
		{
			QRect r = subControlRect(control, option, SC_ScrollBarAddLine, widget);
			qreal circle_radius = r.width() > r.height() ? float(r.height()) / 2 : float(r.width()) / 2;
			painter->setBrush(QColor(238, 180, 180, 255));
			painter->drawRoundedRect(r, circle_radius, circle_radius);
		}
		painter->restore();
		return;
	}
		break;
	default:
		break;
	}
	QProxyStyle::drawComplexControl(control, option, painter, widget);
}
