#include "dpMusic.h"
#include <QInputDialog>
dpMusic::dpMusic(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    mp.setPath("C:\\Users\\lyxyz5223\\Desktop\\LxMusicDownloads\\");//设置播放路径
    //设置歌曲名，若歌曲名带有路径，请setPath("");
    //mp.setFileName("野花香 (Haocore Mix) - Daily天利、EndCat-终猫.mp3");
    //mp.play();
    connect(ui.localListButton, &QPushButton::clicked, this,  
    [=](){
        QString p = QInputDialog::getText(this, "请输入音乐文件夹路径", "请输入音乐文件夹路径");
        mp.setPath(p.toStdString());
        QDir dir;
        dir.setPath(mp.getPath().c_str());
        QFileInfoList fil;
        QStringList strList;//文件列表
        std::vector<std::string> MusicNameList;
        QStringList filterList;//后缀过滤
        filterList << "*.flac" << "*.mp3" << "*.wav" << "*.ogg" << "*.ape";
        fil = dir.entryInfoList(filterList, QDir::Filter::Files, QDir::SortFlag::Name);
        for (QFileInfo qfi : fil)
        {
            MusicNameList.push_back(qfi.fileName().toStdString());
            strList.push_back(qfi.fileName());
            printf(UTF8ToANSI("QFileInfoList：%s\n").c_str(), UTF8ToANSI(qfi.fileName().toStdString()).c_str());
        }
        QStringListModel* modelitem;
        modelitem = new QStringListModel(strList);
        mp.setMusicsListVector(MusicNameList);
        ui.musicListView->setModel(modelitem);
        ui.musicListView->setEditTriggers(QListView::NoEditTriggers);
    });
}

dpMusic::~dpMusic()
{}


void dpMusic::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    QBrush brush(QColor(255, 255, 240,150));
    p.setBrush(brush);
    p.setRenderHints(QPainter::RenderHint::Antialiasing);
    qreal rad = qreal(width() > height() ? height() : width()) / 35;
    p.drawRoundedRect(rect(),rad,rad,Qt::SizeMode::AbsoluteSize);
    p.setBrush(Qt::NoBrush);
    rad = qreal(ui.horizontalLayout_3->geometry().width() > ui.horizontalLayout_3->geometry().height() ? ui.horizontalLayout_3->geometry().height() : ui.horizontalLayout_3->geometry().width()) / 5;
    p.drawRoundedRect(ui.horizontalLayout_3->geometry(),rad,rad,Qt::SizeMode::AbsoluteSize);

    QWidget::paintEvent(e);
}
#ifdef _WIN32
    #include <Windows.h>
#endif // _WIN32

void dpMusic::mousePressEvent(QMouseEvent* e)//鼠标按下事件
{
    if (e->pos().y() <= ui.horizontalLayout_3->geometry().bottom())
    {
#ifdef _WIN32
        if (ReleaseCapture())
            SendMessage(HWND(winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
#endif // _WIN32
        e->ignore();
    }
    QMainWindow::mousePressEvent(e);
}

void dpMusic::listViewDoubleClicked(QModelIndex index)
{
    mp.setFileName(ui.musicListView->model()->data(index).toString().toStdString());
    //while (ui.musicListView->model()->canFetchMore(index))
    //    ui.musicListView->model()->fetchMore(index);
    int rowCount = ui.musicListView->model()->rowCount();
    int columnCount = ui.musicListView->model()->columnCount();
    mp.setPlayingIndex(index.row()*(columnCount)+index.column()+1);
    //if (mp.isPlaying())//在类里面已经实现了检测，无需再次检测
    mp.stop();
    mp.play();
    ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
    ui.musicInformation->setText(mp.getFileName().c_str());
}

void dpMusic::MusicControlPlayPausedClicked()
{
    if (mp.isPaused() || !mp.isPlaying())//暂停或者不在播放
    {
        if (mp.getFileName() != "")
        {
            ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
            mp.pause();
        }
    }
    else
    {
        ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/play.svg"));
        mp.pause();
    }
}

void dpMusic::MusicControlPreviousOneClicked()
{
    if (mp.getPlayingIndex() > 1)
    {
        mp.setPlayingIndex(mp.getPlayingIndex() - 1);
    }
    else
    {
        //QMessageBox::information(this, "~~~", "这已经是第一首了哦~");
        mp.setPlayingIndex(mp.getMusicsListVector().size());
    }
        mp.setFileName(mp.getMusicsListVector()[mp.getPlayingIndex() - 1]);
        mp.stop();
        mp.play();
        ui.musicInformation->setText(mp.getFileName().c_str());

}

void dpMusic::MusicControlNextOneClicked()
{
    if (mp.getPlayingIndex() < mp.getMusicsListVector().size())
    {
        mp.setPlayingIndex(mp.getPlayingIndex() + 1);
    }
    else
    {
        //QMessageBox::information(this, "~~~", "这已经是最后一首了哦~");
        mp.setPlayingIndex(1);
    }
        mp.setFileName(mp.getMusicsListVector()[mp.getPlayingIndex() - 1]);
        mp.stop();
        mp.play();
        ui.musicInformation->setText(mp.getFileName().c_str());
}