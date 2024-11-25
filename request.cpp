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
std::vector<std::string> searchMusic(std::string str, std::string type)
{
	using namespace std;
	wstring parameter = L"-V --auto-file-renaming=false --allow-overwrite=true -o ";
	wstring fileName = L"searchResult.json";
	wstring url = L"https://music.163.com/api/search/get/web?s=zjl&type=1&offset=0&total=true&limit=20";
	parameter += fileName + L" " + url;
	SHELLEXECUTEINFO sei = { 0 };
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpVerb = L"open";
	sei.lpFile = L"aria2c.exe";
	sei.lpParameters = parameter.c_str();
	sei.nShow = SW_NORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE);
	CloseHandle(sei.hProcess);
	ifstream f(fileName);
	if (!f.is_open()) return vector<string>();
	using namespace Json;
	Reader reader;
	Value root;
	reader.parse(f, root);
	f.close();
	if(!reader.good()) return vector<string>();
	vector<string> result;
	root = root["result"]["songs"];
	for (int i = 0; i < root.size(); i++)
		if (root[i].type() == Json::objectValue)
		{
			result.push_back(root[i]["name"].asString());
			cout << UTF8ToANSI(root[i]["name"].asString()) << endl;
		}
	return result;
}