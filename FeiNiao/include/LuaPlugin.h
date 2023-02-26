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
	};

	// typedef functions
	using PrintMsgFunc = std::function<void(const std::string&)>;
	using NotifyDataFunc = std::function<void(int, const std::map<string, string>&)>;

	// 下列的代码作为接口给LUA脚本使用
	void printMessage(const std::string& s);
	double getEngineVersion();
	string getEngineName();
	

	// 这是C++展出lua的一个缺点，即C++无法导出其重载函数
	// 简单实体类，用于处理各种header信息
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

	// 客户端类，用于处理GET和POST
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
		int dataType();

		const string& getBodyAsString();
		string getBodyAsAnsiString();
		json11::Json getBodyAsJson();

	private:
		string host;
		HttpHeader headers;
		string body;
	};


	// JSON处理全局函数
	Json intToJson(int i);
	Json numToJson(double i);
	Json stringToJson(const string& str);
	Json objectToJson(json11::Json::object &obj);
	Json parseJsonStr(const string& str);
	Json parseJsonFile(const string& filePath);

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


	public:
		// 初始化全局各种接口函数
		bool registerGlobal(lua_State* L);
		int dowork(const string& filePath, std::vector<string>& args);
		bool tryLoad(const string& filePath);
		void loadInfo();
		string getScriptInfo(SCRIPT_INFO index);
		void handleTable(luabridge::LuaRef myInt, luabridge::LuaRef table);

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
			luaL_openlibs(L);

			// 先注册自己提供的接口函数
			registerGlobal(L);	
			return this->L;
		}

		// 释放资源
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

