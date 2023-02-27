#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <deque>

#include <string.h>

using namespace std;

namespace bird2fish
{


class PathUtil
{
public:
	// 字符串范围内，逆向查找
	//static const char *  strrfind(const char *begin, const char *end, const char c)
	//{
	//	const char * buf = end;
	//	while (buf >= begin)
	//	{
	//		if (*buf == c)
	//			return buf;
	//		buf--;
	//	}
	//	return nullptr;
	//}
	//static size_t  combinePathC(char ** buffer, const char * dir, const char *name)
	//{
	//	int n1 = strlen(dir);
	//	int n2 = strlen(name);
	//	size_t len = n1 + n2 + 2;
	//	char *buf = new char[len];
	//	*buffer = buf;

	//	memcpy(buf, dir, n1);
	//	size_t len1 = n1;

	//	// 末尾保证有一个"\"
	//	if (buf[n1 - 1] == '\\')
	//	{
	//		len1 = n1;
	//	}
	//	else
	//	{
	//		buf[n1] = '\\';
	//		len1 = n1 + 1;

	//	}
	//	buf[len1] = '\0';

	//	// len1++示当前拼接的长度
	//	size_t index = 0;
	//	char * rPart = (char *)name;
	//	size_t len2 = n2;
	//	while (index < n2)   // 使用index向后滑动，直到末尾
	//	{
	//		// 滑动后当前位置
	//		rPart = (char *)name + index;
	//		char * tmp = (char *)strchr(rPart, '\\');
	//		if (tmp == nullptr)   // end here
	//		{
	//			// 拼接剩下的
	//			len2 = n2 - index;
	//			memcpy(buf + len1, rPart, len2);
	//			len1 += len2;
	//			buf[len1] = '\0';
	//			len1++;

	//			break;
	//		}

	//		// 当前找到的长度
	//		len2 = tmp - rPart;
	//		if (len2 == 0)     // 遇到 "\config.ini"，去掉1个字符
	//		{
	//			index += 1;
	//		}
	//		else if (len2 == 1 && rPart[0] == '.')   // 遇到 ".\config.ini"，去掉2个字符
	//		{
	//			index += 2;
	//		}
	//		else if (len2 >= 2 && rPart[0] == '.')  // 遇到 "..\config.ini", "..x\config.ini"去掉3个字符，末尾去掉一个目录
	//		{
	//			index += len2 + 1;
	//			const char * back = strrfind(buf, buf + len1 - 2, '\\');   // 从末尾的"\"前一个字符开始找
	//			if (back == nullptr)
	//			{
	//				// dir 当前是这样的情况，"d:\”
	//			}
	//			else if ((back - dir) == 2)  // dir 当前是这样的情况，"d:\temp\”
	//			{
	//				len1 = 3;
	//				buf[3] = '\0';
	//			}
	//			else                        // dir 当前是这样的情况，"d:\temp\test1\”
	//			{
	//				len1 = back - buf + 1;
	//				buf[len1] = '\0';
	//			}
	//		}
	//		else    //  遇到首字符不为点 "x\config.ini",  
	//		{
	//			index += len2 + 1;
	//			memcpy(buf + len1, name + index, len2 + 1);
	//			len1 += len2 + 1;
	//		}
	//	}
	//	return len1;
	//}

	/*void test2()
	{
		char * buf = nullptr;
		size_t len;*/


		/*len = combinePathC(&buf, "d:\\temp\\robin\\", "config.ini");

		cout << buf << endl;

		len = combinePathC(&buf, "d:\\temp\\robin", "config.ini");

		cout << buf << endl;*/

		//len = combinePathC(&buf, "d:\\temp\\robin", ".\\config.ini");
		//cout << buf << endl;

		/*len = combinePathC(&buf, "d:\\temp\\robin", "..\\config.ini");
		cout << buf << endl;

		len = combinePathC(&buf, "d:\\temp\\robin", "...\\config.ini");
		cout << buf << endl;*/



