#include "pch.h"
#include "./include/LuaPlugin.h"
#include <functional>
#include "StringUtils.h"
#include "PathUtil.h"

using namespace bird2fish;


string LuaPlugin::localDir = "d:\\mp3";


void bird2fish::printMessage(const std::string& s)
{
	std::cout << s << std::endl;
}
double bird2fish::getEngineVersion()
{
	return 0.1;
}
string bird2fish::getEngineName()
{
	return "media_downloader";
}




void HttpHeader::setItem(const string& key, const string& value) {
	//std::cout << "key=" << key << ", value=" << value << endl;
	items[key] = value;
}

luabridge::LuaRef HttpHeader::getPairsTable(lua_State* luastate)
{
	if (luastate == nullptr)
		return nullptr;

	//lua_State* luastate = GlobalScriptManager.get_lua_state();//获取lua_State
	luabridge::LuaRef uniforms(luastate, luabridge::newTable(luastate));      //返回值   一个table

	std::map<std::string, std::string>::iterator iter;
	for (auto iter: items)
	{
		std::string key = iter.first;
		std::string val = iter.second;
		uniforms[key] = val;                                          //给uniforms插入值
	}

	return uniforms;
}


// 'http://www.kuwo.cn/api/www/search/searchMusicBykeyWord?key={name}&pn={p}&rn={k}
int getHostPart(string& url, string& host, string & api)
{
	string temp = url;
	StringUtils::toLower(temp);
	size_t index = 0;
	const string head = "http://";
	int startwith = temp.compare(0, head.size(), head);
	//bool endwith = str.compare(str.size() - tail.size(), tail.size(), tail);
	if (startwith == 0)
	{
		index = temp.find("/", 7);
		if (index > 7)
		{
			host = temp.substr(0, index);
			api = temp.substr(index);
			return 1;
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////
int HttpClient::getAsString(const string& url, const HttpHeader& header) 
{
	//cout << "url = " << url << endl;
	CUrlAgent agent;
	agent.Init();
	int code = agent.getTest(url, const_cast<HttpHeader &>(header).getItems());
	this->body = agent.getStream().str();
	this->headers = HttpHeader(agent.getHeaderMap());
	if (code == 0)
		code = agent.getCode();
	return code;
}

int HttpClient::getAsFile(const string& url, const HttpHeader& header, const string &filePath)
{
	CUrlAgent agent;
	agent.Init();
	bool ret = agent.setDownload(filePath);
	if (ret == false)
	{
		return -1;
	}

	int code = agent.getTest(url, const_cast<HttpHeader&>(header).getItems(), true);
	
	this->headers = HttpHeader(agent.getHeaderMap());

	if (code == 0)
	{
		errCode = 0;
		code = agent.getCode();
		fileSize = agent.getLen();
	}	
	else
	{
		errCode = code;
		code = -1;
	}
	
	return code;

}

int64_t HttpClient::getFileSize()
{
	return this->fileSize;
}
int HttpClient::getErr()
{
	return errCode;

}
int HttpClient::dataType()
{
	return 0;


}

json11::Json HttpClient::getBodyAsJson()
{
	string err;
	string temp = StringEncoding::utf8ToAnsi(body);
	json11::Json doc = json11::Json::parse(temp, err);
	if (doc.is_null())
	{
		printMessage(err);
		return nullptr;
	}
	
	return doc;
}

const string& HttpClient::getBodyAsString()
{
	return body;
}

string HttpClient::getBodyAsAnsiString()
{
	return StringEncoding::utf8ToAnsi(body);
}

HttpHeader& HttpClient::getHeader()
{
	return this->headers;
}

bool LuaPlugin::tryLoad(const string& filePath)
{
	int ret = luaL_dofile(L, filePath.c_str());
	if (ret != LUA_OK)
	{
		string msg = lua_tostring(L, ret);
		printFunc(msg);
		printFunc("can't load script.lua! ");
		return false;
	}
	loadInfo();
	return true;

}

void LuaPlugin::loadInfo()
{
	if (L == nullptr)
		return;

	try
	{
		LuaRef a = getGlobal(L, "author");
		LuaRef v = getGlobal(L, "version");
		if (!a.isString())
		{
			printFunc("err: author must be string");
			return;
		}
		if (!v.isNumber())
		{
			printFunc("err: author must be number");
			return;
		}

		this->author = a.cast<string>();
		this->version = v.cast<double>();

		LuaRef t = getGlobal(L, "setting");
		LuaRef n = t["name"];
		LuaRef d = t["dir"];
		LuaRef desc = t["desc"];
		LuaRef i1 = t["input1"];
		LuaRef i2 = t["input2"];
		LuaRef i3 = t["input3"];
		this->name = n.cast<string>();
		this->dir = d.cast<string>();
		this->desc = desc.cast<string>();
		this->input1 = i1.cast<string>();
		this->input2 = i2.cast<string>();
		this->input3 = i3.cast<string>();

		// 设置表的内容
		t["dir"].rawset<const string&>(LuaPlugin::localDir);
	}
	catch (LuaException* ex)
	{
		printFunc("err");
	}
	catch (CException* e)
	{
		printFunc("err");
	}
	

	
}

int LuaPlugin::dowork(const string& filePath, std::vector<string> &args)
{
	int ret = luaL_dofile(L, filePath.c_str());
	if (ret != LUA_OK)
	{
		printFunc(lua_tostring(L, ret));
		//std::cout << "can't load script.lua! " << endl;
		printFunc("can't load script.lua! ");
		return -1;
	}

	//lua_pcall(L, 0, 0, 0);
	loadInfo();

	
	// 调用脚本的入口函数
	LuaRef luaMain = getGlobal(L, "lua_main");
	if (luaMain.isNil())
	{
		//std::cout << "open lua_main error. " << std::endl;
		printFunc("open lua_main error. ");
		return -2;
	}
	try
	{
		//lua_pushstring(L, "str1");

		string keyWords = "程响";
		string pageSize = "30";
		string pageNum = "1";

		if (args.size() > 1) keyWords = args[0];
		if (args.size() > 2) pageNum = args[1];
		if (args.size() > 3) pageSize = args[2];
		int result = luaMain(keyWords, pageNum, pageSize);
		//std::cout << "search Result:" << result << std::endl;
		printFunc("脚本返回值：" + std::to_string(result));
	}
	catch (const luabridge::LuaException& e) {
		//std::cerr << e.what() << std::endl;
		string err = "error: ";
		err += +e.what();
		printFunc(err );
	}

	//luaL_unref(L, LUA_REGISTRYINDEX, luaMain); // 清理LuaBridge的引用
	return 0;
}

/// 
// JSON处理全局函数
Json bird2fish::intToJson(int i)
{
	return Json(i);
}

Json bird2fish::numToJson(double i)
{
	return Json(i);
}

Json bird2fish::stringToJson(const string& str)
{
	return Json(str);
}

Json bird2fish::objectToJson(json11::Json::object& obj)
{
	return Json(obj);
}

Json bird2fish::parseJsonStr(const string& str)
{
	string err;
	json11::Json doc = json11::Json::parse(str, err);
	if (doc.is_null())
	{
		printMessage(err);
	}
	return doc;
}


Json bird2fish::parseJsonFile(const string& filePath)
{
	string content = FileUtil::readFileAsString(filePath.c_str());
	string err;
	json11::Json doc = json11::Json::parse(content, err);
	if (doc.is_null())
	{
		printMessage(err);
	}
	return doc;
}

void bird2fish::renameOsFile(lua_State* L)
{
	// set nil to them
	lua_pushnil(L);
	lua_setglobal(L, "os");

	lua_pushnil(L);
	lua_setglobal(L, "file");

	lua_pushnil(L);
	lua_setglobal(L, "io");

	// rename them
	/*lua_pushcfunction(L, luaopen_os);
	lua_pushstring(L, "os_disabled");
	lua_call(L, 1, 0);

	lua_pushcfunction(L, luaopen_io);
	lua_pushstring(L, "file_disabled");
	lua_call(L, 1, 0);*/

}

bool LuaPlugin::registerGlobal(lua_State* L)
{
	renameOsFile(L);

	// 版本信息
	getGlobalNamespace(L).addFunction("engine_name",    getEngineName);
	getGlobalNamespace(L).addFunction("engine_version", getEngineVersion);

	// 注册打印信息函数
	auto handlePrintFunc = std::function<void(luabridge::LuaRef)>(
		[&](luabridge::LuaRef param) {
			this->myPrint(param);
		});

	getGlobalNamespace(L).addFunction("print", handlePrintFunc);

	if (printFunc)
		getGlobalNamespace(L).addFunction("printMessage", printFunc);

	auto handleTableFunc = std::function<void(luabridge::LuaRef, luabridge::LuaRef)>(
		[&](luabridge::LuaRef myInt, luabridge::LuaRef table) {
			this->handleTable(myInt, table);
	});

	getGlobalNamespace(L).addFunction("notifyData", handleTableFunc);


	// other bind method using bind
	/*std::function<void(luabridge::LuaRef, luabridge::LuaRef)> handleTableFunc = 
		std::bind(&LuaPlugin::handleTable, this, std::placeholders::_1, std::placeholders::_2);
	getGlobalNamespace(L).addFunction<void, luabridge::LuaRef, luabridge::LuaRef>("notifyData", handleTableFunc);*/
	
	// Json
	getGlobalNamespace(L).addFunction("intToJson", intToJson);
	getGlobalNamespace(L).addFunction("stringToJson", stringToJson);
	getGlobalNamespace(L).addFunction("numToJson", numToJson);
	getGlobalNamespace(L).addFunction("parseJsonStr", parseJsonStr);
	getGlobalNamespace(L).addFunction("parseJsonFile", parseJsonFile);

	// 文件与字符串
	getGlobalNamespace(L).addFunction("combinePath", PathUtil::combinePathWindows);
	getGlobalNamespace(L).addFunction("mkDir", LuaPlugin::createDir);
	getGlobalNamespace(L).addFunction("readFileAsString", FileUtil::readFileAsString);
	getGlobalNamespace(L).addFunction("writeToFile", FileUtil::writeFile);
	getGlobalNamespace(L).addFunction("fileExist", PathUtil::fileExist);
	
	// 结构体操作

	// 字符串处理
	getGlobalNamespace(L).addFunction<string, const string&>("ansiToUtf8", StringEncoding::ansiToUtf8);
	getGlobalNamespace(L).addFunction<string, const string&>("utf8ToAnsi", StringEncoding::utf8ToAnsi);
	getGlobalNamespace(L).addFunction<string, const string&>("urlEncodeAnsi", UrlEncoding::escapeURLAnsi);
	getGlobalNamespace(L).addFunction<string, const string&>("urlDecodeAnsi", UrlEncoding::deescapeURLAnsi);

	
	auto funcGetheaderTable = std::function <luabridge::LuaRef(HttpHeader*, lua_State*)>(
		[](HttpHeader* header, lua_State* luastate) { return header->getPairsTable(luastate); }
	);
	// 注册HttpHeader类
	luabridge::getGlobalNamespace(L)
		.beginClass<HttpHeader>("HttpHeader")
		.addConstructor<void(*)(void)>()
		.addFunction("setItem", &HttpHeader::setItem)
		.addFunction("getItem", &HttpHeader::getItem)
		.addFunction("getItems", funcGetheaderTable)
		.endClass();


	// 注册HttpClient类
	luabridge::getGlobalNamespace(L)
		.beginClass<HttpClient>("HttpClient")
		.addConstructor<void(*)(void)>()
		.addFunction<int, const string&, const HttpHeader&>("doGet", &HttpClient::getAsString)
		.addFunction("doGetToFile", &HttpClient::getAsFile)
		
		.addFunction<const string&>("getBodyAsString", &HttpClient::getBodyAsString)
		.addFunction<string>("getBodyAsAnsiString", &HttpClient::getBodyAsAnsiString)
		.addFunction<json11::Json>("getBodyAsJson", &HttpClient::getBodyAsJson)
		
		.addFunction("getHeader", &HttpClient::getHeader)
		.addFunction("getSize", &HttpClient::getFileSize)
		.addFunction("getErr", &HttpClient::getErr)
		.endClass();



	// JsonArray的At函数比较麻烦，这里映射为全局函数
	auto funcAt = std::function <Json (const std::vector<Json>*, int)>(
		[](const std::vector<Json>* vec, int i) { return (*vec)[i]; }
	);

	auto funcAppend = std::function <void(std::vector<Json>*, Json &data)>(
		[](std::vector<Json>* vec, Json& data) { return vec->push_back(data); }
	);

	auto funcAppendInt = std::function <void(std::vector<Json>*, int)>(
		[](std::vector<Json>* vec, int data) { return vec->push_back(Json(data)); }
	);

	auto funcAppendNum = std::function <void(std::vector<Json>*, double)>(
		[](std::vector<Json>* vec, double data) { return vec->push_back(Json(data)); }
	);

	auto funcAppendStr = std::function <void(std::vector<Json>*, const string& data)>(
		[](std::vector<Json>* vec, const string& data) { return vec->push_back(Json(data)); }
	);

	auto funcArrayToJson = std::function <Json(std::vector<Json>*)>(
		[](std::vector<Json>* vec) { return Json((*vec)); }
	);

	getGlobalNamespace(L).addFunction("jsonAt", funcAt);

	// 注册JsonArray类
	luabridge::getGlobalNamespace(L)
		.beginClass<json11::Json::array>("JsonArray")
		.addConstructor<void(*)(void)>()
		.addFunction<size_t>("size", &json11::Json::array::size)
		.addFunction("clear", &json11::Json::array::clear)
		.addFunction("append", funcAppend)
		.addFunction("appendInt", funcAppendInt)
		.addFunction("appendNum", funcAppendNum)
		.addFunction("appendStr", funcAppendStr)
		.addFunction("at", funcAt)
		.addFunction("toJson", funcArrayToJson)

		.endClass();


	// 注册JsonObject类
	auto funcObjAt = std::function<Json(Json::object*, const string&)>(
		[](Json::object* obj, const string& i) { return (*obj)[i]; }
	);

	auto funcObjSet = std::function<void (Json::object*, const string&, Json&)>(
		[](Json::object* obj, const string& i, Json& data) { return (*obj)[i] = data; }
	);

	auto funcObjSetInt = std::function<void(Json::object*, const string&, int)>(
		[](Json::object* obj, const string& i, int val) { return (*obj)[i] = Json(val); }
	);

	auto funcObjSetStr = std::function<void(Json::object*, const string&, const string&)>(
		[](Json::object* obj, const string& i, const string&val) { return (*obj)[i] = Json(val); }
	);

	auto funcObjSetNum = std::function<void(Json::object*, const string&, double)>(
		[](Json::object* obj, const string& i, double val) { return (*obj)[i] = Json(val); }
	);

	auto funcObjErase = std::function<void(Json::object*, const string&)>(
		[](Json::object* obj, const string& i) { return obj->erase(i); }
	);

	auto funcObjToJson = std::function<Json(Json::object*)>(
		[](Json::object* obj) { return Json(*obj); }
	);
	
	luabridge::getGlobalNamespace(L)
		.beginClass<json11::Json::object>("JsonObject")
		.addConstructor<void(*)(void)>()
		.addFunction<size_t>("size", &json11::Json::object::size)
		.addFunction("clear", &json11::Json::object::clear)
		.addFunction("at", funcObjAt)
		.addFunction("set", funcObjSet)
		.addFunction("setInt", funcObjSetInt)
		.addFunction("setStr", funcObjSetStr)
		.addFunction("setNum", funcObjSetNum)
		.addFunction("remove", funcObjErase)
		.addFunction("toJson", funcObjToJson)
		.endClass();

	// 注册JSON11类
	luabridge::getGlobalNamespace(L)
		.beginClass<json11::Json>("Json")
		.addConstructor<void(*)(const std::string&)>()
		.addFunction<bool>("isNull", &json11::Json::is_null)
		.addFunction<bool>("isNumber", &json11::Json::is_number)
		.addFunction<bool>("isBool", &json11::Json::is_bool)
		.addFunction<bool>("isString", &json11::Json::is_string)
		.addFunction<bool>("isArray", &json11::Json::is_array)
		.addFunction<bool>("isObject", &json11::Json::is_object)

		.addFunction<int>("toInt",       &json11::Json::int_value)
		.addFunction<bool>("toBool",     &json11::Json::bool_value)
		.addFunction<double>("toNumber", &json11::Json::number_value)
		.addFunction<const string&>("toString", &json11::Json::string_value)
		.addFunction<const json11::Json::array &>("toArray", &json11::Json::array_items)

		.addFunction<const Json&, const std::string&>("get", &json11::Json:: operator[])
		.addFunction("dump", &json11::Json::dumpToString)
		.endClass();

	return true;
}

string LuaPlugin::getScriptInfo(bird2fish::SCRIPT_INFO index)
{
	std::stringstream ss;
	switch (index)
	{
	case SCRIPT_DESC:
		
		
		ss << "脚本说明：" << desc << "\r\n";
		
		return ss.str();
	case SCRIPT_AUTHOR:

		ss << "名称：" << name << ", ";
		ss << "作者：" << author << "，";
		ss << "版本号：" << version ;
		
		return ss.str();


	case SCRIPT_INPUT1:
		return input1;
		
	case SCRIPT_INPUT2:
		return input2;

	case SCRIPT_INPUT3:
		return input3;

	default:
		break;
	}
	return "";
}


void LuaPlugin::myPrint(luabridge::LuaRef param)
{
	if (this->printFunc == nullptr)
		return;

	if (param.isNumber()) {
		
		int t = param.cast<double>();
		this->printFunc(std::to_string(t));
		return;
	}
	else if (param.isString())
	{
		this->printFunc(param.cast<string>());
		return;
	}
	else if (param.isBool())
	{
		bool b = param.cast<bool>();
		if (b)
			this->printFunc("True");
		else
			this->printFunc("False");
	}
	else if (param.isNil())
	{
		this->printFunc("Nil");
	}
	else if (param.isTable())
	{
		lua_State* L = param.state();
		std::vector<string> keys;
		// 确保值是一个 table
		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2) != 0)
			{
				// 如果键是字符串，则打印键名
				if (lua_type(L, -2) == LUA_TSTRING)
				{
					const char* key = lua_tostring(L, -2);
					//std::cout << "Key: " << key << std::endl;
					keys.emplace_back(key);
				}
				lua_pop(L, 1);
			}
		}
		else
		{
			throw std::runtime_error("print() can't translate this object type");
			return;
		}

		stringstream ss;
		for (auto& key : keys) {
			// 使用LuaRef的成员函数来访问table中指定键的值
			LuaRef value = param[key];
			// 输出键值对
			ss << key << " : " << value.cast<string>() << endl;
			//data[key] = value.cast<string>();
		}
		printFunc(ss.str());
	}
	else
	{
		throw std::runtime_error("print() can't translate this object type");
	}
}

