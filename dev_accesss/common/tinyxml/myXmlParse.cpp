////#include "stdafx.h"
#include "myXmlParse.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//---- tinyxml ¸¨Öú
const char * _xmlGetNodeElement(TiXmlNode *pNode, const char *outStr)
{

	if (outStr == NULL)
		outStr = "";

	if (!pNode) {

		return outStr;
	}

	TiXmlElement *pElement = pNode->ToElement();
	if (!pElement) 
	{
		return outStr;
	}

	const char *valStr = pElement->GetText();
	if (valStr != NULL)
	{
		outStr = valStr;
	}

	return outStr;
}

const char *_xmlGetFirstElement(TiXmlElement *pNode,const char *pTitle,const char *default_str /* = NULL */)
{
	if (default_str == NULL)
	{
		default_str = "";
	}
	if (!pNode)
	{
		return default_str;
	}
	TiXmlElement *pItem = pNode->FirstChildElement(pTitle);
	if (!pItem)
	{
		return default_str;
	}
	const char *pTxt = pItem->GetText();
	if (pTxt)
	{
		return pTxt;
	}
	return default_str;
}
const char *_xmlGetElementAttr(TiXmlElement *pNode,const char *pTitle,const char *pAttrName,const char *default_str /* = NULL */)
{
	if (default_str == NULL)
	{
		default_str = "";
	}
	if (!pNode || !pAttrName || !pTitle)
	{
		return default_str;
	}
	TiXmlElement *pItem = pNode->FirstChildElement(pTitle);
	if (!pItem)
	{
		return default_str;
	}
	const char *pAttr = pItem->Attribute(pAttrName);
	if (pAttr)
	{
		return pAttr;
	}
	return default_str;
}
TiXmlElement * _xmlGetFirstElement(TiXmlElement *pNode,const char *pTitle,tstring &out_value,const char *default_str)
{
	out_value = "";
	if (!pNode)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return NULL;
	}
	TiXmlElement *pItem;
	pItem = pNode->FirstChildElement(pTitle);
	if (!pItem)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return pItem;
	}
	const char *pText = pItem->GetText();
	if (!pText)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return pItem;
	}
	out_value = pText;
	return pItem;
}

TiXmlElement * _xmlGetNextElement(TiXmlElement *pNode,const char *pTitle,tstring &out_value,const char *default_str)
{
	out_value = "";
	if (!pNode)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return NULL;
	}
	TiXmlElement *pItem;
	pItem = pNode->NextSiblingElement(pTitle);
	if (!pItem)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return pItem;
	}
	const char *pText = pItem->GetText();
	if (!pText)
	{
		if (default_str)
		{
			out_value = default_str;
		}
		return pItem;
	}
	out_value = pText;
	return pItem;
}

tstring &_xmlstrAddLine(tstring &out,const char *pTitle,int Value,int iSpaceSum /* = 0 */)
{
	const char *space[8] = {
		"","  ","    ","      ","        ","          ","            ","              "
	};
	//tstring line;
	char line[8192];
	sprintf(line,"%s<%s>%d</%s>\r\n",space[iSpaceSum],pTitle,Value,pTitle);
	out += line;
	return out;
}
tstring &_xmlstrAddLine(tstring &out,const char *pTitle,const char *pValue,int iSpaceSum /* = 0 */)
{
	const char *space[8] = {
		"","  ","    ","      ","        ","          ","            ","              "
	};
	//tstring line;
	char line[8192];
	sprintf(line,"%s<%s>%s</%s>\r\n",space[iSpaceSum],pTitle,pValue,pTitle);
	out += line;
	return out;
}
tstring &_xmlstrAddLine(tstring &out,const char *pTitle,const tstring &pValue,int iSpaceSum /* = 0 */)
{
	const char *space[8] = {
		"","  ","    ","      ","        ","          ","            ","              "
	};
	char line[8192];
	sprintf(line,"%s<%s>%s</%s>\r\n",space[iSpaceSum],pTitle,pValue.c_str(),pTitle);
	out += line;
	return out;
}
tstring &_xmlstrAddLineNode(tstring &out,const char *pTitle,int iSpaceSum /* = 0 */)
{
	const char *space[8] = {
		"","  ","    ","      ","        ","          ","            ","              "
	};
	char line[8192];
	sprintf(line,"%s<%s>\r\n",space[iSpaceSum],pTitle);
	out += line;
	return out;
}