		/*if (buf)
			delete buf;
	}*/
	
static string getExt(const string& filepath)
{
	auto pos = filepath.rfind('.');
	if (pos == filepath.npos)
	{
		return "";
	}
	return filepath.substr(pos);
}

		/**
		* NAME: getAbsopath
		* DESC: 获取程序的绝对路径
		* REMK: Absopath - Absolute path
		*/
#if defined(WIN32) || defined(_WIN64)
		static size_t getAbsopath(char* buf, size_t size)
		{
			size_t ret = GetModuleFileName(NULL, buf, (DWORD)size);	//得到程序模块.exe全路径
			//*(strrchr(buf, '\\') + 1) = '\0';		//去掉程序文件名
			return ret;
		}
#else   // linux
		static size_t getAbsopath(char* buf, size_t size)
		{
			int i;
			int len = readlink("/proc/self/exe", buf, size - 1);

			if (len < 0 || (len >= size - 1))
			{
				return 0;
			}

			buf[len] = '\0';
			/*for (i = len; i >= 0; i--)
			{
				if (buf[i] == '/')
				{
					buf[i + 1] = '\0';
					break;
				}
			}*/
			return len;
		}

#endif
		static std::string getAppName()
		{
			char buffer[1024];
			buffer[0] = '\0';
			size_t sz = sizeof(buffer);
			getAbsopath(buffer, sz);
			string path = buffer;
#if defined(WIN32) || defined(_WIN64)
			size_t off = path.rfind('\\');

#else
			int off = path.rfind('/');
	
#endif // WIN32
			if (off != string::npos)
			{
				string name = path.substr(off + 1);
				return name;
			}
			else
			{
				return "unknow";
			}
			

		}

		static std::string getAppPath()
		{
			char buffer[1024];
			buffer[0] = '\0';
			size_t sz = sizeof(buffer);
			getAbsopath(buffer, sz);
			string path = buffer;
			string confPath;

#if defined(WIN32) || defined(_WIN64)
			size_t off = path.rfind('\\');
			confPath = path.substr(0, off);
			confPath += "\\";

#else
			int off = path.rfind('/');
			confPath = path.substr(0, off);
			confPath += "/";
#endif // WIN32

			return confPath;
		}

		static std::string getConfigPath(const string fileName)
		{
			char buffer[1024];
			buffer[0] = '\0';
			size_t sz = sizeof(buffer);
			getAbsopath(buffer, sz);
			string path = buffer;
			string confPath;

#if defined(WIN32) || defined(_WIN64)
			size_t off = path.rfind('\\');
			confPath = path.substr(0, off);
			confPath += "\\";

#else
			int off = path.rfind('/');
			confPath = path.substr(0, off);
			confPath += "/";
#endif // WIN32

			//confPath += fileName;

			string path1 = PathUtil::combinePath(confPath.c_str(), fileName.c_str());
			return path1;
		}

		
	

	static void  testInWindows()
	{
		cout << combinePath("d:\\temp\\robin\\", "..\\demo\\config.ini") << endl;
		cout << endl;

		cout << combinePath("d:", "..\\demo\\config.ini") << endl;
		cout << endl;

		cout << combinePath("d:\\temp\\robin", "../demo/config.ini") << endl;
		cout << endl;

		cout << combinePath("d:\\temp\\robin", "./config.ini") << endl;
		cout << endl;

		cout << combinePath("d:\\temp\\robin", "config.ini") << endl;
		cout << endl;

		cout << combinePath("d:\\temp\\robin\\", "\\config.ini") << endl;
		cout << endl;
	}