void LuaPlugin::handleTable(luabridge::LuaRef myInt, luabridge::LuaRef table)
{
	if (!myInt.isNumber()) {
		throw std::runtime_error("myFunction expects an integer  and a table");
		return;
	}

	int dataType = myInt.cast<int>();

	lua_State* L = table.state();
	std::vector<string> keys;
	// 确保值是一个 table
	if (lua_istable(L, -1))
	{
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			// 如果键是字符串，则打印键名
			if (lua_type(L, -2) == LUA_TSTRING)
			{
				const char* key = lua_tostring(L, -2);
				//std::cout << "Key: " << key << std::endl;
				keys.emplace_back(key);
			}
			lua_pop(L, 1);
		}
	}
	else
	{
		throw std::runtime_error("myFunction expects an integer and a table");
		return;
	}

	std::map<string, string> data;
	for (auto& key : keys) {
		// 使用LuaRef的成员函数来访问table中指定键的值
		LuaRef value = table[key];
		// 输出键值对
		//cout << key << " : " << value.cast<string>() << endl;
		data[key] = value.cast<string>();
	}

	if (this->notifyDataFunc)
	{
		this->notifyDataFunc(dataType, data);
	}
	else
	{
		this->printFunc("recv data, but no callback to show");
	}

	return;
}

bool LuaPlugin::createDir(const string& filePath)
{
	if (!PathUtil::isPrefixWith(filePath, LuaPlugin::localDir))
	{
		return false;
		//printFunc("所创建的目录是设置的子目录");
	}

	return PathUtil::createDir(filePath);
}