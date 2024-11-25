//#define CPPHTTPLIB_OPENSSL_SUPPORT
//#include "httplib.h"
//#include "request.h"
//#include <iostream>

//void testHttpsRequest()
//{
//	using namespace std;
//	//https://music.163.com/api/search/pc?s=zjl&offset=0&type=1&limit=10
//	httplib::Client cli("https://music.163.com");
//	cli.enable_server_certificate_verification(false);
//	httplib::Result res = cli.Get("/api/search/pc?s=zjl&offset=0&type=1&limit=10");
//	if (res)
//		cout << res->body << endl;
//}

/*
The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MIT_License

The full text of the MIT License follows:

========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur and The JsonCpp Authors

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain "license" you can re-license your copy using whatever
license you like.
*/

#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include "stringProcess.h"
#include "request.h"
#include "json/json.h"
bool downloadFile(std::wstring url, std::wstring fileName)
{
	using namespace std;
	wstring parameter = L"-V --auto-file-renaming=false --allow-overwrite=true -o \"";
	parameter += fileName + L"\" \"" + url + L"\"";
	SHELLEXECUTEINFO sei = { 0 };
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpVerb = L"open";
	sei.lpFile = L"aria2c.exe";
	sei.lpParameters = parameter.c_str();
	sei.nShow = SW_NORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&sei);
	if (sei.hProcess == NULL)
		return false;
	WaitForSingleObject(sei.hProcess, INFINITE);
	CloseHandle(sei.hProcess);
	return true;
}
MusicsFullInfo searchMusic(std::string str, MusicType_wy type)
{
	using namespace std;
	wstring parameter = L"-V --auto-file-renaming=false --allow-overwrite=true -o ";
	wstring fileName = L"searchResult.json";
	wstring url = L"\"https://music.163.com/api/search/get/web?offset=0&total=true&limit=100&";
	url += L"s=";
	url += str2wstr_2UTF8(str);
	url += L"&type=";
	url += to_wstring(static_cast<int>(type)) + L"\"";
	parameter += L"\"" + fileName + L"\" " + url;
	SHELLEXECUTEINFO sei = { 0 };
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpVerb = L"open";
	sei.lpFile = L"aria2c.exe";
	sei.lpParameters = parameter.c_str();
	sei.nShow = SW_NORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&sei);
	if(sei.hProcess != NULL)
		WaitForSingleObject(sei.hProcess, INFINITE);
	CloseHandle(sei.hProcess);
	ifstream f(fileName);
	if (!f.is_open()) return {};
	using namespace Json;
	Reader reader;
	Value root;
	reader.parse(f, root);
	f.close();
	if (!reader.good()) return {};
	MusicsFullInfo result;
	root = root["result"]["songs"];
	for (ArrayIndex i = 0; i < root.size(); i++)
		if (root[i].type() == Json::objectValue)
		{
			result.musicsInformations.titlesList.push_back(root[i]["name"].asString());
			cout << UTF8ToANSI(root[i]["name"].asString()) << endl;
			Value artists = root[i]["artists"];
			Value::Members members;
			for (ArrayIndex j = 0; j < artists.size(); j++)
				members.push_back(artists[j]["name"].asString());
			result.musicsInformations.artistsList.push_back(members);
			result.musicsInformations.albumTitlesList.push_back(root[i]["album"]["name"].asString());
			result.musicsInformations.duration.push_back(root[i]["duration"].asLargestInt());
			string url = "https://music.163.com/song/media/outer/url?id=";//id=1992141905
			url += root[i]["id"].asString();
			result.musicFilesInfo.fileName.push_back("");
			result.musicFilesInfo.filePath.push_back(url);
		}
	return result;
}