	static void  testInLinux()
	{
		cout << combinePathLinux("/usr/local/soft/", "../demo/config.ini") << endl;
		cout << endl;

		cout << combinePathLinux("/usr/local", "./demo/config.ini") << endl;
		cout << endl;

		cout << combinePathLinux("/usr/local/", ".\\demo\\config.ini") << endl;
		cout << endl;

		cout << combinePathLinux("usr/local/", "/config.ini") << endl;
		cout << endl;

		cout << combinePathLinux("usr/local", "config.ini") << endl;
		cout << endl;

		cout << combinePathLinux("usr/local", "\\config.ini") << endl;
		cout << endl;
	}




	static string& replace_str(string& str, const string& to_replaced, const string& newchars)
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

	static std::string combinePath(const char *dir, const char* name)
	{
#if defined (_WIN32) || (_WIN64)
		return combinePathWindows(dir, name);
#else
		return combinePathLinux(dir, name);
#endif

	}

	// windows
	static std::string combinePathWindows(const char *dir, const char* name)
	{

		//printf("%s  +  %s\n", dir, name);
		if (dir == nullptr || name == nullptr)
			return "";
		string temp = dir;
		replace_str(temp, "/", "\\");

		

		std::stringstream oss(temp);
		std::deque<std::string> vecDir;
		std::deque<std::string> vecName;

		string part;
		while (std::getline(oss, part, '\\'))
		{
			if (part.length() == 0)
				continue;
			vecDir.emplace_back(part);
		}


		temp = name;
		replace_str(temp, "/", "\\");

		// 2022-04-13 如果是绝对路径d:\\之类的
		if (temp.at(1) == ':')
			return temp;

		oss.clear();
		oss.str(temp);
		while (std::getline(oss, part, '\\'))
		{
			if (part.length() == 0)
				continue;
			vecName.emplace_back(part);
		}

		int index = 0;
		while (index < vecName.size())
		{
			if (vecName[0] == ".")
			{
				vecName.pop_front();
			}
			//else if (vecName[0].find("..") != vecName[0].npos)
			else if (vecName[0] == "..")
			{
				vecName.pop_front();
				if (vecDir.size() > 1)
					vecDir.pop_back();
			}
			else
			{
				vecDir.emplace_back(vecName[0]);
				vecName.pop_front();
			}
		}

		oss.clear();
		oss.str("");
		for (int i = 0; i < vecDir.size(); i++)
		{
			oss << vecDir[i];
			if (vecDir.size() == 1 || i < (vecDir.size() - 1))
			{
				oss << "\\";
			}
		}

		return oss.str();
	}

	// 2022-04-13 robin 更改，增加绝对路径不拼接
	static std::string combinePathLinux(const char *dir, const char* name)
	{

		//printf("%s  +  %s\n", dir, name);
		if (dir == nullptr || name == nullptr)
			return "";
		string temp = dir;
		replace_str(temp, "\\", "/");

		

		std::stringstream oss(temp);
		std::deque<std::string> vecDir;
		std::deque<std::string> vecName;

		string part;
		while (std::getline(oss, part, '/'))
		{
			if (part.length() == 0)
				continue;
			vecDir.emplace_back(part);
		}

		// 替换名字部分
		temp = name;
		replace_str(temp, "\\", "/");

		// 如果是绝对路径，则不拼接
		if (temp.at(0) == '/')
			return temp;

		oss.clear();
		oss.str(temp);
		while (std::getline(oss, part, '/'))
		{
			if (part.length() == 0)
				continue;
			vecName.emplace_back(part);
		}

		int index = 0;
		while (index < vecName.size())
		{
			if (vecName[0] == ".")
			{
				vecName.pop_front();
			}
			//else if (vecName[0].find("..") != vecName[0].npos)
			else if (vecName[0] == "..")
			{
				vecName.pop_front();
				if (vecDir.size() > 0)
					vecDir.pop_back();
			}
			else
			{
				vecDir.emplace_back(vecName[0]);
				vecName.pop_front();
			}
		}

		oss.clear();
		oss.str("");
		for (int i = 0; i < vecDir.size(); i++)
		{
			oss << "/";
			oss << vecDir[i];
			
		}

		return oss.str();
	}

};

}


