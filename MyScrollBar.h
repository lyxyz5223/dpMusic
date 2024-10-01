#pragma once
#include <QScrollBar>

class MyScrollBar : public QScrollBar
{
public:
	~MyScrollBar() {}
	MyScrollBar(QWidget*parent = nullptr);
protected:
	void paintEvent(QPaintEvent* e);

private:

};

