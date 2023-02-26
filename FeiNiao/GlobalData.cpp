#include "pch.h"
#include "GlobalData.h"
#include <fstream>

#include "./include/LuaPlugin.h"
#include "./include/PathUtil.h"

using namespace bird2fish;

CLeftView* GlobalData::pLeftView = nullptr;
CRightView* GlobalData::pRightView = nullptr;

CPageMusic* GlobalData::pPageMusic = nullptr;
CPageEditor* GlobalData::pPageEditor = nullptr;
CPageVideo* GlobalData::pPageVideo = nullptr;
CPageLog* GlobalData::pPageLog = nullptr;


// interface for lua
void GlobalData::printMessage(const string& content)
{
	if (pRightView)
	{
		pRightView->showMsg(content);
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
}


GlobalData::GlobalData()
{
	curSelected = "";
}

GlobalData::~GlobalData()
{


}
std::map<string, string>& GlobalData::getScripts()
{
	return this->fileMap;
}

void EnumerateFiles(string csPath, std::map<string, string>& strArray)
{

		string csPrePath = csPath.c_str();
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
				csNextPath += ff.GetFileName();
				csNextPath += _T("\\");
				EnumerateFiles(csNextPath, strArray);
			}
			else if (ff.IsDirectory())
			{
				
			}
			else
			{
				strArray[fileName] = (LPCTSTR)ff.GetFilePath();
			}
			csNextPath = csPrePath;
		}
	
}

int GlobalData::loadScripts()
{
	this->fileMap.clear();
	string dir = PathUtil::getAppPath();

	// ./bin/script
	string scriptDir = PathUtil::combinePath(dir.c_str(), "script");
	EnumerateFiles(scriptDir, this->fileMap);

	// ./bin/../script
	scriptDir = PathUtil::combinePath(dir.c_str(), "../script");
	EnumerateFiles(scriptDir, this->fileMap);

	// {currentDir}/script
	char buffer[1024];
	::GetCurrentDirectory(1024, buffer);
	scriptDir = PathUtil::combinePath(buffer, "script");
	EnumerateFiles(scriptDir, this->fileMap);

	stringstream ss;
	ss << "load " << (int)this->fileMap.size() << " scripts files.";
	this->printMessage(ss.str());

	return (int)this->fileMap.size();
}

void GlobalData::setScriptSel(const string& name)
{
	this->curSelected = name;
	stringstream ss;
	ss << "current selected file" << name;
	printMessage(ss.str());

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
			GlobalData::pPageEditor->OnSetScript(getScriptContent());
		}

		break;

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
	string filePath;
	getScriptSel(fileName, filePath);
	if (fileName == "")
		return false;

	this->scriptContent = FileUtil::readFileAsString(filePath.c_str());

	return true;
}

// ³¢ÊÔ¼ÓÔØ½Å±¾
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
	
}

void GlobalData::exeScript()
{
	if (GlobalData::pRightView == nullptr)
		return;

	string fileName;
	string filePath;
	getScriptSel(fileName, filePath);
	if (fileName == "")
	{
		printMessage("please select one script first");
		return;
	}
	LuaPlugin lua;
	
	// don't forget to set this function, or will nothing output
	lua.setcallbackFunc(GlobalData::printMessage, GlobalData::onNotifyData);

	lua.create();

	printMessage("begin to execute script ");
	std::vector<string> args = pRightView->getArgs();
	lua.dowork(filePath, args);
	printMessage("finish ");
}