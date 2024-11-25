#pragma once
#include <string>
#include <vector>
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
struct MusicsFullInfo {
	MusicsInformations musicsInformations;
	MusicFilesInfo musicFilesInfo;
};
// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
enum MusicType_wy {
	songs = 1,
	albums = 10,
	singers = 100,
	playlists = 1000,
	users = 1002,
	mvs = 1004,
	lyrics = 1006,
	radios = 1009,
	videos = 1014
};
bool downloadFile(std::wstring url, std::wstring fileName);
MusicsFullInfo searchMusic(std::string str, MusicType_wy type);