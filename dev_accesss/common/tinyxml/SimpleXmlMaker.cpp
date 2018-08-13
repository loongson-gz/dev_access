
////#include "stdafx.h"
#include "./SimpleXmlMaker.h"
#include "time.h"

#ifndef _WIN32
#	include <iconv.h>
#endif
#include "tinyxml.h"
using namespace std;

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

int SplitStr_x(const char *pData, const char *sz_split, vector<string> &out_array)
{
	if(!pData || !sz_split)
	{
		return 0;
	}

	string s1 = pData;
	string s2 = sz_split;
	return SplitStr_x(s1, s2, out_array);
}
int SplitStr_x(const string &sStr, const string &sz_split, vector<string> &out_array)
{
	if(sStr.empty() || sz_split.empty())
	{
		return false;
	}

	out_array.clear();
	std::size_t	ilastindex = 0;
	std::size_t	index = sStr.find(sz_split);

	while(index != string::npos)
	{
		out_array.push_back(sStr.substr(ilastindex, index - ilastindex));
		ilastindex = index + 1;
		index = sStr.find(sz_split, ilastindex);
	}

	out_array.push_back(sStr.substr(ilastindex));
	return (int)out_array.size();
}

int ArrayToString(std::string &outstr, const void *hex_array, const int iLen,int fmt)
{
	if(!hex_array || !iLen)
	{
		outstr = "";
		return 0;
	}

	std::stringstream ss;
	ss << std::hex  << std::setfill('0');
	//unsigned int flag = ss.flags();

	const char *pSpace = "";
	const char *p0x = "";
	bool isUper = false;

	if (fmt == 1 || fmt == 2 || fmt == 5 || fmt == 6)
	{
		pSpace =" ";
	}
	if (fmt == 2)
	{
		p0x = "0x";
	}else if (fmt == 6)
	{
		p0x = "0X";
	}

	if (fmt == 4 || fmt == 5 || fmt == 6)
	{
		isUper = true;
	}

	unsigned char *pArray = (unsigned char *)hex_array;

	for(int i = 0; i < iLen; i++)
	{

		if(isUper)
		{
			ss << setiosflags(ios::uppercase);
		}
		ss << p0x;

		//ss.flags(flag);
		ss << std::setw(2) << (int)(pArray[i]);

		if(i != iLen - 1)
		{
			ss << pSpace;
		}
	}

	outstr = ss.str();
	return outstr.size();
}

char _ToHexChar(int ch_hexstr)
{
	char c = (char)(ch_hexstr & 0xFF);

	if(ch_hexstr >= '0' && ch_hexstr <= '9')
	{
		return ch_hexstr - '0';
	}

	if(ch_hexstr >= 'A' && ch_hexstr <= 'F')
	{
		return ch_hexstr - 'A' + 10;
	}

	if(ch_hexstr >= 'a' && ch_hexstr <= 'f')
	{
		return ch_hexstr - 'a' + 10;
	}

	if(ch_hexstr == ' ' || ch_hexstr == ',')
	{
		return -1;
	}

	return -2;
}

