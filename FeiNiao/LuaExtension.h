#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "./include/PathUtil.h"
#include "./include/FileUtil.h"
#include "./include/Json11.h"

namespace bird2fish
{
	extern "C" {
		typedef int (*luaopen_func)(lua_State* L);
	}

	class DllItem
	{
	public:
		DllItem()
		{
			m_hExtension = nullptr;
		}
		DllItem(DllItem & other)
		{
			m_hExtension = other.m_hExtension;
			other.m_hExtension = nullptr;
			name = other.name;
			filePath = other.filePath;
		}
		DllItem(DllItem&& other)
		{
			m_hExtension = other.m_hExtension;
			other.m_hExtension = nullptr;
			name = other.name;
			filePath = other.filePath;
		}
		~DllItem()
		{
			if (m_hExtension)
				FreeLibrary(m_hExtension);
		}


		bool tryLoad(const string& filePath, const string&name)
		{
			m_hExtension = LoadLibrary(filePath.c_str());
			if (m_hExtension == nullptr)
			{
				AfxMessageBox(_T("can't load dll"));
				return FALSE;
			}
			this->filePath = filePath;
			this->name = name;

			//  luaopen_libxml2pp
			string funcName = "luaopen_" + name;
			this->lpOpen = (luaopen_func)GetProcAddress(m_hExtension, "luaopen_libxml2pp");
			if (lpOpen)
			{
				
			}
			else
			{
				AfxMessageBox(_T("can't load dll because entry function can't be found"));
			}
			return true;
		}

		int regFunctions(lua_State* L)
		{
			if (L && lpOpen)
				return lpOpen(L);

			return -1;
		}

	private:
		HINSTANCE m_hExtension;
		luaopen_func lpOpen;
		string name;
		string filePath;
	};

	using DllItemPtr = std::shared_ptr<DllItem>;

	class LuaExtension
	{
	public:
		LuaExtension() {};
		~LuaExtension() {};

		static LuaExtension& instance()
		{
			static LuaExtension extension;
			return extension;
		}

		int regFunctions(lua_State* L)
		{
			int count = 0;
			for (auto& it : vec)
			{
				int ret = it->regFunctions(L);
				if (ret >= 0)
				{
					count++;
				}
			}
			return count;
		}

		int load()
		{
			vec.clear();
			string dir = PathUtil::getAppPath();
			string filePath = PathUtil::combinePath(dir.c_str(), "extension.json");

			string content = FileUtil::readFileAsString(filePath.c_str());
			string err;
			json11::Json doc = json11::Json::parse(content, err);
			if (doc.is_null())
			{
				return 0;
			}
			auto dllList = doc["dll_extension"];
			if (! dllList.is_array())
				return 0;

			int count = 0;
			for (auto& it : dllList.array_items())
			{
				DllItemPtr dllItem = std::make_shared<DllItem>();
				string name = it["function"].string_value();
				string dllPath = it["file"].string_value();
				bool ret = dllItem->tryLoad(dllPath, name);
				if (ret)
				{
					count++;
					vec.emplace_back(dllItem);
				}
			}


			return count;
		}
	private:
		std::vector<DllItemPtr> vec;

	};

}

