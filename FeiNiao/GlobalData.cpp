#include "pch.h"
#include "GlobalData.h"
#include <fstream>

#include "./include/LuaPlugin.h"
#include "./include/PathUtil.h"
#include "./include/StringUtils.h"
#include "./include/IniParser.h"

using namespace bird2fish;

CLeftView* GlobalData::pLeftView = nullptr;
CRightView* GlobalData::pRightView = nullptr;

CPageMusic* GlobalData::pPageMusic = nullptr;
CPageEditor* GlobalData::pPageEditor = nullptr;
CPageVideo* GlobalData::pPageVideo = nullptr;
CPageLog* GlobalData::pPageLog = nullptr;


void GlobalData::progressInfo(const char* url, size_t bytes, size_t all)
{
	if (pRightView)
	{
		std::map<string, string> vec;
		vec.insert(std::make_pair<string, string>("url", url));
		vec.insert(std::make_pair<string, string>("bytes", std::to_string(bytes)));
		if (all == 0)
			vec.insert(std::make_pair<string, string>("all", "--"));
		else
			vec.insert(std::make_pair<string, string>("all", std::to_string(all)));

		int lineIndex = GlobalData::instance().getUrlIndex(url);
		vec.insert(std::make_pair<string, string>("i", std::to_string(lineIndex)) );
		
		pRightView->SendMessage(MSG_DOWNLOAD_PROGRESS, lineIndex, (LPARAM)&vec);
	}
}
// interface for lua
void GlobalData::printMessage(const string& content)
{
	if (GlobalData::pRightView)
	{
		GlobalData::pRightView->showMsg(content);
	}
}

void GlobalData::onNotifyData(int dataType, const std::map<string, string>& data)
{
	for (auto it : data)
	{
		stringstream ss;
		ss << it.first << " : " << it.second;
		printMessage(ss.str());

	}
	switch (dataType)
	{
	case 0:
	case 1:
	case 2:
		if (pPageMusic)
		{
			pPageMusic->SendMessage(WM_NOTINY_USER_DATA, dataType, (LPARAM)(void *)&data);
			//pPageMusic->onNotifyData(dataType, data);
		}
		
	

	default:
		break;
	}
}


GlobalData::GlobalData()
{
	curSelected = "";
	workerThread = nullptr;
	loadConfig();
	LuaExtension::instance().load();
}

GlobalData::~GlobalData()
{
	stopThread();

}
std::map<string, string>& GlobalData::getScripts()
{
	return this->fileMap;
}

int GlobalData::getUrlIndex(const string& url)
{
	for (auto& item : musicVec)
	{
		if (item->url == url)
		{
			return item->index;
		}
	}
	return -1;
}

void EnumerateFiles(string &pathMain, string csPath, std::map<string, string>& strArray)
{
		string csNextPath = csPath.c_str();
		CFileFind ff;
		if (csPath.at(csPath.length() - 1) == '\\')
		{
			csPath += _T("*.*"); 
		}
		else
		{
			csPath += _T("\\*.*");
		}
		

		int nResult = ff.FindFile(csPath.c_str());
		while (nResult)
		{
			
			nResult = ff.FindNextFile();
			string fileName = (LPCTSTR)ff.GetFileName();

			if (ff.IsDirectory() && !ff.IsDots())
			{
				//cout << (LPCTSTR)ff.GetFilePath() << endl;
				string temp = csNextPath + "\\";
				temp += ff.GetFileName();
				EnumerateFiles(pathMain, temp, strArray);
			}
			else if (ff.IsDirectory())
			{
				
			}
			else
			{
				string curPath = (LPCTSTR)ff.GetFilePath();
				string curName = curPath.substr(pathMain.length()+1);

				//strArray[fileName] = curPath;
				strArray[curName] = curPath;
			}
			
		}
	
}

int GlobalData::loadScripts()
{
	this->fileMap.clear();
	string dir = PathUtil::getAppPath();

	string scriptDir;
	// ./bin/script
	scriptDir = PathUtil::combinePath(dir.c_str(), "script");
	EnumerateFiles(scriptDir, scriptDir, this->fileMap);

	// ./bin/../script
	scriptDir = PathUtil::combinePath(dir.c_str(), "../script");
	EnumerateFiles(scriptDir, scriptDir, this->fileMap);

	//// {currentDir}/script
	char buffer[1024];
	::GetCurrentDirectory(1024, buffer);
	scriptDir = PathUtil::combinePath(buffer, "script");
	EnumerateFiles(scriptDir, scriptDir, this->fileMap);

	stringstream ss;
	ss << "load " << (int)this->fileMap.size() << " scripts files.";
	this->printMessage(ss.str());

	return (int)this->fileMap.size();
}

void GlobalData::setScriptSel(const string& name)
{
	this->curSelected = name;
}

void GlobalData::getScriptSel(string& fileName, string& filePath)
{
	auto it = this->fileMap.find(this->curSelected);
	if (it == fileMap.end())
	{
		fileName = "";
		filePath = "";
	}
	else
	{
		fileName = it->first;
		filePath = it->second;
	}
}

