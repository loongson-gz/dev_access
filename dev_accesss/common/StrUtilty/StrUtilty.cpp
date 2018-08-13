#include "StrUtilty.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef __linux__
#	include <iconv.h>
#	include <string>
#else
#	include <Windows.h>
#endif

bool StrTrim(char *pData, int iLen)
{
	if (pData == NULL)
	{
		return false;
	}

	if (iLen == 0)
	{
		iLen = strlen(pData);
		if (iLen == 0)
		{
			return false;
		}
	}

	int iStart = 0;
	int iEnd = iLen;
	for(iStart=0; iStart<iLen; iStart++)
	{
		if (isspace(pData[iStart]) == 0)//非空格
		{
			break;
		}
	}
	
	for(; iEnd > iStart; iEnd--)
	{
		if (isspace(pData[iEnd-1]) == 0)//非空格
		{
			break;
		}
	}
	
	int iCharLen = iEnd - iStart;
	if (iCharLen > 0 && iCharLen != iLen)
	{
		memmove(pData, pData+iStart, iCharLen);
		pData[iCharLen] = 0;
		
		return true;
	}
	
	return false;
}

bool GetValueFromStr(const char *pData, const char *pKey, char *pValue, int iMaxValueLen, int iCount)
{
	char *pStart = (char*)pData;
	while (true)
	{
		pStart = strstr(pStart, pKey);
		if (pStart == NULL)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			pStart += strlen(pKey);
		}
		else
		{
			//pStart is good
			break;
		}	
	}
	if (pStart == NULL)
	{
		return false;
	}
	
	pStart += strlen(pKey);
	char *pEnd = strstr(pStart, "\n");
	if (pEnd == NULL)
	{
		pEnd = pStart+strlen(pStart);
		//return false;
	}
	if (*(pEnd-1) == '\r')
	{
		pEnd--;
	}

	int iValueLen = pEnd - pStart;
	if (iValueLen > 0 && iValueLen < iMaxValueLen-1)
	{
		strncpy(pValue, pStart, iValueLen);
		pValue[iValueLen] = 0;
		
		return true;
	}
	
	return false;
}

bool GetValueFromStr(const char *pData, const char *pKey, int &iValue, int iCount)
{
	char *pStart = (char*)pData;
	while (true)
	{
		pStart = strstr(pStart, pKey);
		if (pStart == NULL)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			pStart += strlen(pKey);
		}
		else
		{
			//pStart is good
			break;
		}	
	}
	if (pStart == NULL)
	{
		return false;
	}
	
	pStart += strlen(pKey);
	char *pEnd = strstr(pStart, "\n");
	if (pEnd == NULL)
	{
		//return false;
		pEnd=pStart + strlen(pStart);
	}
	if (*(pEnd-1) == '\r')
	{
		pEnd--;
	}

	int iValueLen = pEnd - pStart;
	if (iValueLen > 0 )
	{
		int iStart = 0;
		for(iStart=0; iStart<iValueLen; iStart++)
		{
			if (isspace(pData[iStart]) == 0)//非空格
			{
				break;
			}
		}
		
		pStart += iStart;
		if (isdigit(pStart[0]) != 0)//数字
		{
			iValue = atoi(pStart);
			return true;
		}	
	}
	
	return false;
}

bool GetValueFromStr(const char *pData, const char *pKey, bool &bValue, int iCount)
{
	char *pStart = (char*)pData;
	while (true)
	{
		pStart = strstr(pStart, pKey);
		if (pStart == NULL)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			pStart += strlen(pKey);
		}
		else
		{
			//pStart is good
			break;
		}	
	}
	if (pStart == NULL)
	{
		return false;
	}
	
	pStart += strlen(pKey);
	char *pEnd = strstr(pStart, "\n");
	if (pEnd == NULL)
	{
		pEnd=pStart + strlen(pStart);
		//return false;
	}
	if (*(pEnd-1) == '\r')
	{
		pEnd--;
	}

	int iValueLen = pEnd - pStart;
	if (iValueLen > 0 )
	{
		int iStart = 0;
		for(iStart=0; iStart<iValueLen; iStart++)
		{
			if (isspace(pData[iStart]) == 0)//非空格
			{
				break;
			}
		}
		
		pStart += iStart;
		if (strncmp(pStart, "true", strlen("true")) == 0 
			|| strncmp(pStart, "True", strlen("True")) == 0
			|| strncmp(pStart, "TRUE", strlen("TRUE")) == 0)
		{
			bValue = true;
		}
		else
		{
			bValue = false;
		}
		return true;
	}
	
	return false;
}

//查找指定分隔符strDelims后的数据
bool GetLastValueFromStr(const char *pData, const char* pDelims, int &iValue)
{
	if (pData == NULL || pDelims == NULL)
	{
		return false;
	}

	char *pSearch = (char*)pData;
	char *pStart = pSearch;
	do
	{
		pSearch = strstr(pSearch, pDelims);
		if (pSearch == NULL)
		{
			break;
		}
		pSearch += strlen(pDelims);
		pStart = pSearch;
	}while(true);
	
	if (pStart == pData)
	{
		//没有查找到
		return false;
	}

	while (isspace(*pStart) != 0)//空格类字符
	{
		pStart++;
	}

	if (isdigit(*pStart) != 0)//数字
	{
		iValue = atoi(pStart);
		return true;		
	}
	
	return false;	
}