/*
.	字符串 / 十六进制数组 转换通用模版
.	模版参数: _chartype : char  / wchar_t
.			  _hextype  : char/short/int/int64 以及对应 unsigned 版本
.	函数参数: _chartype str: 输入源字符串
.			  _hextype  *out:输出数组
.			  int	buflen: out 数组的大小.
.	返回值: 输出的数组大小. ( <= buflen )
.说明:
.	字符串解释时,遇到字符串结束,或者缓冲区长度不足时结束识别.
.	任何非 0-9A-Fa-f 的字符(包括 0x/0X) 均作为分组间隔字符.
.	支持任意个空格(或特殊符号)作为分隔符,支持字符串带有任意个0x前缀.
.支持的字符串示例:
.	aa bb,CC  ,  dD		: 以任意个空格或逗号或其他特殊字符间隔
.	AABBcCdD			: 无任何间隔.
.	0xaa bb 0xcc 0XdD	: 全部或部分 以0x开头的字符串
.	a b cc d 0xa		: 单个字符( a 将识别为 0a )
.	ahfi				: h 和 f 为无效字符,将按间隔符处理. 识别为:  0a 0f
.注意:以下格式为有歧义的格式(主要是由 0x 引起的歧义.),请不要使用.
.	a0xb				: 等价于: a0 x b ,识别为: a0 0b
.	0a0xb				: 等价于: 0a 0xb ,识别为: 0a 0b
.	a,,,,b				: 等价于: a b	 ,识别为: 0a 0b
*/
template<typename _chartype, typename _hextype>
int _Template_HexstrToArray(const _chartype *str, _hextype *out, int buflen)
{
	static const int iSize = sizeof(_hextype) * 2;

	if(!str || !out)
	{
		return 0;
	}

	int i = 0; //str index
	int j = 0; //bit index
	int ss = 0; //sum
	_hextype hextmp;
	char ctype;

	while(str[i])
	{
		if(buflen && ss >= buflen) //判断输出缓冲区长度
		{
			break;
		}

		//如果是 0x / 0X ,则丢弃
		if(str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		{
			i += 2;
			continue;
		}

		//分组..
		hextmp = 0;
		ctype = 0;

		for(j = 0; j < iSize && ctype >= 0; j++)
		{
			ctype = _ToHexChar(str[i]);
			i++;

			if(ctype >= 0)
			{
				hextmp = (hextmp << 4) | ctype;
			}
			else
			{
				continue;
			}
		}

		if(j == 1) //该分组无效.丢弃
		{
			continue;
		}

		out[ss] = hextmp;
		ss++;
	}

	return ss;
}

//=============================================
SimpleXMLParser::SimpleXMLParser()
{
	m_doc = NULL;
	m_currNode = NULL;
}

SimpleXMLParser::SimpleXMLParser(cchar *xml_str)
{
	m_doc = NULL;
	m_currNode = NULL;
	_create_xml_(xml_str);
}
SimpleXMLParser::~SimpleXMLParser()
{
	delete m_doc;
	m_doc = NULL;
	m_currNode = NULL;
}


//------------------------------
void SimpleXMLParser::str(std::string &out_str, bool isCleanStr)
{
	if(isCleanStr)
	{
		out_str.clear();
	}

	if(m_doc == NULL)
	{
		return;
	}

	TiXmlPrinter prit;
	m_doc->Accept(&prit);
	out_str += prit.CStr();
	return;
}

std::string SimpleXMLParser::str()
{

	if(m_doc == NULL)
	{
		return "";
	}

	TiXmlPrinter prit;
	m_doc->Accept(&prit);
	return prit.CStr();
}

//----- get value
const char *SimpleXMLParser::GetValue(cchar *title)
{
	if(!m_doc)
	{
		return NULL;
	}

	TiXmlElement *node = NULL;

	if(!_goto(title, node))
	{
		return NULL;
	}

	if(!node)
	{
		return NULL;
	}

	const char *p = node->GetText();

	return p;
}
bool SimpleXMLParser::GetValue(cchar *title, std::string &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	out = p;
	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, char *out, unsigned int iOutBufSize)
{

	if(!out || iOutBufSize == 0)
	{
		return false;
	}

	std::string str ;


	if(!GetValue(title, str))
	{
		return false;
	}

	if(str.length() >= iOutBufSize)
	{
		return false;
	}

	strcpy(out, str.c_str());
	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, int &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	if(sscanf(p, "%d", &out) != 1)
	{
		return false;
	}

	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, unsigned int &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	if(sscanf(p, "%u", &out) != 1)
	{
		return false;
	}

	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, float &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	if(sscanf(p, "%f", &out) != 1)
	{
		return false;
	}

	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, double &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	if(sscanf(p, "%lf", &out) != 1)
	{
		return false;
	}

	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, short &out)
{
	int iv = 0;
	if (GetValue(title,iv))
	{
		out = (short)iv;
		return true;
	}
	return false;
}
bool SimpleXMLParser::GetValue(cchar *title, unsigned short &out)
{
	unsigned int iv = 0;
	if (GetValue(title,iv))
	{
		out = (unsigned short)iv;
		return true;
	}
	return false;
}

bool SimpleXMLParser::GetValue(cchar *title, unsigned char &out)
{
	unsigned int iv = 0;
	if (GetValue(title,iv))
	{
		out = (unsigned char )iv;
		return true;
	}
	return false;
}
bool SimpleXMLParser::GetValue(cchar *title, char &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}
	int iv = 0;
	if(sscanf(p, "%d", &iv) != 1)
	{
		out = p[0];
	}

	return true;
}


