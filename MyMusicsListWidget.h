#pragma once
#include <QListWidget.h>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include "MyScrollBar.h"
#include <qboxlayout.h>
#include "TranslucentTextButton.h"
#include <qlabel.h>

class MyMusicsListViewItemDelegate : public QStyledItemDelegate
{
public:
	MyMusicsListViewItemDelegate(QWidget* parent = 0) {}
	//MyMusicsListViewItemDelegate(int width, QWidget* parent = 0) {
	//	verticalScrollBarWidth = width;
	//}

	// painting
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex&index) const override {
        Q_UNUSED(index);
		//return option.decorationSize;
        return QSize(option.decorationSize.width(), 30);
    }
	//void setVerticalScrollBarWidth(int width) {
	//	verticalScrollBarWidth = width;
	//}
	//void setViewPortWidth(int width) {
	//	viewPortWidth = width;
	//}
private:
	//int verticalScrollBarWidth = 0;
	//int viewPortWidth = 0;
};
class MyMusicsListWidgetItem :public TranslucentTextButton
{
public:
	MyMusicsListWidgetItem(QWidget* parent = nullptr) {
		if (parent != nullptr)
			QPushButton::setParent(parent);
	}
	MyMusicsListWidgetItem(QString text, QWidget* parent = nullptr)
	{
		if (parent != nullptr)
			QPushButton::setParent(parent);
		this->setText(text);
		QPushButton::setText("");
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
class MyMusicsListWidget : public QListWidget
{

public:
	~MyMusicsListWidget(){}
	MyMusicsListWidget(QWidget* parent = nullptr);
	void addRow(QStringList strlist) {
		QListWidgetItem* i = new QListWidgetItem(strlist[0]);
		this->addItem(i);
		QWidget* w = new QWidget();
		w->resize(i->sizeHint());
		QHBoxLayout* l = new QHBoxLayout(w);
		l->setContentsMargins(0, 0, 0, 0);
		l->setSpacing(15);
		QLabel* numCountLabel = new QLabel(std::to_string((char)this->count()).c_str());
		numCountLabel->setAlignment(Qt::AlignCenter);
		numCountLabel->setMinimumSize(24, 24);
		numCountLabel->setMaximumSize(24, 24);
		l->addWidget(numCountLabel);
		//歌曲名
		QString strButton;
		MyMusicsListWidgetItem* btn;
		for (int i = 0; i < 3; i++)
		{
			strButton = strlist[i];// .replace("&", "&&");//歌曲名，歌手，专辑//两个&&表示转义&字符，不然就是加速键
			btn = new MyMusicsListWidgetItem(strButton, w);
			l->addWidget(btn);
			btn->setRow(count());
			connect(btn, &QPushButton::clicked, this, [=] {emit doubleClicked(model()->index(btn->getRow()-1, 0)); });
		}
		//时长
		strButton = strlist[3].replace("&", "&&");
		QLabel* musicLengthLabel = new QLabel(strButton,w);
		l->addWidget(musicLengthLabel);
		l->setStretch(0, 0);
		l->setStretch(1, 1);
		l->setStretch(2, 1);
		l->setStretch(3, 1);
		l->setStretch(4, 1);
		w->setLayout(l);
		this->setItemWidget(i, w);
	}

protected:
	void paintEvent(QPaintEvent* e)override;


private:
	MyScrollBar* myVerticalScrollBar = new MyScrollBar();
	mutable QWidget* viewPort = new QWidget(this);
	MyMusicsListViewItemDelegate* myMusicsListViewItemDelegate = new MyMusicsListViewItemDelegate();
};

