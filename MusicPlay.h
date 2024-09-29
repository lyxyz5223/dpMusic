#pragma once
#include <irrKlang.h>
#include <string>
#include <vector>
#include <functional>

class MusicPlay
{
public:
	~MusicPlay() {}
	MusicPlay(std::string FileName, std::string Path = ".\\");
	MusicPlay();
	void play();
	void pause();
	void stop();
	bool isPaused() {
		return paused;
	}
	bool isPlaying() {
		return playing;
	}
	void setPath(std::string Path) {
		if (Path != "" && Path.back() != '\\' && Path.back() != '/')
			Path += '\\';
		path = Path;
	}
	std::string getPath() {
		return path;
	}
	void setFileName(std::string FileName) {
		fileName = FileName;
	}
	std::string getFileName() {
		return fileName;
	}
	void setMusicsListVector(std::vector<std::string> musicNameListVector) {
		musicsList = musicNameListVector;
	}
	std::vector<std::string> getMusicsListVector() {
		return musicsList;
	}
	void setPlayingIndex(int index) {
		playingIndex = index;
	}
	int getPlayingIndex() {
		return playingIndex;
	}
	int getNextPlayingIndex() {
		return (playingIndex < musicsList.size() ? playingIndex + 1 : 1);
	}
	void checkIsFinished();

private:
	std::string path = ".\\";//文件路径
	std::string fileName = "";//文件名
	irrklang::ISoundEngine* engine;//音乐播放Engine
	irrklang::ISound* sound;
	bool paused = false;
	bool playing = false;
	std::vector<std::string> musicsList;
	int playingIndex = 0;
	bool finished = false;
	irrklang::ik_f32 volume = 1;
	bool multiPlay = false;
};
