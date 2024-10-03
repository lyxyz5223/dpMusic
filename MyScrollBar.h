#pragma once
#include <QScrollBar>
#include <QProxyStyle>

class MyStyle : public QProxyStyle
{
public:
	~MyStyle() {}
	MyStyle() {}
	void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const override;
protected:
};

class MyScrollBar : public QScrollBar
{
	Q_OBJECT
private:
	struct Radius
	{
		qreal x = 7;
		qreal y = 7;
	} radius;
	MyStyle* mystyle = new MyStyle();
public:
	~MyScrollBar() {}
	MyScrollBar(QWidget*parent = nullptr);
	void setRadius(qreal x, qreal y) {
		radius.x = x;
		radius.y = y;
	}
	Radius getRadius() {
		return radius;
	}
	
protected:
	void paintEvent(QPaintEvent* e);
	
};

