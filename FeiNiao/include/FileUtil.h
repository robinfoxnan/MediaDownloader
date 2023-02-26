#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "CUrlAgent.h"
#include "UrlEncoding.h"

using namespace std;
namespace bird2fish
{
#define LINE_LENGTH 4096

	class FileUtil {
	public:
		static void writeFile(const stringstream& body, const char* filePath)
		{
			//string outFilePath = "temp.html";
			std::ofstream fs(filePath, std::ios::out | std::ios::binary);
			/*std::ofstream outFile;
			outFile.open(outFilePath);*/
			fs << body.str();
		}

		static void writeFile(const string& str, const char* filePath) 
		{
			std::ofstream fs(filePath, std::ios::out | std::ios::binary);
			fs << str;
		}

		static string readFileAsString(const char * filePath)
		{
			std::ifstream file(filePath, ios::in | ios::binary);
			if (!file) {
				return "";
			}

			//指针定位到文件末尾
			file.seekg(0, std::ios::end);
			int fileLength = file.tellg();

			//指定定位到文件开始
			file.seekg(0, std::ios::beg);

			//cout << "fileLength:" << fileLength << endl;

			char* buffer = nullptr;
			buffer = new char[fileLength + (int)1];

			file.read(buffer, fileLength);
			buffer[fileLength] = '\0';
			string contents = buffer;
			cout << "contents:" << contents << endl;

			if (buffer) {
				delete[] buffer;
			}
			file.close();

			return std::move(contents);
		}
	};

}