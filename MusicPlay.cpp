#include "MusicPlay.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

using namespace std;

MusicPlay::MusicPlay()
{
	engineResult = ma_engine_init(NULL, &engine);
}

void callEndCallBackFunc(void* pUserData, ma_sound* pSound)
{
	//先等待on_ma_sound_end_proc执行结束并返回，再执行回调函数，否则ma_sound_uninit会死循环

	MusicPlay::ma_data* procData = (MusicPlay::ma_data*)pUserData;
	MusicPlay* mp = (MusicPlay*)procData->pUserData.musicplayClass;
	mp->setStatus(MusicPlay::stopped);
	if (mp->isAutoPlayNextOne())
	{
		mp->setPlayingIndex(mp->getNextPlayingIndex());
		mp->play();
	}
	if (procData->callBackFunction != NULL)
		procData->callBackFunction(&procData->pUserData, pSound);
}
