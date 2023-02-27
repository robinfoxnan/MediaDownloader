#pragma once
#include "CLeftView.h"
#include "CRightView.h"
#include "CPageEditor.h"

class MusicItem 
{
public:
	int index;   // �к�
	string singer;
	string song;
	string downloadName;
	string url;
	int percent;
};

using MusicItemPtr = std::shared_ptr<MusicItem>;
using MusicVector = std::vector<MusicItemPtr>;

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
	
	void startThread();
	void stopThread();
	void onStopThread();
	void setSearchkeys(std::vector<string> & other);
	void setMusicVec(MusicVector & other);

public: 
	static GlobalData& instance()
	{
		static GlobalData data;
		return data;
	}

	static void exeScript(GlobalData* lpData);
	static void printMessage(const string& content);
	static void onNotifyData(int dataType, const std::map<string, string>& data);
	

	

	// �������������
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

	std::shared_ptr<std::thread> workerThread;
	std::vector<string> searchKeys;
	MusicVector musicVec;
};