bool SimpleXMLParser::GetValue(cchar *title, time_t &tt)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	struct tm t;

	if(6 != sscanf(p, "%d-%d-%d %d:%d:%d",
	               &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec))
	{
		return false;
	}

	t.tm_year -= 1900;
	t.tm_mon -= 1;

	tt = ::mktime(&t);
	return true;
}

bool SimpleXMLParser::GetValue(cchar *title, bool &out)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	out = true;

	switch(p[0])
	{
	case '0':
	case 'f':
	case 'F':
	case 'n':
	case 'N':
		out = false;
		break;

	case 'T':
	case 't':
	case '1':
	case 'y':
	case 'Y':
		out = true;
		break;

	default:
		break;
	}

	return true;
}
bool SimpleXMLParser::GetValue(cchar *title, unsigned char *out, int &in_out_bufsize)
{
	cchar *p = GetValue(title);

	if(p == NULL)
	{
		return false;
	}

	int iBufsize = in_out_bufsize;
	int iArraySize = _Template_HexstrToArray(p, out, iBufsize);
	in_out_bufsize = iArraySize;
	return true;
}

//------------------
bool SimpleXMLParser::Parse(cchar *xml)
{
	if(!m_doc)
	{
		m_doc = new TiXmlDocument();
	}

	m_doc->Clear();
	m_doc->Parse(xml);

	if(m_doc->Error())
	{
		m_doc->Clear();
		return false;
	}

	return true;
}

bool SimpleXMLParser::ParseFile(cchar *xml_file)
{
	if(!m_doc)
	{
		m_doc = new TiXmlDocument();
	}

	m_doc->Clear();
	m_doc->LoadFile(xml_file);

	if(m_doc->Error())
	{
		m_doc->Clear();
		return false;
	}

	return true;
}

bool SimpleXMLParser::_create_xml_(cchar *xml_str/* =NULL */)
{
	if(m_doc)
	{
		return false;
	}

	if(xml_str != NULL)
	{
		m_doc = new TiXmlDocument();
		m_doc->Parse(xml_str);

		if(m_doc->Error())
		{
			delete m_doc;
			m_doc = NULL;
		}
		else
		{
			TiXmlElement *root = m_doc->RootElement();

			if(root == NULL)
			{
				delete m_doc;
				m_doc = NULL;
			}
		}
	}

	if(m_doc == NULL)
	{
		m_doc = new TiXmlDocument;
		TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "gbk", "");
		m_doc->LinkEndChild(dec);
	}

	m_currNode = NULL;
	//m_currTxt = NULL;

	return m_doc != NULL;
}

bool SimpleXMLParser::_goto(cchar *path, TiXmlElement *&out)
{
	if(m_doc == NULL)
	{
		_create_xml_();
	}

	typedef vector<string> vstr;
	typedef vstr::iterator vstr_iter;
	TiXmlElement *parent = m_currNode;
	vstr strlst ;

	if(path == NULL || path[0] == 0
	        || (path[0] == '.' && path[1] == 0))
	{
		out = m_currNode;
		return true;
	}

	if(path[0] == '/' && path[1] == 0)
	{
		out = NULL;
		return true;
	}

	if(SplitStr_x(path, "/", strlst))
	{
		if(path[0] == '/')
		{
			parent = NULL;
		}

		for(size_t i = 0; i < strlst.size(); i++)
		{
			std::string &str = strlst[i];
			size_t iFind = str.find(":");
			int iIndex = 0;

			if(iFind != string::npos)
			{
				if(sscanf(str.c_str() + iFind, ":%d", &iIndex) != 1)
				{
					iIndex = 0;
				}

				str[iFind] = '\0';//非标准用法..
			}

			if(str[0] != '\0')
			{
				bool isFind = false;
				parent = _find_node(parent, str.c_str(), iIndex, NULL, &isFind);

				if(!isFind)
				{
					return false;
				}
			}
		}
	}

	//return parent;
	//m_currNode = parent;
	out = parent;
	return true;
}
bool SimpleXMLParser::Goto(cchar *path)
{
	bool iRes = _goto(path, m_currNode);

	return iRes;
}
bool SimpleXMLParser::GotoFirstChild(cchar *path)
{
	TiXmlElement *next = NULL;

	if(!m_doc)
	{
		return false;
	}

	if(m_currNode == NULL)
	{
		if(path == NULL || path[0] == 0)
		{
			next = m_doc->FirstChildElement();
		}
		else
		{
			next = m_doc->FirstChildElement();
		}
	}
	else
	{
		if(path == NULL || path[0] == 0)
		{
			next = m_currNode->FirstChildElement();
		}
		else
		{
			next = m_currNode->FirstChildElement();
		}
	}

	if(next == NULL)
	{
		return false;
	}

	m_currNode = next;
	return true;
}