SimpleXmlMaker::SimpleXmlMaker(int iDefaultSpace /* =0 */,const char *default_value /* = NULL */)
{
	m_iSpace = iDefaultSpace;
	if (default_value)
	{
		m_str = default_value;
	}
	if (m_iSpace<0)
	{
		m_iSpace = 0;
	}
}
SimpleXmlMaker::~SimpleXmlMaker()
{
	while(!m_nodelst.empty())
	{
		m_nodelst.pop();
	}
}

SimpleXmlMaker &SimpleXmlMaker::AddHead(const char *ver /* = NULL */,const char *encode /* = NULL */)
{
	char buf[256];
	if (ver && encode)
	{
		sprintf(buf,"<?xml version=\"%s\" encoding=\"%s\" ?>",ver,encode);
	}else if (ver)
	{
		sprintf(buf,"<?xml version=\"%s\" ?>",ver,encode);
	}else if (encode)
	{
		sprintf(buf,"<?xml encoding=\"%s\" ?>",ver,encode);
	}else
	{
		sprintf(buf,"<?xml ?>");
	}
	AddText(buf);
	return *this;
}

SimpleXmlMaker&SimpleXmlMaker::AddNode(const char *title)
{
	if (title)
	{
		char line[2048];
		sprintf(line,"%s<%s>\r\n",space(m_iSpace),title);
		m_str += line;
		m_iSpace++;
		m_nodelst.push(title);
	}
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::AddNode(const char *title,const char *fmt,...)
{
	if (!fmt)
	{
		return AddNode(title);
	}
	if (title)
	{
		va_list al;
		va_start(al,fmt);
		char param[1024];
		char line[2048];
		vsprintf(param,fmt,al);
		sprintf(line,"%s<%s %s>\r\n",space(m_iSpace),title,param);
		va_end(al);
		m_str += line;
		m_iSpace++;
		m_nodelst.push(title);
	}
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::EndNode()
{
	if (!m_nodelst.empty())
	{
		tstring &s = m_nodelst.top();
		m_iSpace--;
		if (m_iSpace<0)
		{
			m_iSpace = 0;
		}
		char line[2048];
		sprintf(line,"%s</%s>\r\n",space(m_iSpace),s.c_str());
		m_str += line;
		m_nodelst.pop();
	}
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::AddValueV(const char *title,const char *fmt,...)
{
	if (fmt)
	{
		va_list al;
		va_start(al,fmt);
		//tstring param,line;
		char line[2048];
		char param[1024];
		vsprintf(param,fmt,al);
		va_end(al);
		sprintf(line,"%s<%s>%s</%s>\r\n",space(m_iSpace),title,param,title);
		m_str += line;
	}else
	{
		return AddValue(title,fmt);
	}
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::AddValue(const char *title,const char *val,const char *if_val_empty /* = NULL */)
{
	char line[2048];
	if (val == NULL || val[0] == 0 )
	{
		if(if_val_empty)
		{
			val = if_val_empty;
		}
	}
	sprintf(line,"%s<%s>%s</%s>\r\n",space(m_iSpace),title,val,title);
	m_str += line;
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::AddValue(const char *title,const tstring &val,const char *if_val_empty /* = NULL */)
{
	return AddValue(title,val.c_str(),if_val_empty);
}
SimpleXmlMaker&SimpleXmlMaker::AddValue(const char *title,int val,const char *if_val_empty /* = NULL */)
{
	if (val == 0 && if_val_empty)
	{
		return AddValue(title,if_val_empty);
	}
	char line[2048];
	sprintf(line,"%s<%s>%d</%s>\r\n",space(m_iSpace),title,val,title);
	m_str += line;
	return *this;
}
SimpleXmlMaker&SimpleXmlMaker::AddText(const char *txt,int iSpace /* = -1 */)
{
	if (txt)
	{
		if (iSpace == -1)
		{
			iSpace = m_iSpace;
		}
		char line[2048];
		sprintf(line,"%s%s\r\n",space(iSpace),txt);
		m_str += line;
	}
	return *this;
}

tstring &SimpleXmlMaker::str()
{
	return m_str;
}
void SimpleXmlMaker::str(const char *s,int ispacenum /* = -1 */)
{
	if (s)
	{
		m_str = s;
	}else
	{
		m_str = "";
	}
	if (ispacenum>=0)
	{
		m_iSpace = ispacenum;
	}
	while(!m_nodelst.empty())
	{
		m_nodelst.pop();
	}
}
const char *SimpleXmlMaker::space(int i)
{
	static const char *_space[8] = {
		"","  ","    ","      ","        ","          ","            ","              "
	};
	if (i < 0)
	{
		i = 0;
	}
	if (i >= 8)
	{
		i = 7;
	}
	return _space[i];
}
