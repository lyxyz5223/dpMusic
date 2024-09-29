#pragma once
#include <qpushbutton.h>
class TranslucentIconButton :
    public QPushButton
{
    Q_OBJECT
public:
    ~TranslucentIconButton() {}
    TranslucentIconButton(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* e)override;

private:
};


