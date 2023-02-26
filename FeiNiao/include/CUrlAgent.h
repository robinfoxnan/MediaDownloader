#pragma once

#include <curl/curl.h>  
#include <string>  
#include <iostream>  
#include <fstream>
#include <assert.h>
#include <string>
#include <sstream>
#include <deque>
#include <map>

#ifdef WIN32
#pragma comment(lib, "libcurl.lib") 
#endif

using namespace std;
/*
	CURLE_OK    任务完成一切都好
	CURLE_UNSUPPORTED_PROTOCOL  不支持的协议，由URL的头部指定
	CURLE_COULDNT_CONNECT   不能连接到remote 主机或者代理
	CURLE_REMOTE_ACCESS_DENIED  访问被拒绝
	CURLE_HTTP_RETURNED_ERROR   Http返回错误
	CURLE_READ_ERROR    读本地文件错误
	CURLE_SSL_CACERT    访问HTTPS时需要CA证书路径
*/

class CurlInit
{
public:
	CurlInit()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}
	~CurlInit()
	{
		curl_global_cleanup();
	}
};

class CUrlAgent
{
public:
	CUrlAgent();
	~CUrlAgent();
	void Init();
	int getTest(const string &url, std::map<string, string>& headers);
	int postTest(const string &url, const string & data, std::map<string, string>& headers);

	void appendBody(const char  *ptr, size_t size);
	void appendHeaderLine(const char  *ptr, size_t size);
	void addHeader(const string& key, const string& value);
	bool setDownload(const string& path) 
	{ 
		
		file.open(this->filePath, ios::out | ios::binary | ios::trunc);
	
		if (!file)
		{
			return false;
		}
		

		fileMode = 1; 
		this->filePath = path; 
		return true;

	}
	int  getMode() { return fileMode; }

	inline void reset()
	{
		status = 0;
		allBytes = 0;
		fileMode = 0;
		filePath = "";

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
		ss.clear();
		ss.str("");
		code = CURLE_OK;

		headers.clear();
		

		if (headerList) //last free the header list
		{
			curl_slist_free_all(headerList); /* free the header list */
			headerList = nullptr;
		}
	}

	inline stringstream & getStream()
	{
		return ss;

	}
	inline int64_t getLen()
	{
		//return ss.tellp();
		return allBytes;
	}

	inline deque<string>& getHeader()
	{
		return headers;
	}

	std::map<string, string> getHeaderMap();

	inline int getCode()
	{
		return status;
	}

private:
	CURL *curl = nullptr;
	struct curl_slist *headerList = nullptr;
	int fileMode = 0;
	string filePath;
	std::ofstream file;

	int code = CURLE_OK;
	long status;

	char errorBuffer[CURL_ERROR_SIZE];
	std::deque<string> headers;
	std::string url = "";
	//string body;

	int64_t firstResTime;
	int64_t resEndTime;
	int64_t allBytes;
	stringstream ss;
	
  
	std::string useragent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1";
};

