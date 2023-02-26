#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
namespace bird2fish
{


class ZBase64
{
public:

	static string Encode(const char* Data, int DataByte, bool changeLine = false);
	static string Decode(const char* Data, int DataByte, int& OutByte);

	static string obfuscate(const string & src, const string & key);

	static string encodeTraceInfo(const string & jsonStr,   const string & key);
	static string decodeTraceInfo(const string & base64Str, const string & key);


	static string readFileIntoString(const char * filename);
	static bool writeToFile(const char * filename, const string & content);
};
}
