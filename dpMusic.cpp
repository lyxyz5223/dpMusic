#include "dpMusic.h"
#include <QInputDialog>
#ifdef _WIN32
#include <Windows.h>
//#pragma comment(lib,"Version.lib")
#include <propsys.h>
#include <propkey.h>
#include <shobjidl.h>
#include <propidlbase.h>
#endif // _WIN32

//void onMusicEnd(void* musicPlay, void* dpmusic)
//{
//    MusicPlay* mp = (MusicPlay*)musicPlay;
//    dpMusic* dp = (dpMusic*)dpmusic;
//    auto index = mp->getPlayingIndex();
//    mp->setPlayingIndex(mp->getNextPlayingIndex());
//    mp->setFileNameUTF8(mp->getMusicsListVector()[mp->getPlayingIndex() - 1]);
//    mp->play();
//    dp->getUi().musicInformation->setText(mp->getFileNameUTF8().c_str());
//}
void onMusicEnd(void* pUserData, ma_sound* pSound)
{
    MusicPlay::ma_sound_pUserData *ud = (MusicPlay::ma_sound_pUserData*)pUserData;
    dpMusic* dp = (dpMusic*)ud->parentClass;
    MusicPlay* mp = (MusicPlay*)ud->musicplayClass;
    if (mp->getIsAutoPlayNextOne())
    {
        dp->getUi().musicInformation->setText(mp->getFileNameUTF8().c_str());
        mp->setEndCallback(onMusicEnd, dp, mp);
    }

}


dpMusic::dpMusic(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //mp.setPathUTF8("C:\\Users\\lyxyz5223\\Desktop\\LxMusicDownloads\\");//设置播放路径
    //设置歌曲名，若歌曲名带有路径，请setPathUTF8("");
    //mp.setFileNameUTF8("野花香 (Haocore Mix) - Daily天利、EndCat-终猫.mp3");
    //mp.play();
    //connect(ui.localListButton, &QPushButton::clicked, this,  
    //[=](){
    //});
}

dpMusic::~dpMusic()
{}


