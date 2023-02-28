#include "pch.h"

#include "./include/IniParser.h"
#include <fstream>
#include <iostream>


using namespace bird2fish;

std::string & IniParser::trimString(std::string &str)
{
	std::string::size_type pos = 0;

	pos = 0;
	if (str.npos != (pos = str.find("#")))
	{
		str = str.replace(pos, str.length() - 1, "");
	}

	while (str.npos != (pos = str.find(" ")) )
	{
		str = str.replace(pos, pos + 1, "");
	}
	pos =0;
	while (str.npos != (pos = str.find("\r")) )
	{
		str = str.replace(pos, pos + 1, "");
	}
	
	return str;
}

IniParser::IniParser() : errMsg(""), fieldsMap({})
{

}

IniParser::~IniParser()
{
	fieldsMap.clear();
}

std::string IniParser::getValue(std::string root, std::string key)
{
	string item = root + "#" + key;
	std::map<string, string>::iterator it;
	it = fieldsMap.find(item);
	if (it != fieldsMap.end())
		return it->second;

	errMsg = "not found";
	return "";
}

bool IniParser::load(std::string & path)
{
	printf("loading cpp_agent configfile = %s \n", path.c_str());
	return load(path.c_str());
}

bool IniParser::load(const char * path)
{
	std::ifstream in_conf_file(path);

	if (!in_conf_file)
	{
		errMsg = "open config file error, check ini exist or not!";
		return false;
	}

	std::string str_line = "";
	std::string str_root = "";

	while (getline(in_conf_file, str_line))
	{
		std::string::size_type left_pos = 0;
		std::string::size_type right_pos = 0;
		std::string::size_type equal_div_pos = 0;
		std::string str_key = "";
		std::string str_value = "";

		if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
		{
			str_root = str_line.substr(left_pos + 1, right_pos - 1);
		}
		if (str_line.npos != (equal_div_pos = str_line.find("=")))
		{
			str_key = str_line.substr(0, equal_div_pos);
			str_value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
			str_key = trimString(str_key);
			str_value = trimString(str_value);
		}
		if ((!str_root.empty()) && (!str_key.empty()) && (!str_value.empty()))
		{
			std::string tmp_key = str_root + "#" + str_key;

			fieldsMap[str_root + "#" + str_key] = str_value;
		}
	}
	in_conf_file.close();
	in_conf_file.clear();
	return true;
}

size_t IniParser::getSize() const
{
	return fieldsMap.size();
}






