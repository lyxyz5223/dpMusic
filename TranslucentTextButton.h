#pragma once
#include <qpushbutton.h>
#include <qlabel.h>

class TranslucentTextButton :
    public QLabel
{
    Q_OBJECT
public:
    ~TranslucentTextButton() {}
    TranslucentTextButton(QWidget* parent = nullptr);
    TranslucentTextButton(QString text, QWidget* parent = nullptr);
    void setText(QString text) {
        this->text = text;
    }
signals:
    void doubleClicked();
    void clicked();
protected:
    void paintEvent(QPaintEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e) {
        emit doubleClicked();
    }
    void mouseReleaseEvent(QMouseEvent* e) {
        emit clicked();
    }
private:
    QString text;
};

