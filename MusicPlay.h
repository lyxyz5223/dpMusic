#pragma once
#include <string>
#include <vector>
#include <functional>
#include <miniaudio.h>
#include <thread>
#include "stringProcess.h"
void callEndCallBackFunc(void* pUserData, ma_sound* pSound);

class MusicPlay
{
public:
	struct MusicsInformations {
		std::vector<std::string> titlesList;
		std::vector<std::vector<std::string>> artistsList;
		std::vector<std::string> albumTitlesList;
		std::vector<long long> duration;
	};
	struct MusicFilesInfo {
		std::vector<std::string> fileName;
		std::vector<std::string> filePath;
	};
	struct MusicPlayInfo {
		int playingIndex = 0;//从1开始！
		bool autoPlayNextOne = true;
	};
	enum MusicPlayStatus {
		playing,
		paused,
		stopped
	};
	enum MusicPlayMode {
		online, local
	};
	MusicPlay();
	~MusicPlay() {
		ma_engine_uninit(&engine);
	}
	//音乐控制函数
	void play()
	{
		if (engineResult == MA_SUCCESS)
		{
			ma_sound_uninit(&sound);
			ma_result soundResult = ma_sound_init_from_file_w(&engine, 
				str2wstr_2UTF8(filesInfo.filePath[getPlayingIndex() - 1] + filesInfo.fileName[getPlayingIndex() - 1]).c_str(),
				0, 0, 0, &sound);
			if (soundResult == MA_SUCCESS)
			{
				sound.endCallback = [](void* pUserData, ma_sound* pSound) {
					std::thread t(callEndCallBackFunc, pUserData, pSound);
					t.detach();
					};
				sound.pEndCallbackUserData = &procData;
				status = playing;
				ma_sound_start(&sound);
			}
			//ma_engine_play_sound(&engine, (path + fileName).c_str(), NULL);
		}
	}
	void pause() {
		if (isPaused())
		{
			ma_sound_start(&sound);
			status = playing;
		}
		else {
			//ma_sound_stop_with_fade_in_milliseconds(&sound,1000);
			//用了上述语句导致无法恢复播放
			ma_sound_stop(&sound);
			status = paused;
		}
	}
	void stop()
	{
		if (isPlaying() || isPaused())
		{
			ma_sound_uninit(&sound);
			status = stopped;
		}
	}
	bool isPaused() const { 
		return (status == paused ? true : false); 
	}
	bool isPlaying() const { 
		return (status == playing ? true : false); 
	}
	bool isStopped() const { 
		return (status == stopped ? true : false); 
	}
	void setMusicsInformations(std::vector<std::string> titlesVector, 
		std::vector<std::vector<std::string>> artistsVector, 
		std::vector<std::string> albumTitlesVector) {
		musicsInfo.titlesList = titlesVector;
		musicsInfo.artistsList = artistsVector;
		musicsInfo.albumTitlesList = albumTitlesVector;
	}
	/*---*/auto getMusicsInformations() const { 
		return musicsInfo; 
	}
	void setMusicFilesInformations(std::vector<std::string> fileName,
		std::vector<std::string> filePath) {
		for (std::vector<std::string>::iterator iter = filePath.begin(); iter != filePath.end(); iter++)
			if (*iter != "" && (*iter).back() != '\\' && (*iter).back() != '/')
				*iter += '\\';
		filesInfo.fileName = fileName;
		filesInfo.filePath = filePath;
	}
	/*---*/auto getMusicFilesInformations() {
		return &filesInfo;
	}
	//PlayingIndex从1开始！
	void setPlayingIndex(size_t index) {
		if (index < 1 || index > musicsInfo.titlesList.size())
			throw "error index invalid";
		mpInfo.playingIndex = index;
	}
	//PlayingIndex从1开始！
	int getPlayingIndex() const {
		return mpInfo.playingIndex;
	}
	int getNextPlayingIndex() const {
		return (mpInfo.playingIndex < musicsInfo.titlesList.size() ? mpInfo.playingIndex + 1 : 1);
	}
	void setAutoPlayNextOne(bool trueOrFalse) {
		mpInfo.autoPlayNextOne = trueOrFalse;
	}
	bool isAutoPlayNextOne() const {
		return mpInfo.autoPlayNextOne;
	}
	MusicPlayMode getMode() const {
		return mode;
	}
	//类里调用的函数，切勿在类外调用
	void setStatus(MusicPlayStatus mpStatus) {
		status = mpStatus;
	}
	void setMode(MusicPlayMode mpMode) {
		mode = mpMode;
	}
	//proc example:
	//void CallBackProc(void* pUserData, ma_sound* pSound)
	//{
	//	//TODO:
	//}
	//设置播放完成后的回调函数
	void setEndCallback(ma_sound_end_proc proc/*void* pUserData, ma_sound* pSound*/,void*parentClass,void*musicplayClass) {
		procData = { {parentClass,musicplayClass}, proc };
		sound.endCallback = [](void* pUserData, ma_sound* pSound) {
			std::thread t(callEndCallBackFunc, pUserData, pSound);
			t.detach();
			};//先经过类本身的处理，再发放给用户处理
		sound.pEndCallbackUserData = &procData;//给类处理的数据
	}
	struct ma_sound_pUserData
	{
		void* parentClass;
		void* musicplayClass;
	};
	struct ma_data {
		ma_sound_pUserData pUserData;//用户处理数据，包含MusicPlay类和dpMusic类
		ma_sound_end_proc callBackFunction = 0;//用户的回调函数
	} procData;
private:
	//以下几个容器中的字符串（包括容器的容器中的）均为UTF-8编码格式下的字符串
	MusicsInformations musicsInfo;
	MusicFilesInfo filesInfo;
	MusicPlayInfo mpInfo;
	MusicPlayStatus status = stopped;
	ma_result engineResult;
	ma_engine engine;//音乐播放Engine
	ma_sound sound;
	MusicPlayMode mode = local;
};
