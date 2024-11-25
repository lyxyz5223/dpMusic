#include "MyMusicsListView.h"
#include <QPainter>
#include <QPainterPath>
#include <QProxyStyle>

MyMusicsListView::MyMusicsListView(QWidget* parent) : QTreeView(parent)
{
	setVerticalScrollBar(myVerticalScrollBar);
	setFrameStyle(QFrame::NoFrame);	// Must
	setViewport(viewPort);
	myVerticalScrollBar->setRadius(7, 7);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setItemDelegate(myMusicsListViewItemDelegate);
	//添加列表项的信号与槽的绑定
	connect(this, &MyMusicsListView::addRowSignal, this, &MyMusicsListView::addRowProc);
}

void MyMusicsListView::paintEvent(QPaintEvent* e)
{
	
	QPainter p(viewport());
	p.setRenderHints(QPainter::Antialiasing);
	p.setBrush(QBrush(QColor(110, 100, 110, 0)));
	p.setPen(Qt::NoPen);
	qreal roundX, roundY;
	roundX = myVerticalScrollBar->getRadius().x;
	roundY = myVerticalScrollBar->getRadius().y;
	p.drawRoundedRect(rect(), roundX, roundY, Qt::AbsoluteSize);
	QTreeView::paintEvent(e);
}


void MyMusicsListViewItemDelegate::paint(QPainter* painter,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	//printf("MyMusicsListViewItemDelegate---paint!!  ");
	//painter->save();
	//painter->setPen(Qt::NoPen);
	//painter->setRenderHints(QPainter::Antialiasing);
	//if (index.isValid()) {
	//	//Paint Items
	//	QRect rect = option.rect;
	//	if (option.state.testFlag(QStyle::State_MouseOver))
	//		painter->setBrush(QColor(225, 221, 221, 255));
	//	else if (option.state.testFlag(QStyle::State_Selected))
	//		painter->setBrush(QColor(200, 200, 200, 255));
	//	else
	//		painter->setBrush(QColor(0, 0, 0, 0));
	//	painter->drawRoundedRect(QRect(rect.x(), rect.y(), rect.width()/* - verticalScrollBarWidth */, rect.height()), 5, 5);
	//	painter->restore();
	//	QVariant var = index.data(Qt::ItemDataRole::DisplayRole);
	//	QString itemData = var.value<QString>();
	//	var = index.data(Qt::ItemDataRole::FontRole);
	//	QFont font = var.value<QFont>();
	//	QFontMetrics qfm(font);
	//	QTextOption qto;
	//	qto.setAlignment(Qt::AlignVCenter);
	//	painter->drawText(QRect(rect.x() + qfm.averageCharWidth(), rect.y(), rect.width(), rect.height()), itemData, qto);
	//}
	//else
	//	painter->restore();
	QStyledItemDelegate::paint(painter, option, index);
}

void MyMusicsListView::drawRow(QPainter* painter, 
	const QStyleOptionViewItem& options, 
	const QModelIndex& index) const
{
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setRenderHints(QPainter::Antialiasing);
	if (index.isValid()) {
		//Paint Items
		QRect rect = options.rect;
		if (options.state.testAnyFlag(QStyle::State_MouseOver))
			painter->setBrush(QColor(225, 221, 221, 255));
		else if (options.state.testAnyFlag(QStyle::State_Selected))
			painter->setBrush(QColor(200, 200, 200, 255));
		else
			painter->setBrush(QColor(0, 0, 0, 0));
		painter->drawRoundedRect(QRect(rect.x(), rect.y(), rect.width()/* - verticalScrollBarWidth */, rect.height()), 5, 5);
		painter->restore();
		QVariant var = index.data(Qt::ItemDataRole::DisplayRole);
		QString itemData = var.value<QString>();
		var = index.data(Qt::ItemDataRole::FontRole);
		QFont font = var.value<QFont>();
		QFontMetrics qfm(font);
		QTextOption qto;
		qto.setAlignment(Qt::AlignVCenter);
		//painter->drawText(QRect(rect.x() + qfm.averageCharWidth(), rect.y(), rect.width(), rect.height()), itemData, qto);
	}
	else
		painter->restore();
	QTreeView::drawRow(painter, options, index);
}
