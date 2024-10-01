#include "MyMusicsListView.h"
#include <QPainter>
#include "MyScrollBar.h"
#include <QPainterPath>
#include <QProxyStyle>

MyMusicsListView::MyMusicsListView(QWidget* parent) : QListView(parent)
{
	MyScrollBar* myVerticalScrollBar = new MyScrollBar();
	setVerticalScrollBar(myVerticalScrollBar);
	setFrameStyle(QFrame::NoFrame);	// Must
	setViewport(viewPort);
}

void MyMusicsListView::paintEvent(QPaintEvent* e)
{
	
	QPainter p(viewport());
	p.setBrush(QBrush(QColor(110, 100, 110, 255)));
	qreal roundX = 0, roundY = 0;
	roundX = ((MyMusicsListViewParentWidget*)parent())->roundX;
	roundY = ((MyMusicsListViewParentWidget*)parent())->roundY;
	p.drawRoundedRect(rect(), roundX, roundY, Qt::AbsoluteSize);
	//qreal x = size().width() - verticalScrollBar()->size().width();
	//viewPort->resize(size().width() - 2*verticalScrollBar()->size().width(), viewport()->height());
	//viewPort->move(roundX,0);
	QListView::paintEvent(e);
}

//void MyMusicsListView::drawPrimitive(
//    QStyle::PrimitiveElement element,
//    const QStyleOption* option,
//    QPainter* painter,
//    const QWidget* widget)
//{
//    switch (element)
//    {
//        /* PE_PanelItemViewItem ��Ҫ��������б���ı������Լ�ѡ�б���/���������� */
//    case QStyle::PE_PanelItemViewItem:
//    {
//        const QStyleOptionViewItem* opt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
//        if (nullptr == opt) { break; }
//
//        QColor c(Qt::white);	/* Ĭ�ϱ���ɫ */
//        if (QStyle::State_MouseOver & opt->state)
//        {
//            c = QColor(0, 0, 255, 255 * 0.2);
//        }
//        else if (QStyle::State_Selected & opt->state)
//        {
//            c = QColor(0, 0, 255, 255 * 0.5);
//        }
//
//        int x, y, w, h;
//        opt->rect.getRect(&x, &y, &w, &h);
//
//        QPainterPath path;
//        int rowHeight = 0;
//        qreal Radius = 0;
//        /* ����һ�� */
//        if (0 == y)
//        {
//            /* ��������һ�У���Բ�ǵľ���·�� */
//            path.moveTo(x, y + h);
//            path.arcTo(QRect(x, y, 2 * Radius - 5, 2 * Radius - 5), 180, -90);
//            path.lineTo(x + w, y);
//            path.lineTo(x + w, y + h);
//            path.closeSubpath();
//        }
//        /* ����һ�� */
//        bool isLastRow = false;
//            /* �б����ʾ���� */
//        int rowCount = opt->rect.height() / opt->rect.height();
//        /* �� y �������и߼��������� */
//        int index = opt->rect.y() / opt->rect.height();
//
//        if (rowCount == index)
//        {
//            rowHeight = opt->rect.height() - index * opt->rect.height();
//            isLastRow = true;
//        }
//        else if (isLastRow)
//        {
//            /* ��������һ�У���Բ�ǵľ���·�� */
//            path.moveTo(x, y);
//            path.lineTo(x + w, y);
//            path.lineTo(x + w, y + rowHeight);
//            path.arcTo(QRect(x, y + rowHeight - 2 * Radius, 2 * Radius, 2 * Radius), 270, -90);
//            path.closeSubpath();
//        }
//        else
//        {
//            path.addRect(QRect(x, y, w, h));
//        }
//
//        painter->save();
//        painter->setRenderHint(QPainter::Antialiasing);
//        painter->setPen(Qt::NoPen);
//        painter->setBrush(QBrush(c));
//        painter->drawPath(path);
//        painter->restore();
//
//        return;
//    }
//    default:
//        break;
//    }
//    QProxyStyle* qp = new QProxyStyle();
//    qp->drawPrimitive(element, option, painter, widget);
//}
