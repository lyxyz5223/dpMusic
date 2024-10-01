#pragma once
#include <qwidget.h>

class MyMusicsListViewParentWidget : public QWidget
{
public:
	~MyMusicsListViewParentWidget() {}
	MyMusicsListViewParentWidget(QWidget* parent = nullptr);
	qreal roundX, roundY;
protected:
	void paintEvent(QPaintEvent* e);
};

