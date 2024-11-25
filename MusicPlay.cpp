#include "MusicPlay.h"
#include <thread>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

using namespace std;

MusicPlay::MusicPlay()
{
	engineResult = ma_engine_init(NULL, &engine);
}

void callEndCallBackFunc(void* pUserData, ma_sound* pSound)
{
	//�ȵȴ�on_ma_sound_end_procִ�н��������أ���ִ�лص�����������ma_sound_uninit����ѭ��

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

void on_ma_sound_end_proc(void* pUserData, ma_sound* pSound)
{
	thread t(callEndCallBackFunc, pUserData, pSound);
	t.detach();
}

