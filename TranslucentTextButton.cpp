#include "TranslucentTextButton.h"
#include <qpainter.h>

TranslucentTextButton::TranslucentTextButton(QWidget* parent) : QLabel(parent)
{

}

TranslucentTextButton::TranslucentTextButton(QString text, QWidget* parent) : QLabel(parent)
{
	this->setText(text);
	//QPushButton::setText("");
	this->adjustSize();
}

void TranslucentTextButton::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.fillRect(rect(), QColor(255, 255, 255, 0));
	QFontMetrics* qfm = new QFontMetrics(font());
	QString text1 = qfm->elidedText(this->text, Qt::ElideRight, width());
	p.drawText(rect(), text1, QTextOption(Qt::AlignVCenter|Qt::AlignLeft));
	//QPushButton::paintEvent(e);
}
