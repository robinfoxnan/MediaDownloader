#include "pch.h"
#include "./include/ZBase64.h"
using namespace bird2fish;

string ZBase64::Encode(const char* Data, int DataByte, bool changeLine)
{
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76)
		{
			if (changeLine)
			{
				strEncode += "\r\n";
				LineLength = 0;
			}

		}
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}

string ZBase64::Decode(const char* Data, int DataByte, int& OutByte)
{
	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//返回值
	//string strDecode;
	ostringstream ss;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			//strDecode += (nValue & 0x00FF0000) >> 16;
			ss.put((nValue & 0x00FF0000) >> 16);
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				//strDecode += (nValue & 0x0000FF00) >> 8;
				ss.put((nValue & 0x0000FF00) >> 8);
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					//strDecode += nValue & 0x000000FF;
					ss.put(nValue & 0x000000FF);
					OutByte++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
	//return strDecode;
	return ss.str();
}

string ZBase64::obfuscate(const string & src, const string & key)
{
	// get memory
	char * dest = new char[src.length() + 1];
	for (int i = 0; i < src.length(); i++)
	{
		dest[i] = src[i] ^ key.at(i % key.length());
	}
	dest[src.length()] = '\0';

	string code(dest, src.length());
	delete[] dest;

	return std::move(code);
}

string ZBase64::encodeTraceInfo(const string &jsonStr, const string & key)
{
	string dest = obfuscate(jsonStr, key);
	
	return Encode(dest.c_str(), dest.length(), false);
}

string ZBase64::decodeTraceInfo(const string& base64Str, const string & key)
{
	int len;
	string json = Decode(base64Str.c_str(), base64Str.length(), len);
	return obfuscate(json, key);
}

//void test()
//{
//	string rawJson = R"delimiter(["b94aa44af8fde532",false,"cd2631a10ddcb593","http:192.168.126.1:80/PythonTier","192.168.126.1",null])delimiter";
//	string base64Code = "OlwgW3ozO0IGJzNuB1c3X1lcd1UHHy4RK354FVZ0Y2VQUmNaDgo2G1RHcUBicDICRjZvZ1gBfFtcVntIU0hsU3RqalliPyE+Dl0GAw8cd1VDT3tQYGNsThx3Z2BPAnBGBBs5FTw=";
//}


string ZBase64::readFileIntoString(const char * filename)
{
	ifstream ifile(filename);
	if (!ifile)
	{
		return "";
	}

	ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);

	ifile.close();
	ifile.clear();
	return buf.str();
}

bool ZBase64::writeToFile(const char * filename, const string & content)
{
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out);   // | std::ofstream::app
	if (!ofs)
	{
		return false;
	}
	ofs << content;

	ofs.close();

	return true;
}