void dpMusic::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    QBrush brush(QColor(255, 250, 240, 255));
    p.setBrush(brush);
    p.setRenderHints(QPainter::RenderHint::Antialiasing);
    qreal rad;
    //rad = qreal(width() > height() ? height() : width()) / 35;
    rad = 10;
    p.drawRoundedRect(rect(),rad,rad,Qt::SizeMode::AbsoluteSize);
    p.setBrush(Qt::NoBrush);
    p.setPen(QColor(130, 130, 130, 150));
    //rad = qreal(ui.horizontalLayout_3->geometry().width() > ui.horizontalLayout_3->geometry().height() ? ui.horizontalLayout_3->geometry().height() : ui.horizontalLayout_3->geometry().width()) / 5;
    rad = 5;
    p.drawRoundedRect(ui.horizontalLayout_3->geometry(),rad,rad,Qt::SizeMode::AbsoluteSize);

    QWidget::paintEvent(e);
}

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
void dpMusic::showLocalMusicsListProc(QString path)
{
    mp.setPathUTF8(path.toStdString());
    QDir dir;
    dir.setPath(mp.getPathUTF8().c_str());
    QFileInfoList fil;
    QStringList strList;//文件列表，歌曲信息
    std::vector<std::string> musicsList;//歌曲信息
    std::vector<std::string> titlesList;//歌曲标题信息
    std::vector<std::vector<std::string>> artistsList;//歌曲歌手信息
    std::vector<std::string> albumTitlesList;//歌曲专辑名信息
    QStringList filterList;//后缀过滤
    filterList << "*.flac" << "*.mp3" << "*.wav" << "*.ogg" << "*.ape";
    fil = dir.entryInfoList(filterList, QDir::Filter::Files, QDir::SortFlag::Name);
    ui.musicListWidget->setResizeMode(QListView::Adjust);
    ui.musicListWidget->setAutoScroll(true);
    ui.musicListWidget->clear();
    for (QFileInfo qfi : fil)//遍历列表
    {
        musicsList.push_back(qfi.fileName().toStdString());
        strList.push_back(qfi.fileName());
        printf(UTF8ToANSI("QFileInfoList：%s\n").c_str(), UTF8ToANSI(qfi.fileName().toStdString()).c_str());
#ifdef _WIN32
        //Win32获取歌曲信息
        QStringList l;//信息列表
        IPropertyStore* lpMusicInfo = 0;
        std::wstring musicAbsoluteFilePath = qfi.absoluteFilePath().toStdWString();
        while (musicAbsoluteFilePath.find('/') != std::wstring::npos)
            musicAbsoluteFilePath.replace(musicAbsoluteFilePath.find('/'), 1, L"\\");
        HRESULT hr = SHGetPropertyStoreFromParsingName(musicAbsoluteFilePath.c_str(), NULL, GPS_DEFAULT, __uuidof(IPropertyStore), (void**)&lpMusicInfo);
        if (SUCCEEDED(hr))
        {
            PROPVARIANT* musicInfoValue = new PROPVARIANT;
            std::wstring musicInfoWstr;
            std::vector<std::string> artistsVector;
            lpMusicInfo->GetValue(PKEY_Title, musicInfoValue);
            if (musicInfoValue->bstrVal != NULL)
                musicInfoWstr = (WCHAR*)musicInfoValue->bstrVal;
            else
                musicInfoWstr = qfi.fileName().toStdWString();
            titlesList.push_back(wstr2str_2UTF8(musicInfoWstr));//歌曲名添加进入MusicPlay模块的列表
            l.push_back(QString::fromStdWString(musicInfoWstr));//歌曲名添加进展示的歌曲列表
            musicInfoWstr = L"";
            lpMusicInfo->GetValue(PKEY_Music_Artist, musicInfoValue);
            for (ULONG i = 0; i < musicInfoValue->calpwstr.cElems; i++)
            {
                musicInfoWstr += musicInfoValue->calpwstr.pElems[i];
                artistsVector.push_back(wstr2str_2UTF8(musicInfoValue->calpwstr.pElems[i]));
                if (i != musicInfoValue->calpwstr.cElems - 1)
                    musicInfoWstr += L"、";
            }
            if (musicInfoWstr == L"")
                l[0] = qfi.fileName();
            artistsList.push_back(artistsVector);//作曲家添加到MusicPlay模块的作曲家列表
            l.push_back(QString::fromStdWString(musicInfoWstr));//歌手添加进展示的歌曲列表中
            lpMusicInfo->GetValue(PKEY_Music_AlbumTitle, musicInfoValue);
            if (musicInfoValue->bstrVal != NULL)
                musicInfoWstr = (WCHAR*)musicInfoValue->bstrVal;
            else
                musicInfoWstr = L"";
            albumTitlesList.push_back(wstr2str_2UTF8(musicInfoWstr));//专辑名添加进MusicPlay模块的专辑名列表中
            l.push_back(QString::fromStdWString(musicInfoWstr));//专辑名添加进展示的歌曲列表中
            lpMusicInfo->GetValue(PKEY_Media_Duration, musicInfoValue);
            ULONGLONG MusicDuration = musicInfoValue->uhVal.QuadPart;// 100ns units, not milliseconds
            int Minutes = MusicDuration / 10000 / 1000 / 60;
            int Seconds = MusicDuration / 10000 / 1000 % 60;
            musicInfoWstr = L"";
            if (Minutes < 10)
                musicInfoWstr += L'0';
            musicInfoWstr += std::to_wstring(Minutes) + L":";
            if (Seconds < 10)
                musicInfoWstr += L'0';
            musicInfoWstr += std::to_wstring(Seconds);
            l.push_back(QString::fromStdWString(musicInfoWstr));
            delete musicInfoValue;
            lpMusicInfo->Release();
        }
        else
        {
            titlesList.push_back(qfi.fileName().toStdString());
            artistsList.push_back(std::vector<std::string>());
            albumTitlesList.push_back("");
            l.push_back(qfi.fileName());
            l.push_back("");
            l.push_back("");
            l.push_back("");
        }
        ui.musicListWidget->addRow(l);
        l.clear();
#endif //_WIN32
    }
    mp.setMusicsListVector(musicsList);
    mp.setBasicMusicsInformations(titlesList, artistsList, albumTitlesList);
    musicsList.clear();
    titlesList.clear();
    artistsList.clear();
    albumTitlesList.clear();
    ui.musicListWidget->setEditTriggers(QListView::NoEditTriggers);
}
void dpMusic::showLocalMusicsList()
{
    QString p = QInputDialog::getText(this, "请输入音乐文件夹路径", "请输入音乐文件夹路径");
#ifdef _DEBUG
    if (p.isEmpty())
    {
        p = "C:\\Users\\lyxyz5223\\Desktop\\LxMusicDownloads";
        p = "E:\\mp3播放器\\CloudMusic";
        //E:\mp3播放器\CloudMusic
    }
#endif // _DEBUG
    if (!p.isEmpty())
    {
        std::thread showLocalMusicsListThread(&dpMusic::showLocalMusicsListProc, this, p);
        showLocalMusicsListThread.detach();//脱离主线程，异步调用
    }
}
void dpMusic::listViewDoubleClicked(QModelIndex index)
{
    //while (ui.musicListWidget->model()->canFetchMore(index))
    //    ui.musicListWidget->model()->fetchMore(index);
    int rowCount = ui.musicListWidget->model()->rowCount();
    int columnCount = ui.musicListWidget->model()->columnCount();
    mp.setPlayingIndex(index.row()*(columnCount)+index.column()+1);
    mp.setFileNameUTF8(mp.getMusicsListVector().at(mp.getPlayingIndex() - 1));
    mp.setMusicTitleUTF8(mp.getTitlesListVector().at(mp.getPlayingIndex() - 1));
    //mp.setFileNameUTF8(ui.musicListWidget->model()->data(index).toString().toStdString());
    //if (mp.isPlaying())//在类里面已经实现了检测，无需再次检测
    mp.play();
    ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
    ui.musicInformation->setText(mp.getMusicTitleUTF8().c_str());
    mp.setEndCallback(onMusicEnd,this,&mp);
}

