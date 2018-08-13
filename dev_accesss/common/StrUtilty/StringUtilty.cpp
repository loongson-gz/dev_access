#include "StringUtilty.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool StrTrim(string &strData)
{
	int iStart = 0;
	int iEnd = strData.length();
	for(iStart=0; iStart<iEnd; iStart++)
	{
		if (isspace(strData.at(iStart)) == 0)//非空格
		{
			break;
		}
	}
	
	for(; iEnd > iStart; iEnd--)
	{
		if (isspace(strData.at(iEnd-1)) == 0)//非空格
		{
			break;
		}
	}
	
	if (iStart != 0 || iEnd != strData.length())
	{
		strData.substr(iStart, iEnd - iStart);
		return true;
	}
	
	return false;
}

bool GetValueFromStr(const string &strData, const string &strKey, string &strValue, int iCount)
{
	int iStart = 0;
	while (true)
	{
		iStart = strData.find(strKey, iStart);
		if (iStart == string::npos)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			iStart += strKey.length();
		}
		else
		{
			//finish
			break;
		}	
	}
	if (iStart == string::npos)
	{
		return false;
	}
	
	iStart += strKey.length();
	int iEnd = strData.find(END_LINE, iStart);
	if (iEnd == string::npos)
	{
		return false;
	}
	
	int iValueLen = iEnd - iStart;
	if (iValueLen > 0)
	{
		strValue = strData.substr(iStart, iValueLen);
		return true;
	}
	
	return false;
}

bool GetValueFromStr(const string &strData, const string &strKey, int &iValue, int iCount)
{
	int iStart = 0;
	while (true)
	{
		iStart = strData.find(strKey, iStart);
		if (iStart == string::npos)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			iStart += strKey.length();
		}
		else
		{
			//finish
			break;
		}	
	}
	if (iStart == string::npos)
	{
		return false;
	}
	
	iStart += strKey.length();
	int iEnd = strData.find(END_LINE, iStart);
	if (iEnd == string::npos)
	{
		return false;
	}
	
	int iValueLen = iEnd - iStart;
	if (iValueLen > 0)
	{
		string strValue = strData.substr(iStart, iValueLen);
		if (StrTrim(strValue))
		{
			if (isdigit(strValue.at(0)) != 0)//数字
			{
				iValue = atoi(strValue.c_str());
				return true;
			}
		}
	}
	
	return false;
}

bool GetValueFromStr(const string &strData, const string &strKey, bool &bValue, int iCount)
{
	int iStart = 0;
	while (true)
	{
		iStart = strData.find(strKey, iStart);
		if (iStart == string::npos)
		{
			break;
		}
		iCount--;
		if (iCount > 0)
		{
			iStart += strKey.length();
		}
		else
		{
			//finish
			break;
		}	
	}
	if (iStart == string::npos)
	{
		return false;
	}
	
	iStart += strKey.length();
	int iEnd = strData.find(END_LINE, iStart);
	if (iEnd == string::npos)
	{
		return false;
	}
	
	int iValueLen = iEnd - iStart;
	if (iValueLen > 0)
	{
		string strValue = strData.substr(iStart, iValueLen);
		if (StrTrim(strValue))
		{
			if (strValue.compare(0, strlen("true"), "true") == 0
				|| strValue.compare(0, strlen("True"), "True") == 0
				|| strValue.compare(0, strlen("TRUE"), "TRUE") == 0)
			{
				bValue = true;
			}
			else
			{
				bValue = false;
			}
			return true;
		}
	}
	
	return false;
}

//查找指定分隔符strDelims前的数据
bool GetLastValueFromStr(const string &strData, const string &strDelims, int &iValue)
{
	int iStart = strData.find_last_of(strDelims);
	if (iStart == string::npos)
	{
		return false;
	}
	iStart += strDelims.length();
	string strValue = strData.substr(iStart);
	if (StrTrim(strValue))
	{
		if (isdigit(strValue.at(0)) != 0)//数字
		{
			iValue = atoi(strValue.c_str());
			return true;
		}
	}
	
	return false;
}

bool GetKeyAndValueFromStr(const string &strData, const string &strDelims, string &strKey, string &strValue)
{
	int iPos = strData.find(strDelims);
	if (iPos == string::npos)
	{
		return false;
	}

	int iKeyDataLen = iPos;	
	int iValueDataLen = strData.length() - iKeyDataLen - strDelims.length();

	if (iKeyDataLen > 0)
	{
		strKey = strData.substr(0, iKeyDataLen);
	}
	if (iValueDataLen > 0)
	{
		strValue = strData.substr(iPos+strDelims.length(), iValueDataLen);
	}

	return true;

}

int SplitStr(const char *pData,const char *sz_split,vector<string> &out_array,bool isSkipEmptyItem )
{
	if (!pData || !sz_split)
	{
		return 0;
	}
	string s1 = pData;
	string s2 = sz_split;
	return SplitStr(s1,s2,out_array,isSkipEmptyItem);
}
int SplitStr(const string& sStr,const string &sz_split,vector<string> &out_array,bool isSkipEmptyItem )
{
	if (sStr.empty() || sz_split.empty())
	{
		return false;
	}
	out_array.clear();
	std::size_t	ilastindex = 0;
	std::size_t	index = sStr.find(sz_split);
	while(index != string::npos)
	{
		string ss = sStr.substr(ilastindex,index-ilastindex);
		if (!ss.empty() || !isSkipEmptyItem)
		{
			out_array.push_back(ss);
		}
		ilastindex = index+1;
		index = sStr.find(sz_split,ilastindex);
	}
	out_array.push_back(sStr.substr(ilastindex));
	return (int)out_array.size();
}

int SplitParamStr(const char* str,std::map<string,string> &out,const char *spstr /* =  */,const char *opt_str /* = */ )
{
	if (!str)
	{
		return 0;
	}
	if (spstr == NULL || spstr[0] == 0)
	{
		spstr = ";";
	}
	if (opt_str == NULL || opt_str[0] == 0)
	{
		opt_str = "=";
	}
	std::vector<string> lst;
	int iLen1 = SplitStr(str,spstr,lst);
	for (int i=0;i<iLen1;i++)
	{
		string &s = lst[i];
		if (s.empty())
		{
			continue;
		}
		std::size_t npos = string::npos;
		std::size_t pos = s.find(opt_str);
		if (pos == npos)
		{
			out[s] = "[set]";
		}else if (pos == 0)
		{
			
		}else
		{
			string s1 = s.substr(0,pos);
			string s2 = s.substr(pos+1);
			out[s1] = s2;
		}
	}
	return (int)out.size();
}
