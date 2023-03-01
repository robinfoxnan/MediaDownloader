#include "pch.h"
#include "../include/CUrlAgent.h"
#include "StringUtils.h"
#include "GlobalData.h"

#define  SKIP_PEER_VERIFICATION    1  
#define  SKIP_HOSTNAME_VERFICATION 1  

using namespace bird2fish;

static CurlInit initGlobal;

/*
ptr是指向存储数据的指针，
size是每个块的大小，
nmemb是指块的数目，
stream是用户参数。
所以根据以上这些参数的信息可以知道，ptr中的数据的总长度是size*nmemb
*/
size_t call_wirte_func(const char *ptr, size_t size, size_t nmemb, CUrlAgent* conn)
{
	assert(conn != NULL);
	size_t len = size * nmemb;
	conn->appendBody(ptr, len);

	return len;
}
// 返回http header回调函数    
size_t header_callback(const char  *ptr, size_t size, size_t nmemb, CUrlAgent* conn)
{
	assert(conn != NULL);

	size_t len = size * nmemb;
	conn->appendHeaderLine(ptr, len);

	return len;
}
////////////////////////////////////////////////////////////////
CUrlAgent::CUrlAgent()
{
	curl = nullptr;
	filePath = "";
}

CUrlAgent::~CUrlAgent()
{
	if (curl != nullptr)
	{
		/* 释放内存 */
		curl_easy_cleanup(curl);
		curl = nullptr;
	}

	if (headerList) //last free the header list
		curl_slist_free_all(headerList); /* free the header list */

}

void CUrlAgent::appendBody(const char  *ptr, size_t size)
{
	//string str(ptr, size);
	//ss << str;

	allBytes += size;
	if (fileMode == 0)
	{
		ss.write(ptr, size);
	}
	else
	{
		file.write(ptr, size);
		GlobalData::progressInfo(this->url.c_str(), allBytes, 0);
	}
	
	
	//body.append(str);

}
void CUrlAgent::appendHeaderLine(const char  *ptr, size_t size)
{
	if (status == 0)
	{
		CURLcode ret = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
	}
	if (headers.size() == 0)
	{

	}
	
	string str(ptr, size);
	headers.emplace_back(str);
	//cout << str << endl;
}
void CUrlAgent::addHeader(const string& key, const string& value)
{

	//headers = curl_slist_append(headers, "Content-Type: text/xml");
	//headers = curl_slist_append(headers, "Accept: text/html, */*;q=0.01");
	//...
	string str = key + ":" + value;
	headerList = curl_slist_append(headerList, str.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);


}

void CUrlAgent::Init()
{
	/*
	CURL_GLOBAL_ALL             //初始化所有的可能的调用。
	CURL_GLOBAL_SSL             //初始化支持 安全套接字层。
	CURL_GLOBAL_WIN32           //初始化win32套接字库。
	CURL_GLOBAL_NOTHING         //没有额外的初始化。
	*/

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent.c_str());
	// 官方下载的DLL并不支持GZIP，Accept-Encoding:deflate, gzip  
	curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip, deflate");
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//调试信息打开  

	//https 访问专用：start  
#ifdef SKIP_PEER_VERIFICATION  
		//跳过服务器SSL验证，不使用CA证书  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	//如果不跳过SSL验证，则可指定一个CA证书目录  
	//curl_easy_setopt(curl, CURLOPT_CAPATH, "this is ca ceat");  
#endif  

#ifdef SKIP_HOSTNAME_VERFICATION  
		//验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif  
	//https 访问专用：end  


	//发送cookie值给服务器  
	//curl_easy_setopt(curl, CURLOPT_COOKIE, "name1=var1; name2=var2;");   
	/* 与服务器通信交互cookie，默认在内存中，可以是不存在磁盘中的文件或留空 */
	//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./cookie.txt");
	/* 与多个CURL或浏览器交互cookie，会在释放内存后写入磁盘文件 */
	//curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./cookie.txt");

	/* POST 数据 */
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");  
	//设置重定向的最大次数  
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
	//设置301、302跳转跟随location  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 5);
	//抓取内容后，回调函数  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, call_wirte_func);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	//抓取头信息，回调函数  
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

	/* enable TCP keep-alive for this transfer */
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

	/* keep-alive idle time to 120 seconds */
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);

	/* interval time between keep-alive probes: 6 seconds */
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 6L);

}
std::map<string, string> CUrlAgent::getHeaderMap()
{
	std::map<string, string> temp;
	std::string key = "";
	std::string val = "";
	for (auto str : headers) {
		bool ret = StringUtils::string2Pair(str, ':', key, val);
		if (ret)
		{
			temp[key] = val.substr(0, val.length()-2);
		}
	}
	return temp;
}

int CUrlAgent::getTest(const string &url, std::map<string, string>& headers, bool bSaveFile)
{
	if (curl == nullptr)
		Init();

	if (curl == nullptr)
		return false;

	reset();
	if (bSaveFile)
		fileMode = 1;

	this->url = url;
	status = 0;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());


	for (auto i : headers)
	{
		this->addHeader(i.first, i.second);
	}

	code = curl_easy_perform(curl);
	if (CURLE_OK == code)
	{
		if (status == 0)
		{
			CURLcode ret = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
			
			//status = ret;
		}
	}
	if (file)
		file.close();

	// 通知
	GlobalData::progressInfo(this->url.c_str(), allBytes, allBytes);

	return code;
}

int CUrlAgent::postTest(const string &url, const string & data, std::map<string, string>& headers)
{
	if (curl == nullptr)
		Init();

	/* POST 数据 */
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");  
	//addHeader("Content-Type", "text/plain");
	//addHeader("Content-Type", "application/json");
	int timeout = 3000;
	status = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	// 设置请求超时时间
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);
	// 设置请求体
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);//打印调试信息

	code = curl_easy_perform(curl);
	if (CURLE_OK == code)
	{
		if (status == 0)
		{
			CURLcode ret = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
		}
	}

	if (file)
		file.close();
	return -code;
}