#pragma once
#include <qpushbutton.h>

class TranslucentTextButton :
    public QPushButton
{
public:
    ~TranslucentTextButton() {}
    TranslucentTextButton(QWidget* parent = nullptr);
    TranslucentTextButton(QString text, QWidget* parent = nullptr);
    void setText(QString text) {
        this->text = text;
    }
protected:
    void paintEvent(QPaintEvent* e);
private:
    QString text;
};