void GlobalData::NotifyMessage(int msgCode)
{
	bool ret = false;

	switch (msgCode)
	{
	case MSG_SCRIPT_CHANGE:

		ret = loadScript();
		if (ret == false)
		{
			return;
		}
	
		// load and set info
		tryLoadInfo();
		

		if (GlobalData::pPageEditor)
		{
			GlobalData::pPageEditor->OnSetScript(scriptPath, getScriptContent());
		}

		LuaPlugin::localDir = dirDefault;

		break;
	case MSG_SCRIPT_SAVE:
		
			if (pPageEditor)
			{
				pPageEditor->OnSaveScript(scriptPath);
			}

	default:
		break;
	}
}
string& GlobalData::getScriptContent()
{
	return this->scriptContent;
}

bool GlobalData::loadScript()
{
	string fileName; 
	getScriptSel(fileName, scriptPath);
	if (fileName == "")
		return false;

	this->scriptContent = FileUtil::readFileAsString(scriptPath.c_str());

	return true;
}

// 尝试加载脚本
void GlobalData::tryLoadInfo()
{
	if (GlobalData::pRightView == nullptr)
		return;

	string fileName;
	string filePath;
	getScriptSel(fileName, filePath);
	if (fileName == "")
		return;

	LuaPlugin lua;
	lua.setcallbackFunc(GlobalData::printMessage, GlobalData::onNotifyData);
	lua.create();
	bool ret = lua.tryLoad(filePath);
	if (ret == false)
		return;
	
	pRightView->setInfo(0, lua.getScriptInfo(SCRIPT_DESC));
	pRightView->setInfo(1, lua.getScriptInfo(SCRIPT_INPUT1));
	pRightView->setInfo(2, lua.getScriptInfo(SCRIPT_INPUT2));
	pRightView->setInfo(3, lua.getScriptInfo(SCRIPT_INPUT3));

	stringstream ss;
	ss << "选择脚本【" << lua.getScriptInfo(SCRIPT_AUTHOR) << "】";
	printMessage(ss.str());
	
}

void GlobalData::exeScript(GlobalData* lpData)
{
	if (GlobalData::pRightView == nullptr)
		return;

	string fileName;
	string filePath;
	lpData->getScriptSel(fileName, filePath);
	if (fileName == "")
	{
		printMessage("please select one script first");
		return;
	}
	LuaPlugin lua;

	pRightView->onStart();
	
	// don't forget to set this function, or will nothing output
	lua.setcallbackFunc(GlobalData::printMessage, GlobalData::onNotifyData);

	lua.create();

	printMessage("开始执行脚本 ");

	lua.dowork(filePath, lpData->searchKeys);
	printMessage("脚本执行结束 ");
	lpData->onStopThread();

}

void GlobalData::exeDownloadMusic(GlobalData* lpData)
{
	HttpClient client;
	printMessage("开始下载 ");
	
	for (auto& item : lpData->musicVec)
	{
		HttpHeader header;
		string ext = PathUtil::getExt(item->url);
		string name = item->downloadName;
		string filePath = PathUtil::combinePath(lpData->dirDefault.c_str(), name.c_str());

		string info = "准备下载：" + std::to_string(item->index +1) + " " + filePath;
		printMessage(info);
		client.getAsFile(item->url, header, filePath);
	}
	printMessage("下载结束 ");
	lpData->onStopThread();
}

void GlobalData::onStopThread()
{
	if (pRightView)
		pRightView->onStop();

}
void GlobalData::startThread(int mode)
{
	stopThread();
	if (mode == 0)
	{
		this->workerThread = std::make_shared<std::thread>(GlobalData::exeScript, this);
	}
	else if (mode == 1)
	{
		this->workerThread = std::make_shared<std::thread>(GlobalData::exeDownloadMusic, this);
	}
	

}

void GlobalData::stopThread()
{
	if (this->workerThread)
	{
		if (workerThread->joinable())
		{
			workerThread->join();

		}
		workerThread = nullptr;
	}
}

void GlobalData::setSearchkeys(std::vector<string>& other)
{
	this->searchKeys = other;
}

void GlobalData::setMusicVec(MusicVector& other)
{
	this->musicVec = other;
}

void GlobalData::loadConfig()
{
	string curDir = PathUtil::getAppPath();
	std::string path = PathUtil::combinePath(curDir.c_str(), "downloader.ini");

	IniParser iniFile;
	bool ret = iniFile.load(path);
	string& error = iniFile.getErrorMsg();

	if (error.length() > 0)
		printf("loading cpp_agent err = %s \n", error.c_str());

	// enable agent or not
	dirDefault = iniFile.getValue("global", "dir");
	 
}

string GlobalData::getDefaultDir()
{
	return dirDefault;
}

void GlobalData::setDefaultDir(const string & str)
{
	dirDefault = str;
	LuaPlugin::localDir = dirDefault;

	string curDir = PathUtil::getAppPath();
	std::string path = PathUtil::combinePath(curDir.c_str(), "downloader.ini");

	::WritePrivateProfileString("global", "dir", str.c_str(), path.c_str());
}