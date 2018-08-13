#ifndef _STR_UTILTY_H_
#define _STR_UTILTY_H_

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include <string>

#define END_LINE "\r\n"

//去掉字符串左右两边的空格，
//返回true表示有修改
//返回false表示没有修改
bool StrTrim(char *pData, int iLen = 0);

//iCount从1开始
bool GetValueFromStr(const char *pData, const char *pKey, char *pValue, int iMaxValueLen, int iCount = 1);

bool GetValueFromStr(const char *pData, const char *pKey, int &iValue, int iCount = 1);

bool GetValueFromStr(const char *pData, const char *pKey, bool &bValue, int iCount = 1);

//查找指定分隔符strDelims前的数据
bool GetLastValueFromStr(const char *pData, const char* pDelims, int &iValue);

bool GetKeyAndValueFromStr(const char *pData, const char* pDelims, char *pKey, int iKeyLen, char *pValue, int iValueLen);

//utf8 <--> gbk 互转
int Str_Utf8ToGbk(const char *inbuf,int inlen,char *outbuf,int outlen);
int Str_GbkToUtf8(const char *inbuf,int inlen,char *outbuf,int outlen);

//utf8 <--> gbk 互转
std::string Str_Utf8ToGbk(const char *inbuf,int inlen = 0);
std::string Str_GbkToUtf8(const char *inbuf,int inlen = 0);

int Str_Utf8ToGbk(const char *inbuf,int inlen,std::string &out);
int Str_GbkToUtf8(const char *inbuf,int inlen,std::string &out);

#endif