bool SimpleXMLParser::Next(cchar *path)
{
	if(m_currNode == NULL)
	{
		return false;
	}

	TiXmlElement *next = NULL;

	if(path == NULL || path[0] == 0)
	{
		next = m_currNode->NextSiblingElement();
	}
	else
	{
		next = m_currNode->NextSiblingElement(path);
	}

	if(next == NULL)
	{
		return false;
	}

	m_currNode = next;
	return true;
}

const char *SimpleXMLParser::GetNodeName(cchar *path)
{
	TiXmlElement *ptr = NULL;

	if(_goto(path, ptr))
	{
		if(ptr)
		{
			return ptr->Value();
		}
	}

	return NULL;
}

TiXmlElement *SimpleXMLParser::_find_node(TiXmlElement *parent, cchar *title, int index, int *out_i, bool *out_isFind)
{
	TiXmlElement *p = NULL;
	int i = 0;
	bool isFind = false;

	if(out_isFind)
	{
		*out_isFind = true;
	}

	if(title == NULL || title[0] == 0)
	{
		return parent;
	}
	else if(strcmp(title, ".") == 0)
	{
		return parent;
	}
	else if(strcmp(title, "..") == 0)
	{
		if(parent)
		{
			parent = (TiXmlElement *)parent->Parent();
		}

		return parent;
	}

	if(parent != NULL)
	{
		p = parent->FirstChildElement();

	}
	else
	{
		//从根节点开始查找
		p = m_doc->FirstChildElement();
	}

	for(; p; p = p->NextSiblingElement())
	{
		if(strcmp(title, p->Value()) == 0)
		{
			if(i == index)
			{
				isFind = true;
				break;
			}
			else
			{
				i++;
			}
		}
	}

	if(out_isFind)
	{
		*out_isFind = isFind;
	}

	if(out_i)
	{
		*out_i = i;
	}

	if(isFind)
	{
		return p;
	}

	return NULL;
}


//====================================================================

SimpleXMLMaker::SimpleXMLMaker()
{

}

SimpleXMLMaker::SimpleXMLMaker(cchar *xml_str)
	: SimpleXMLParser(xml_str)
{
}
SimpleXMLMaker::~SimpleXMLMaker()
{
}


TiXmlElement *SimpleXMLMaker::_find_and_create_node(TiXmlElement *parent, cchar *title, int index)
{
	int i = 0;
	bool isFind = true;
	TiXmlElement *p = _find_node(parent, title, index, &i, &isFind);

	if(isFind)
	{
		return p;
	}

	for(; i <= index; i++)
	{
		p = new TiXmlElement(title);

		if(parent == NULL)
		{
			m_doc->LinkEndChild(p);
		}
		else
		{
			parent->LinkEndChild(p);
		}
	}

	return p;
}

