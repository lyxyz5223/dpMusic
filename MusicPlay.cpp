#include "MusicPlay.h"
#include <thread>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

using namespace std;

MusicPlay::MusicPlay(std::string FileName, std::string Path)
{
    if (Path != "" && Path.back() != '\\' && Path.back() != '/')
        Path += '\\';
    path = Path;
    fileName = FileName;
    engineResult = ma_engine_init(NULL, &engine);
}

MusicPlay::MusicPlay()
{
    path = ".\\";
    fileName = "";
    engineResult = ma_engine_init(NULL, &engine);
}

void MusicPlay::play()
{
    //if (!multiPlay && isPlaying())
    //{
    //    ma_sound_uninit(&sound);
    //}
    if(engineResult == MA_SUCCESS)
    {
        ma_sound_uninit(&sound);
        uninit_sound = true;
        ma_result soundResult = ma_sound_init_from_file(&engine, (path + fileName).c_str(), 0, 0, 0, &sound);
        if (soundResult == MA_SUCCESS)
        {
            ma_sound_start(&sound);
        }
        //ma_engine_play_sound(&engine, (path + fileName).c_str(), NULL);
        playing = true;
    }
}

void MusicPlay::pause()
{
    if (!isPaused() && isPlaying())
    {
        //ma_sound_stop_with_fade_in_milliseconds(&sound,1000);
        //用了上述语句导致无法恢复播放
        ma_sound_stop(&sound);
        paused = true;
    }
    else
    {
        ma_sound_start(&sound);
        paused = false;
    }
}

void MusicPlay::stop()
{
    if (isPlaying())
    {
        ma_sound_uninit(&sound);
        playing = false;
        paused = false;
        finished = false;
    }
}

void on_ma_sound_end_proc_wait_for_sound_uninit_and_run_user_callback(void* pUserData, ma_sound* pSound)
{
    MusicPlay::ma_data* procData = (MusicPlay::ma_data*)pUserData;
    MusicPlay* mp = (MusicPlay*)procData->pUserData.musicplayClass;
    while (!mp->uninit_sound)
    {
        mp->uninit_sound = false;
    }
    procData->callBackFunction(&procData->pUserData, pSound);
}
void on_ma_sound_end_proc(void* pUserData, ma_sound* pSound)
{
    MusicPlay::ma_data* procData = (MusicPlay::ma_data*)pUserData;
    MusicPlay* mp = (MusicPlay*)procData->pUserData.musicplayClass;
    mp->setIsPlaying(false);
    mp->setIsPaused(false);
    if (mp->getIsAutoPlayNextOne())
    {
        auto index = mp->getPlayingIndex();
        mp->setPlayingIndex(mp->getNextPlayingIndex());
        mp->setFileNameUTF8(mp->getMusicsListVector()[mp->getPlayingIndex() - 1]);
        thread t(&MusicPlay::play,mp);
        t.detach();
    }
    thread t(on_ma_sound_end_proc_wait_for_sound_uninit_and_run_user_callback, &procData->pUserData, pSound);
    t.detach();
    //procData->callBackFunction(&procData->pUserData, pSound);
}

