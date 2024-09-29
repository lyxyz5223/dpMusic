#include "MusicPlay.h"
#include <thread>
using namespace std;

MusicPlay::MusicPlay(std::string FileName, std::string Path)
{
    if (Path != "" && Path.back() != '\\' && Path.back() != '/')
        Path += '\\';
    path = Path;
    fileName = FileName;
    engine = irrklang::createIrrKlangDevice();
}

MusicPlay::MusicPlay()
{
    path = ".\\";
    fileName = "";
    engine = irrklang::createIrrKlangDevice();
}

void MusicPlay::play()
{
    if (!multiPlay && isPlaying())
        sound->stop();
    if (engine)
    {
        engine->setSoundVolume(volume);
        sound = engine->play2D((path + fileName).c_str(),false,false,true);
        playing = true;
    }
}

void MusicPlay::pause()
{
    if (!isPaused() && isPlaying())
    {
        sound->setIsPaused(true);
        paused = true;
    }
    else
    {
        sound->setIsPaused(false);
        paused = false;
    }
}

void MusicPlay::stop()
{
    if (isPlaying())
    {
        sound->stop();
        playing = false;
        paused = false;
        finished = false;
    }
}

