#pragma once
#include <QListView.h>
#include <QTreeView.h>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include "MyScrollBar.h"
#include <qboxlayout.h>
#include "TranslucentTextButton.h"
#include <qlabel.h>
#include <qabstractitemmodel.h>
#include <qstringlistmodel.h>
#include "MyMusicsListViewItemModel.h"
class MyMusicsListViewItemDelegate : public QStyledItemDelegate
{
public:
	MyMusicsListViewItemDelegate(QWidget* parent = 0) {}

	// painting
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex&index) const override {
        Q_UNUSED(index);
		//return option.decorationSize;
        return QSize(option.decorationSize.width(), 30);
    }
private:
};
class MyMusicsListViewItem :public TranslucentTextButton
{
public:
	MyMusicsListViewItem(QWidget* parent = nullptr) {
		if (parent != nullptr)
			QLabel::setParent(parent);
	}
	MyMusicsListViewItem(QString text, QWidget* parent = nullptr)
	{
		if (parent != nullptr)
			QLabel::setParent(parent);
		this->setText(text);
		QLabel::setText("");
		this->adjustSize();
	}

	const int getRow() const {
		return row;
	}
	void setRow(const int row) {
		this->row = row;
	}
private:
	int row = 0;
};
class MyMusicsListView : public QTreeView
{
	Q_OBJECT
public slots:
	//添加列表项的操作过程
	void addRowProc(QStringList strList) {
		musicsListModel->addRow(strList);
		this->setModel(musicsListModel);
	}
signals:
	void addRowSignal(QStringList strlist);

public:
	~MyMusicsListView(){}
	MyMusicsListView(QWidget* parent = nullptr);
	void addRow(QStringList strlist) {
		emit addRowSignal(strlist);//发送添加列表项的信号
	}
protected:
	void paintEvent(QPaintEvent* e)override;
	void drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const;

private:
	MyScrollBar* myVerticalScrollBar = new MyScrollBar();
	mutable QWidget* viewPort = new QWidget(this);
	MyMusicsListViewItemDelegate* myMusicsListViewItemDelegate = new MyMusicsListViewItemDelegate();
	MyMusicsListViewItemModel *musicsListModel = new MyMusicsListViewItemModel;
};

