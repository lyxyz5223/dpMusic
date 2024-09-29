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
        thread t(&MusicPlay::checkIsFinished,this);
        t.detach();
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
class MyISoundStopEventReceiver : public irrklang::ISoundStopEventReceiver
{
public:
    MyISoundStopEventReceiver() {
    }
    virtual void OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
    {
        // called when the sound has ended playing
        printf("Sound has ended.\n");
        if (reason == irrklang::ESEC_SOUND_FINISHED_PLAYING)
        {
            MusicPlay* mp = (MusicPlay*)userData;
            auto index = mp->getPlayingIndex();
            mp->setPlayingIndex(mp->getNextPlayingIndex());
            mp->setFileName(mp->getMusicsListVector()[mp->getPlayingIndex()-1]);
            mp->play();
        }
    }
};

void MusicPlay::checkIsFinished()
{
    MyISoundStopEventReceiver* my = new MyISoundStopEventReceiver();
    sound->setSoundStopEventReceiver(my,this);

}

