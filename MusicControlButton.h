#pragma once
#include <qpushbutton.h>
class MusicControlButton :
    public QPushButton
{
    Q_OBJECT
public:
    ~MusicControlButton(){}
    MusicControlButton(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* e)override;

private:
};

