#pragma once
#include <qlistview.h>
#include "MyMusicsListViewParentWidget.h"

class MyMusicsListView : public QListView
{
public:
	~MyMusicsListView(){}
	MyMusicsListView(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* e)override;

	//Paint items
	//void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget);

private:
	mutable QWidget* viewPort = new QWidget(this);
};

