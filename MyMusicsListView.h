#pragma once
#include <qlistview.h>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include "MyScrollBar.h"

class MyMusicsListViewItemDelegate : public QStyledItemDelegate
{
public:
	MyMusicsListViewItemDelegate(QWidget* parent = 0) {}
	MyMusicsListViewItemDelegate(int width, QWidget* parent = 0) {
		verticalScrollBarWidth = width;
	}

	// painting
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex&index) const override {
        Q_UNUSED(index);
        return QSize(option.rect.width(), 30);
    }
	void setVerticalScrollBarWidth(int width) {
		verticalScrollBarWidth = width;
	}
	void setViewPortWidth(int width) {
		viewPortWidth = width;
	}
private:
	int verticalScrollBarWidth = 0;
	int viewPortWidth = 0;
};
class MyMusicsListView : public QListView
{
public:
	~MyMusicsListView(){}
	MyMusicsListView(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* e)override;


private:
	MyScrollBar* myVerticalScrollBar = new MyScrollBar();
	mutable QWidget* viewPort = new QWidget(this);
	MyMusicsListViewItemDelegate* myMusicsListViewItemDelegate = new MyMusicsListViewItemDelegate();
};

