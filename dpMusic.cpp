#include "dpMusic.h"
#include <QInputDialog>
#include "request.h"
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
    if (mp->isAutoPlayNextOne())
    {
        dp->getUi().musicInformation->setText(mp->getMusicsInformations().titlesList[mp->getPlayingIndex() - 1].c_str());
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
void dpMusic::showHomepage()
{
    showOnlineMusicsList(searchMusic("好运来", "title"));
}
void dpMusic::showOnlineMusicsList(std::vector<std::string> musicsList)
{
    QStandardItemModel* model = new QStandardItemModel;
    for (int i = 0; i < musicsList.size(); i++)
    {
        QList<QStandardItem*> items;
        for (int i = 0; i < 4; i++) items << new QStandardItem();
        items[0]->setText(QString::fromStdString(musicsList[i]));
        model->appendRow(items);
    }
    ui.musicListView->setModel(model);
}
void dpMusic::showLocalMusicsListProc(QString path)
{
    QDir dir;
    dir.setPath(path);
    QFileInfoList fil;
    QStringList strList;//文件列表，歌曲信息
    std::vector<std::string> musicsFileNameList;//歌曲文件名称信息
    std::vector<std::string> musicsFilePathList;//歌曲文件路径信息
    std::vector<std::string> titlesList;//歌曲标题信息
    std::vector<std::vector<std::string>> artistsList;//歌曲歌手信息
    std::vector<std::string> albumTitlesList;//歌曲专辑名信musicsFilePathList息
    QStringList filterList;//后缀过滤
    filterList << "*.flac" << "*.mp3" << "*.wav" << "*.ogg" << "*.ape";
    fil = dir.entryInfoList(filterList, QDir::Filter::Files, QDir::SortFlag::Name);
    //ui.musicListView->setResizeMode(QListView::Adjust);
    QStandardItemModel* model = new QStandardItemModel;
    ui.musicListView->setAutoScroll(true);
    ui.musicListView->reset();
    for (QFileInfo qfi : fil)//遍历列表
    {
        QList<QStandardItem*> items;
        for (int i = 0; i < 4; i++) items << new QStandardItem();
        musicsFileNameList.push_back(qfi.fileName().toStdString());
        musicsFilePathList.push_back(qfi.absolutePath().toStdString());
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
            //获取歌曲标题
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
            items[0]->setText(QString::fromStdWString(musicInfoWstr));
            //获取歌曲艺术家
            musicInfoWstr.clear();
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
            items[1]->setText(QString::fromStdWString(musicInfoWstr));
            //获取专辑名
            lpMusicInfo->GetValue(PKEY_Music_AlbumTitle, musicInfoValue);
            if (musicInfoValue->bstrVal != NULL)
                musicInfoWstr = (WCHAR*)musicInfoValue->bstrVal;
            else
                musicInfoWstr.clear();
            albumTitlesList.push_back(wstr2str_2UTF8(musicInfoWstr));//专辑名添加进MusicPlay模块的专辑名列表中
            l.push_back(QString::fromStdWString(musicInfoWstr));//专辑名添加进展示的歌曲列表中
            items[2]->setText(QString::fromStdWString(musicInfoWstr));
            //获取歌曲时长
            lpMusicInfo->GetValue(PKEY_Media_Duration, musicInfoValue);
            ULONGLONG MusicDuration = musicInfoValue->uhVal.QuadPart;// 100ns units, not milliseconds
            int Minutes = MusicDuration / 10000 / 1000 / 60;
            int Seconds = MusicDuration / 10000 / 1000 % 60;
            musicInfoWstr.clear();
            if (Minutes < 10) musicInfoWstr += L'0';
            musicInfoWstr += std::to_wstring(Minutes) + L":";
            if (Seconds < 10) musicInfoWstr += L'0';
            musicInfoWstr += std::to_wstring(Seconds);
            l.push_back(QString::fromStdWString(musicInfoWstr));
            items[3]->setText(QString::fromStdWString(musicInfoWstr));
            delete musicInfoValue;
            lpMusicInfo->Release();
        }
        else
        {
            titlesList.push_back(qfi.fileName().toStdString());
            artistsList.push_back(std::vector<std::string>());
            albumTitlesList.push_back("");
            items[0]->setText(qfi.fileName());
            l << qfi.fileName() << "" << "" << "";
        }
        model->appendRow(items);
        ui.musicListView->setModel(model);
#endif //_WIN32
    }
    mp.setMusicsInformations(titlesList, artistsList, albumTitlesList);
    mp.setMusicFilesInformations(musicsFileNameList, musicsFilePathList);
    ui.musicListView->setEditTriggers(QListView::NoEditTriggers);
}
void dpMusic::showLocalMusicsList()
{
    QString p = QInputDialog::getText(this, "请输入音乐文件夹路径", "请输入音乐文件夹路径");
#ifdef _DEBUG
    if (p.isEmpty())
    {
        p = "C:\\Users\\lyxyz5223\\Desktop\\LxMusicDownloads";
        //p = "E:\\mp3播放器\\CloudMusic";
        //E:\mp3播放器\CloudMusic
    }
#endif // _DEBUG
    if (!p.isEmpty())
    {
        std::thread showLocalMusicsListThread(&dpMusic::showLocalMusicsListProc, this, p);
        showLocalMusicsListThread.detach();//脱离主线程，异步调用
        //showLocalMusicsListProc(p);
    }
}
void dpMusic::listViewDoubleClicked(QModelIndex index)
{
    mp.setPlayingIndex(index.row() + 1);
    mp.play();
    ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
    ui.musicInformation->setText(mp.getMusicsInformations().titlesList[mp.getPlayingIndex() - 1].c_str());
    mp.setEndCallback(onMusicEnd,this,&mp);
}

void dpMusic::MusicControlPlayPausedClicked()
{
    if (!mp.isPlaying())//不在播放
    {
        if (mp.getPlayingIndex() != 0)
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
    if (mp.getMusicsInformations().titlesList.size())
    {

        if (mp.getPlayingIndex() > 1)
        {
            mp.setPlayingIndex(mp.getPlayingIndex() - 1);
        }
        else
        {
            //QMessageBox::information(this, "~~~", "这已经是第一首了哦~");
            mp.setPlayingIndex(mp.getMusicsInformations().titlesList.size());
        }
        mp.play();
        ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
        ui.musicInformation->setText(mp.getMusicsInformations().titlesList[mp.getPlayingIndex() - 1].c_str());
        mp.setEndCallback(onMusicEnd, this, &mp);
    }
}

void dpMusic::MusicControlNextOneClicked()
{
    size_t musicsListSize = mp.getMusicsInformations().titlesList.size();
    if (musicsListSize)
    {
        if (mp.getPlayingIndex() < musicsListSize)
        {
            mp.setPlayingIndex(mp.getPlayingIndex() + 1);
        }
        else
        {
            //QMessageBox::information(this, "~~~", "这已经是最后一首了哦~");
            mp.setPlayingIndex(1);
        }
        mp.play();
        ui.play->setIcon(QIcon(":/dpMusic/src/svgs/feather/pause.svg"));
        ui.musicInformation->setText(mp.getMusicsInformations().titlesList[mp.getPlayingIndex() - 1].c_str());
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
