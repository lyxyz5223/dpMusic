#pragma once

//Qt
#include <QtWidgets/QMainWindow>
#include "ui_dpMusic.h"
#include <QPainter>
#include <qevent.h>
#include <qdir>
#include <QStringConverter>
#include <QStringListModel>
#include <qmessagebox.h>
#include <QStandardItemModel>

//库
#include "MusicPlay.h"


//VC++
#include "stringProcess.h"
#include <thread>

class dpMusic : public QMainWindow
{
    Q_OBJECT

public:
    dpMusic(QWidget* parent = nullptr);
    ~dpMusic();
    //void onMusicPlayFinished();
    void showLocalMusicsListProc(QString path);
    Ui::dpMusicClass getUi() {
        return ui;
    }
public slots:
    void showOnlineMusicsList(std::vector<std::string> list);
    void showLocalMusicsList();
    void listViewDoubleClicked(QModelIndex index);
    void MusicControlPlayPausedClicked();
    void MusicControlPreviousOneClicked();
    void MusicControlNextOneClicked();
    void maximumBtnClicked();
    void showHomepage();
protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e);
private:
    Ui::dpMusicClass ui;
    MusicPlay mp;

};
