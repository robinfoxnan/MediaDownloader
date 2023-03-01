#include "pch.h"
#include "../include/CUrlAgent.h"
#include "StringUtils.h"
#include "GlobalData.h"

#define  SKIP_PEER_VERIFICATION    1  
#define  SKIP_HOSTNAME_VERFICATION 1  

using namespace bird2fish;

static CurlInit initGlobal;

/*
ptr��ָ��洢���ݵ�ָ�룬
size��ÿ����Ĵ�С��
nmemb��ָ�����Ŀ��
stream���û�������
���Ը���������Щ��������Ϣ����֪����ptr�е����ݵ��ܳ�����size*nmemb
*/
size_t call_wirte_func(const char *ptr, size_t size, size_t nmemb, CUrlAgent* conn)
{
	assert(conn != NULL);
	size_t len = size * nmemb;
	conn->appendBody(ptr, len);

	return len;
}
// ����http header�ص�����    
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
		/* �ͷ��ڴ� */
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
	CURL_GLOBAL_ALL             //��ʼ�����еĿ��ܵĵ��á�
	CURL_GLOBAL_SSL             //��ʼ��֧�� ��ȫ�׽��ֲ㡣
	CURL_GLOBAL_WIN32           //��ʼ��win32�׽��ֿ⡣
	CURL_GLOBAL_NOTHING         //û�ж���ĳ�ʼ����
	*/

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent.c_str());
	// �ٷ����ص�DLL����֧��GZIP��Accept-Encoding:deflate, gzip  
	curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip, deflate");
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//������Ϣ��  

	//https ����ר�ã�start  
#ifdef SKIP_PEER_VERIFICATION  
		//����������SSL��֤����ʹ��CA֤��  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	//���������SSL��֤�����ָ��һ��CA֤��Ŀ¼  
	//curl_easy_setopt(curl, CURLOPT_CAPATH, "this is ca ceat");  
#endif  

#ifdef SKIP_HOSTNAME_VERFICATION  
		//��֤�������˷��͵�֤�飬Ĭ���� 2(��)��1���У���0�����ã�  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif  
	//https ����ר�ã�end  


	//����cookieֵ��������  
	//curl_easy_setopt(curl, CURLOPT_COOKIE, "name1=var1; name2=var2;");   
	/* �������ͨ�Ž���cookie��Ĭ�����ڴ��У������ǲ����ڴ����е��ļ������� */
	//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./cookie.txt");
	/* ����CURL�����������cookie�������ͷ��ڴ��д������ļ� */
	//curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./cookie.txt");

	/* POST ���� */
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");  
	//�����ض����������  
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
	//����301��302��ת����location  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 5);
	//ץȡ���ݺ󣬻ص�����  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, call_wirte_func);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	//ץȡͷ��Ϣ���ص�����  
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

	// ֪ͨ
	GlobalData::progressInfo(this->url.c_str(), allBytes, allBytes);

	return code;
}

int CUrlAgent::postTest(const string &url, const string & data, std::map<string, string>& headers)
{
	if (curl == nullptr)
		Init();

	/* POST ���� */
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");  
	//addHeader("Content-Type", "text/plain");
	//addHeader("Content-Type", "application/json");
	int timeout = 3000;
	status = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	// ��������ʱʱ��
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);
	// ����������
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);//��ӡ������Ϣ

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