TiXmlElement *SimpleXMLMaker::_create_mult_node(cchar *path)
{
	if(m_doc == NULL)
	{
		_create_xml_();
	}

	typedef vector<string> vstr;
	typedef vstr::iterator vstr_iter;
	TiXmlElement *parent = m_currNode;
	vstr strlst ;

	if(SplitStr_x(path, "/", strlst))
	{
		if(path[0] == '/')
		{
			parent = NULL;
		}

		for(size_t i = 0; i < strlst.size(); i++)
		{
			std::string &str = strlst[i];
			size_t iFind = str.find(":");
			int iIndex = 0;

			if(iFind != string::npos)
			{
				if(sscanf(str.c_str() + iFind, ":%d", &iIndex) != 1)
				{
					iIndex = 0;
				}

				str[iFind] = '\0';//非标准用法..
			}

			parent = _find_and_create_node(parent, str.c_str(), iIndex);
		}
	}

	return parent;
}


bool SimpleXMLMaker::SimpleAddNode(cchar *path)
{
	if(path == NULL)
	{
		return false;
	}

	if(m_doc == NULL)
	{
		_create_xml_();
	}

	TiXmlElement *p = new TiXmlElement(path);

	if(m_currNode == NULL)
	{
		m_doc->LinkEndChild(p);
	}
	else
	{
		m_currNode->LinkEndChild(p);
	}

	m_currNode = p;
	return true;
}

bool SimpleXMLMaker::AddNode(cchar *path)
{
	if(m_doc == NULL)
	{
		_create_xml_();
	}

	if(path == NULL)
	{
		return false;
	}

	TiXmlElement *p = _create_mult_node(path);
	m_currNode = p;
	return true;
}

bool SimpleXMLMaker::AddValue(cchar *title, cchar *val)
{
	if(title == NULL)
	{
		return false;
	}

	if(val == NULL)
	{
		val = "";
	}

	if(m_doc == NULL)
	{
		_create_xml_();
	}

	TiXmlElement *p = _create_mult_node(title);

	if(p == NULL)
	{
		return false;
	}

	TiXmlText *t = new TiXmlText(val);
	p->LinkEndChild(t);

	return true;
}

bool SimpleXMLMaker::UpdateValue(cchar *title, cchar *val)
{
	if(title == NULL)
	{
		return false;
	}

	if(val == NULL)
	{
		val = "";
	}

	if(m_doc == NULL)
	{
		_create_xml_();
	}

	TiXmlElement *pNode = NULL;
	if (!_goto(title,pNode))
	{
		return AddValue(title,val);
	}
	pNode->Clear();
	TiXmlText *t = new TiXmlText(val);
	pNode->LinkEndChild(t);
	return true;
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, cchar *val)
{
	if(title == NULL)
	{
		return false;
	}

	if(val == NULL)
	{
		val = "";
	}

	if(m_doc == NULL)
	{
		_create_xml_();
	}

	TiXmlElement *p = new TiXmlElement(title);

	if(m_currNode == NULL)
	{
		m_doc->LinkEndChild(p);
	}
	else
	{
		m_currNode->LinkEndChild(p);
	}

	TiXmlText *t = new TiXmlText(val);
	p->LinkEndChild(t);
	return true;
}

