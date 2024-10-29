#pragma once
#include <string>
#include <vector>
#include <functional>
#include <miniaudio.h>
#include "stringProcess.h"

void on_ma_sound_end_proc(void* pUserData, ma_sound* pSound);

class MusicPlay
{
public:
	~MusicPlay() {
		ma_engine_uninit(&engine);
	}
	MusicPlay(std::string FileName, std::string Path = ".\\");
	MusicPlay();
	void play();
	void pause();
	void stop();
	bool isPaused() const {
		return paused;
	}
	bool isPlaying() const {
		return playing;
	}
	void setIsPaused(bool TrueOrFalse) {
		paused = TrueOrFalse;
	}
	void setIsPlaying(bool TrueOrFalse) {
		playing = TrueOrFalse;
	}
	void setPathUTF8(std::string Path) {
		Path = UTF8ToANSI(Path);
		if (Path != "" && Path.back() != '\\' && Path.back() != '/')
			Path += '\\';
		path = Path;
	}
	void setPathANSI(std::string Path) {
		if (Path != "" && Path.back() != '\\' && Path.back() != '/')
			Path += '\\';
		path = Path;
	}
	std::string getPathUTF8() 
	{
		return ANSIToUTF8(path);
	}
	std::string getPathANSI() {
		return path;
	}
	void setFileNameUTF8(std::string FileName) {
		FileName = UTF8ToANSI(FileName);
		fileName = FileName;
	}
	void setFileNameANSI(std::string FileName) {
		fileName = FileName;
	}
	std::string getFileNameANSI() {
		return fileName;
	}
	std::string getFileNameUTF8() {
		return ANSIToUTF8(fileName);
	}
	void setMusicTitleUTF8(std::string MusicTitle) {
		MusicTitle = UTF8ToANSI(MusicTitle);
		musicTitle = MusicTitle;
	}
	void setMusicTitleANSI(std::string MusicTitle) {
		musicTitle = musicTitle;
	}
	std::string getMusicTitleANSI() {
		return musicTitle;
	}
	std::string getMusicTitleUTF8() {
		return ANSIToUTF8(musicTitle);
	}
	void setTitlesListVector(std::vector<std::string> titlesVector) {
		titlesList = titlesVector;
	}
	std::vector<std::string> getTitlesListVector() {
		return titlesList;
	}
	void setArtistsListVector(std::vector<std::vector<std::string>> artistsVector) {
		artistsList = artistsVector;
	}
	std::vector<std::vector<std::string>> getArtistsListVector() {
		return artistsList;
	}
	void setAlbumTitlesListVector(std::vector<std::string> albumTitlesVector) {
		albumTitlesList = albumTitlesVector;
	}
	std::vector<std::string> getAlbumTitlesListVector() {
		return albumTitlesList;
	}
	void setBasicMusicsInformations(
		std::vector<std::string> titlesVector,
		std::vector<std::vector<std::string>> artistsVector,
		std::vector<std::string> albumTitlesVector) {
		titlesList = titlesVector;
		artistsList = artistsVector;
		albumTitlesList = albumTitlesVector;
	}
	void setMusicsListVector(std::vector<std::string> musicNameListVector) {
		musicsList = musicNameListVector;
	}
	std::vector<std::string> getMusicsListVector() {
		return musicsList;
	}
	//PlayingIndex从1开始！
	void setPlayingIndex(int index) {
		playingIndex = index;
	}
	//PlayingIndex从1开始！
	int getPlayingIndex() {
		return playingIndex;
	}
	int getNextPlayingIndex() {
		return (playingIndex < musicsList.size() ? playingIndex + 1 : 1);
	}
	void setIsAutoPlayNextOne(bool TrueOrFalse) {
		autoPlayNextOne = TrueOrFalse;
	}
	bool getIsAutoPlayNextOne() {
		return autoPlayNextOne;
	}
	ma_sound getSound() {
		return sound;
	}
	//proc example:
	//void CallBackProc(void* pUserData, ma_sound* pSound)
	//{
	//	//TODO:
	//}
	//设置播放完成后的回调函数
	void setEndCallback(ma_sound_end_proc proc/*void* pUserData, ma_sound* pSound*/,void*parentClass,void*musicplayClass) {
		procData = { {parentClass,musicplayClass}, proc };
		sound.endCallback = on_ma_sound_end_proc;//先经过类本身的处理，再发放给用户处理
		sound.pEndCallbackUserData = &procData;//给类处理的数据
	}
	struct ma_sound_pUserData
	{
		void* parentClass;
		void* musicplayClass;
	};
	struct ma_data {
		ma_sound_pUserData pUserData;//用户处理数据，包含MusicPlay类和dpMusic类
		ma_sound_end_proc callBackFunction;//用户的回调函数
	} procData;
	bool uninit_sound = false;

private:
	std::string path = ".\\";//文件路径
	std::string fileName = "";//文件名
	std::string musicTitle = "";//歌曲名
	bool paused = false;
	bool playing = false;
	//以下几个容器中的字符串（包括容器的容器中的）均为UTF-8编码格式下的字符串
	std::vector<std::string> musicsList;
	std::vector<std::string> titlesList;
	std::vector<std::vector<std::string>> artistsList;
	std::vector<std::string> albumTitlesList;
	int playingIndex = 0;//从1开始！
	bool finished = false;
	bool multiPlay = false;
	bool autoPlayNextOne = true;
	ma_result engineResult;
	ma_engine engine;//音乐播放Engine
	ma_sound sound;
};