bool GetKeyAndValueFromStr(const char *pData, const char* pDelims, char *pKey, int iKeyLen, char *pValue, int iValueLen)
{
	if (pData == NULL || pKey == NULL || pValue == NULL)
	{
		return false;
	}

	char *pSerach = strstr((char*)pData, pDelims);
	if (pSerach == NULL)
	{
		return false;
	}

	int iKeyDataLen = pSerach - pData;	
	int iValueDataLen = strlen(pData) - iKeyDataLen - strlen(pDelims);

	if (iKeyDataLen > 0)
	{
		iKeyDataLen = iKeyLen-1>iKeyDataLen?iKeyDataLen:iKeyLen-1;
		memcpy(pKey, pData, iKeyDataLen);
		pKey[iKeyDataLen] = 0x00;
	}
	if (iValueDataLen > 0)
	{
		iValueDataLen = iValueLen-1>iValueDataLen?iValueDataLen:iValueLen-1;
		memcpy(pValue, pSerach+strlen(pDelims), iValueDataLen);
		pKey[iValueDataLen] = 0x00;
	}

	return true;

}


#ifdef __linux__
int __code_convert_(const char *from_charset,const char *to_charset,const char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	char **pin = (char**)&inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd == 0)
	{
		return -1;
	}

	std::size_t iLen = outlen;
	std::size_t iLenIn = inlen;
	memset(outbuf,0,iLen);
	if (::iconv(cd,pin,&iLenIn,pout,&iLen) == std::string::npos)
	{
		return -1;
	}
	iconv_close(cd);
	return (int)iLen;
}

int Str_GbkToUtf8(const char *inbuf,int inlen,char *outbuf,int outlen)
{
	return __code_convert_("GBK","UTF-8",inbuf,inlen,outbuf,outlen);
}
int Str_Utf8ToGbk(const char *inbuf,int inlen,char *outbuf,int outlen)
{
	return __code_convert_("UTF-8","GBK",inbuf,inlen,outbuf,outlen);
}
#else
int Str_GbkToUtf8(const char *inbuf,int inlen,char *outbuf,int outlen)
{
	if (!inbuf || !outbuf || !inlen || !outlen)
	{
		return -1;
	}
	wchar_t *str1 = NULL;
	int n=MultiByteToWideChar(CP_ACP,0,inbuf,-1,NULL,0);
	str1 = new wchar_t[n];
	MultiByteToWideChar(CP_ACP,0,inbuf,-1,str1,n);
	n = WideCharToMultiByte(CP_UTF8,0,str1,-1,NULL,0,NULL,NULL);
	if (n < outlen)
	{
		WideCharToMultiByte(CP_UTF8,0,str1,-1,outbuf,outlen,NULL,NULL);
	}else
	{
		n = -1;
	}
	delete[] str1;
	return n;
}
int Str_Utf8ToGbk(const char *inbuf,int inlen,char *outbuf,int outlen)
{
	if (!inbuf || !outbuf || !inlen || !outlen)
	{
		return -1;
	}
	int len = MultiByteToWideChar(CP_UTF8,0,inbuf,-1,NULL,0);
	wchar_t *wszGBK = new wchar_t[len+1];
	
	MultiByteToWideChar(CP_UTF8,0,inbuf,-1,wszGBK,len);

	len = WideCharToMultiByte(CP_ACP,0,wszGBK,-1,NULL,0,NULL,NULL);
	if (len < outlen)
	{
		WideCharToMultiByte(CP_ACP,0,wszGBK,-1,outbuf,outlen,NULL,NULL);
	}else
	{
		len = -1;
	}
	delete[] wszGBK;
	return len;
}
#endif

int Str_GbkToUtf8(const char *inbuf,int inlen,std::string &out)
{
	int il = inlen * 2 + 2;
	char *pBuf = new char[il];
	int ret = Str_GbkToUtf8(inbuf,inlen,pBuf,il);
	if (il>0)
	{
		out = pBuf;
	}
	delete []pBuf;
	return il;
}
std::string Str_GbkToUtf8(const char *inbuf,int inlen)
{
	std::string out;
	if (inlen == 0)
	{
		inlen = strlen(inbuf);
	}
	Str_GbkToUtf8(inbuf,inlen,out);
	return out;
}

int Str_Utf8ToGbk(const char *inbuf,int inlen,std::string &out)
{
	int il = inlen * 2 + 2;
	char *pBuf = new char[il];
	int ret = Str_Utf8ToGbk(inbuf,inlen,pBuf,il);
	if (il>0)
	{
		out = pBuf;
	}
	delete []pBuf;
	return il;
}
std::string Str_Utf8ToGbk(const char *inbuf,int inlen)
{
	std::string out;
	if (inlen == 0)
	{
		inlen = strlen(inbuf);
	}
	Str_Utf8ToGbk(inbuf,inlen,out);
	return out;
}