//-------------------------------
bool SimpleXMLMaker::AddValue(cchar *title, const unsigned char *val, int bufflen,int fmt)
{
	std::string str;
	ArrayToString(str, (void*)val, bufflen,fmt);
	return AddValue(title, str.c_str());
}
bool SimpleXMLMaker::AddValue(cchar *title, bool val)
{
	if(val)
	{
		return AddValue(title, "true");
	}

	return AddValue(title, "false");
}
bool SimpleXMLMaker::AddValue(cchar *title, const unsigned int &val)
{
	char buf[64] = "";
	sprintf(buf, "%u", val);
	return AddValue(title, buf);
}
bool SimpleXMLMaker::AddValue(cchar *title, int val)
{
	char buf[64] = "";
	sprintf(buf, "%d", val);
	return AddValue(title, buf);
}
bool SimpleXMLMaker::AddValue(cchar *title, const float &val)
{
	char buf[64] = "";
	sprintf(buf, "%f", val);
	return AddValue(title, buf);
}
bool SimpleXMLMaker::AddValue(cchar *title, const double &val)
{
	char buf[64] = "";
	sprintf(buf, "%lf", val);
	return AddValue(title, buf);
}
bool SimpleXMLMaker::AddValue(cchar *title, const time_t &tt)
{
	char buf[32] = "";
	struct tm t = *::localtime(&tt);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
	        t.tm_year + 1900 , t.tm_mon + 1 , t.tm_mday,
	        t.tm_hour, t.tm_min, t.tm_sec);
	return AddValue(title, buf);
}
bool SimpleXMLMaker::AddValue(cchar *title, const std::string &val)
{
	return AddValue(title, val.c_str());
}
//-------------------------------
bool SimpleXMLMaker::UpdateValue(cchar *title, const unsigned char *val, int bufflen,int fmt)
{
	std::string str;
	ArrayToString(str, (void*)val, bufflen,fmt);
	return UpdateValue(title, str.c_str());
}
bool SimpleXMLMaker::UpdateValue(cchar *title, bool val)
{
	if(val)
	{
		return UpdateValue(title, "true");
	}

	return UpdateValue(title, "false");
}
bool SimpleXMLMaker::UpdateValue(cchar *title, const unsigned int &val)
{
	char buf[64] = "";
	sprintf(buf, "%u", val);
	return UpdateValue(title, buf);
}
bool SimpleXMLMaker::UpdateValue(cchar *title, int val)
{
	char buf[64] = "";
	sprintf(buf, "%d", val);
	return UpdateValue(title, buf);
}
bool SimpleXMLMaker::UpdateValue(cchar *title, const float &val)
{
	char buf[64] = "";
	sprintf(buf, "%f", val);
	return UpdateValue(title, buf);
}
bool SimpleXMLMaker::UpdateValue(cchar *title, const double &val)
{
	char buf[64] = "";
	sprintf(buf, "%lf", val);
	return UpdateValue(title, buf);
}
bool SimpleXMLMaker::UpdateValue(cchar *title, const time_t &tt)
{
	char buf[32] = "";
	struct tm t = *::localtime(&tt);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
		t.tm_year + 1900 , t.tm_mon + 1 , t.tm_mday,
		t.tm_hour, t.tm_min, t.tm_sec);
	return UpdateValue(title, buf);
}
bool SimpleXMLMaker::UpdateValue(cchar *title, const std::string &val)
{
	return UpdateValue(title, val.c_str());
}
//-------------------------------------------
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const unsigned char *val, int bufflen,int fmt)
{
	std::string str;
	ArrayToString(str, val, bufflen,fmt);
	return SimpleAddValue(title, str.c_str());
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, bool val)
{
	if(val)
	{
		return SimpleAddValue(title, "true");
	}

	return SimpleAddValue(title, "false");
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const unsigned int &val)
{
	char buf[64] = "";
	sprintf(buf, "%u", val);
	return SimpleAddValue(title, buf);
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const time_t &tt)
{
	char buf[64] = "";
	struct tm t = *::localtime(&tt);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
	        t.tm_year + 1900 , t.tm_mon + 1 , t.tm_mday,
	        t.tm_hour, t.tm_min, t.tm_sec);
	return SimpleAddValue(title, buf);
}

bool SimpleXMLMaker::SimpleAddValue(cchar *title, int val)
{
	char buf[64] = "";
	sprintf(buf, "%d", val);
	return SimpleAddValue(title, buf);
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const float &val)
{
	char buf[64] = "";
	sprintf(buf, "%f", val);
	return SimpleAddValue(title, buf);
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const double &val)
{
	char buf[64] = "";
	sprintf(buf, "%lf", val);
	return SimpleAddValue(title, buf);
}
bool SimpleXMLMaker::SimpleAddValue(cchar *title, const std::string &val)
{
	return SimpleAddValue(title, val.c_str());
}

bool SimpleXMLMaker::EraseValue(cchar *n_title)
{
	TiXmlElement *pNode = NULL;
	if (_goto(n_title,pNode))
	{
		TiXmlNode *pParent = pNode->Parent();
		if (pParent)
		{
			pParent->RemoveChild(pNode);
		}else
		{
			m_doc->RemoveChild(pNode);
		}
		if (pNode == m_currNode)
		{
			m_currNode = NULL;
		}
		return true;
	}
	return false;
}

bool SimpleXMLMaker::SaveToFile(cchar *szFile)
{
	if (!szFile)
	{
		return false;
	}
	//std::string sz = szFile;
	m_doc->SaveFile(szFile);
	return true;
}

