#pragma once
#include <iostream>
#include <string>
#include <map>
#include "json11.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge.h>
#pragma comment(lib, "Lua54.lib")

#include "CUrlAgent.h"
#include "FileUtil.h"
#include "UrlEncoding.h"
#include <functional>

#include "LuaExtension.h"

using namespace std;
using namespace luabridge;
using namespace luabridge::detail;
using namespace json11;

namespace bird2fish
{
	enum  SCRIPT_INFO
	{
		SCRIPT_DESC = 0,
		SCRIPT_INPUT1 = 1,
		SCRIPT_INPUT2 = 2,
		SCRIPT_INPUT3 = 3,
		SCRIPT_AUTHOR = 4
	};

	// typedef functions
	using PrintMsgFunc = std::function<void(const std::string&)>;
	using NotifyDataFunc = std::function<void(int, const std::map<string, string>&)>;

	// ���еĴ�����Ϊ�ӿڸ�LUA�ű�ʹ��
	void printMessage(const std::string& s);
	double getEngineVersion();
	string getEngineName();
	void renameOsFile(lua_State* L);

	// JSON����ȫ�ֺ���
	Json intToJson(int i);
	Json numToJson(double i);
	Json stringToJson(const string& str);
	Json objectToJson(json11::Json::object& obj);
	Json parseJsonStr(const string& str);
	Json parseJsonFile(const string& filePath);

	

	// ����C++չ��lua��һ��ȱ�㣬��C++�޷����������غ���
	// ��ʵ���࣬���ڴ������header��Ϣ
	class HttpHeader
	{
	public:
		HttpHeader()
		{
		}
		HttpHeader(const std::map<string, string>& other)
		{
			this->items = other;
		}
		HttpHeader(const std::map<string, string>&& other)
		{
			this->items = other;
		}
	public:
		void setItem(const string& key, const string& value);
		size_t size() { return items.size(); }
		std::map<string, string>& getItems() { return items; }
		string getItem(const string& key)
		{
			auto pos = items.find(key);
			if (pos != items.end())
				return pos->second;
			return "";
		}
		luabridge::LuaRef getPairsTable(lua_State* luastate);

	private:
		std::map<string, string>  items;
	};

	// �ͻ����࣬���ڴ���GET��POST
	class HttpClient
	{
	public:
		HttpClient()
		{
			body = "";
		}

		int getAsString(const string& url, const HttpHeader& header);
		int getAsFile(const string& url, const HttpHeader& header, const string& filePath);

		HttpHeader& getHeader();
		int getErr();
		int64_t getFileSize();
		int dataType();

		const string& getBodyAsString();
		string getBodyAsAnsiString();
		json11::Json getBodyAsJson();

	private:
		string host;
		HttpHeader headers;
		string body;
		int errCode;
		size_t fileSize;
	};


	
	class LuaPlugin {

	public:
		LuaPlugin():L(nullptr)
		{
			this->printFunc = printMessage;
		};
		virtual ~LuaPlugin()
		{
			close();
		}
		static string localDir;

	public:
		// ��ʼ��ȫ�ָ��ֽӿں���
		bool registerGlobal(lua_State* L);
		int dowork(const string& filePath, std::vector<string>& args);
		bool tryLoad(const string& filePath);
		void loadInfo();
		string getScriptInfo(SCRIPT_INFO index);
		void handleTable(luabridge::LuaRef myInt, luabridge::LuaRef table);
		void myPrint(luabridge::LuaRef param);

		static bool createDir(const string& filePath);
		

		void setcallbackFunc(PrintMsgFunc p, NotifyDataFunc  n)
		{
			if (p)
				this->printFunc = p;
			if (n)
				this->notifyDataFunc = n;
		}

		lua_State* create() {
			if (L)
				return L;

			this->L = luaL_newstate();
			// �� Lua ��׼����ص� Lua ������
			luaL_openlibs(L);

			// ���Զ��� Lua ģ�����ڵ�·����ӵ� package.path
			/*lua_getglobal(L, "package");
			lua_getfield(L, -1, "path");
			std::string path = lua_tostring(L, -1);
			path.append(";path/to/my/module/?.lua");
			lua_pop(L, 1);
			lua_pushstring(L, path.c_str());
			lua_setfield(L, -2, "path");
			lua_pop(L, 1);*/

			// ��ע���Լ��ṩ�Ľӿں���
			registerGlobal(L);	

			LuaExtension::instance().regFunctions(L);
			return this->L;
		}

		// �ͷ���Դ
		void close()
		{
			if (L)		
				lua_close(L);
			L = nullptr;
		}

		private:
			lua_State* L = nullptr;

			// lua script informations	
			string author;
			double version;

			// setting[]
			string name;
			string desc;
			string dir;
			string input1;
			string input2;
			string input3;
			
			// functions
			PrintMsgFunc printFunc = nullptr;
			NotifyDataFunc notifyDataFunc = nullptr;
			

	};// end of plugin class

		
}// end namespace

