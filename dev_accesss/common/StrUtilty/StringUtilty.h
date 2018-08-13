#ifndef _STRING_UTILTY_H_
#define _STRING_UTILTY_H_

#define END_LINE "\r\n"

#include <string>
#include <vector>
#include <map>
using namespace std;

//去掉字符串左右两边的空格，
//返回true表示有修改
//返回false表示没有修改
bool StrTrim(string &strData);

//iCount从1开始
bool GetValueFromStr(const string &strData, const string &strKey, string &strValue, int iCount = 1);

bool GetValueFromStr(const string &strData, const string &strKey, int &iValue, int iCount = 1);

bool GetValueFromStr(const string &strData, const string &strKey, bool &bValue, int iCount = 1);

//查找指定分隔符strDelims前的数据
bool GetLastValueFromStr(const string &strData, const string &strDelims, int &iValue);

bool GetKeyAndValueFromStr(const string &strData, const string &strDelims, string &strKey, string &strValue);

//分解字符串
int SplitStr(const char *str,const char *spstr,std::vector<std::string> &ret_result,bool isSkipEmptyItem = false);
int SplitStr(const std::string &str,const std::string&spstr,std::vector<std::string> &ret_result,bool isSkipEmptyItem = false);

int SplitParamStr(const char* str,std::map<string,string> &out,const char *spstr = ";",const char *opt_str = "=");

#endif