void dpMusic::MusicControlPlayPausedClicked()
{
    if (mp.isPaused() || !mp.isPlaying())//暂停或者不在播放
    {
        if (mp.getFileNameUTF8() != "")
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
    if (mp.getMusicsListVector().size())
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
        mp.setFileNameUTF8(mp.getMusicsListVector()[mp.getPlayingIndex() - 1]);
        mp.setMusicTitleUTF8(mp.getTitlesListVector()[mp.getPlayingIndex() - 1]);
        mp.play();
        ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
        ui.musicInformation->setText(mp.getMusicTitleUTF8().c_str());
        mp.setEndCallback(onMusicEnd, this, &mp);
    }
}

void dpMusic::MusicControlNextOneClicked()
{
    if (mp.getMusicsListVector().size())
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
        mp.setFileNameUTF8(mp.getMusicsListVector()[mp.getPlayingIndex() - 1]);
        mp.setMusicTitleUTF8(mp.getTitlesListVector()[mp.getPlayingIndex() - 1]);
        mp.play();
        ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
        ui.musicInformation->setText(mp.getMusicTitleUTF8().c_str());
        mp.setEndCallback(onMusicEnd, this, &mp);
    }
}

void dpMusic::maximumBtnClicked()
{
    if (isMaximized() || isFullScreen())
        showNormal();
    else
        showMaximized();
}
