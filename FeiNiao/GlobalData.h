#pragma once
#include "CLeftView.h"
#include "CRightView.h"
#include "CPageEditor.h"

#define  MSG_SCRIPT_CHANGE 1
#define  MSG_
class GlobalData
{
public:
	GlobalData();
	virtual ~GlobalData();
	std::map<string, string>& getScripts();
	int loadScripts();
	void setScriptSel(const string& index);
	void getScriptSel(string& fileName, string& filePath);

	void NotifyMessage(int msgCode);
	string& getScriptContent();
	
	void tryLoadInfo();
	void exeScript();

public: 
	static GlobalData& instance()
	{
		static GlobalData data;
		return data;
	}

	static void printMessage(const string& content);
	static void onNotifyData(int dataType, const std::map<string, string>& data);
	

	

	// 这里仅仅是引用
	static CLeftView* pLeftView;
	static CRightView* pRightView;

	static CPageMusic* pPageMusic;
	static CPageEditor* pPageEditor;
	static CPageVideo* pPageVideo;
	static CPageLog* pPageLog;

	
private:
	bool loadScript();
	
private:
	std::map<string, string> fileMap;
	string curSelected;
	string scriptContent;
};

