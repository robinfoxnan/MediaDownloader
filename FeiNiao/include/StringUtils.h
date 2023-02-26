#pragma once
#include <stdio.h>
#include <string>
#include <cctype>
#include <algorithm>

#if defined(WIN32) || defined(_WIN64)
#include <windows.h>
#else   // linux
#include <unistd.h>
#endif

#include "PathUtil.h"

using namespace std;

namespace bird2fish
{
	class StringUtils
	{
	public:

		// trim from start  c++14
		//static inline std::string &ltrim(std::string &s)
		//{
		//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		//	return s;
		//}

		//// trim from end using iterator
		//static inline std::string &rtrim(std::string &s)
		//{
		//	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		//	return s;
		//}

		//static inline std::string &trim(std::string &s)
		//{
		//	return ltrim(rtrim(s));
		//}

		// 在message中添加转义字符
		inline string sealMessage(string & message)
		{
			string old = "\"";
			string chars = "\\\"";
			return replace_str(message, old, chars);
		}

		string& replace_str(string& str, const string& to_replaced, const string& newchars)
		{
			for (string::size_type pos(0); pos != string::npos; pos += newchars.length())
			{
				pos = str.find(to_replaced, pos);
				if (pos != string::npos)
					str.replace(pos, to_replaced.length(), newchars);
				else
					break;
			}
			return   str;
		}

		static inline void toLower(std::string & s)
		{
			transform(s.begin(), s.end(), s.begin(), ::tolower);
		}

		static inline void toUpper(std::string & s)
		{
			transform(s.begin(), s.end(), s.begin(), ::toupper);
		}

		static inline bool string2Pair(const string str, char c, std::string& left, std::string& right)
		{
			auto pos = str.find(c, 0);
			if (pos == string::npos) {
				return false;
			}
			left = str.substr(0, pos);
			right = str.substr(pos + 1);
			//right = str.substr(pos + 1, str.length() - left.length() - 3);
			return true;
		}

	};// end of class


}

