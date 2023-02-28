#ifndef _IniParser_H
#define _IniParser_H

#include <string>
#include <map>
using namespace std;

namespace bird2fish
{
	class IniParser
	{
	public:
		IniParser();
		~IniParser();

		bool load(const char * path);
		bool load(std::string & path);
		inline std::string & getErrorMsg() 
		{
			return errMsg;
		}

		size_t getSize()  const;

		inline void clear()
		{
			fieldsMap.clear();
		};

		std::string getValue(std::string root, std::string key);
	private:
		std::string &trimString(std::string &str);

		IniParser(const IniParser&) = delete;
		IniParser(const IniParser&&) = delete;
		IniParser& operator=(const IniParser&) = delete;
		IniParser& operator=(const IniParser&&) = delete;

	private:
		std::map<std::string, std::string> fieldsMap;
		std::string errMsg;
	};

/*
void testIni()
{
	string path = getConfigPath("config1.ini");
	IniParser parser;
	bool ret = parser.load(path);
	string & error = parser.getErrorMsg();

	string str = parser.getValue("global", "log_file");
	if (str == "")
	{
		error = parser.getErrorMsg();
	}
	std::cout << str << endl;
}
*/

} // namespace oneapm


#endif