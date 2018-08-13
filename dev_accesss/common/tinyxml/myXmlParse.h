
#ifndef _MY_XML_PARSE_H__
#define _MY_XML_PARSE_H__

#include "tinyxml.h"
#include <string>
#include <stack>
typedef std::string tstring;

/*****************************************************************************/
//--- tinyXml¸¨Öú
const char * _xmlGetNodeElement(TiXmlNode *pNode, const char *outStr=NULL);
TiXmlElement * _xmlGetFirstElement(TiXmlElement *pNode,const char *pTitle,tstring &out_value,const char *default_str = NULL);
const char * _xmlGetFirstElement(TiXmlElement *pNode,const char *pTitle,const char *default_str = NULL);
const char * _xmlGetElementAttr(TiXmlElement *pNode,const char *pTitle,const char *pAttrName,const char *default_str = NULL);
TiXmlElement * _xmlGetNextElement(TiXmlElement *pNode,const char *pTitle,tstring &out_value,const char *default_str = NULL);

//const char * _xmlGetNextElement(TiXmlElement *pNode,const char *pTitle,const char *default_str = NULL);
tstring &_xmlstrAddLine(tstring &out,const char *pTitle,const char *pValue,int iSpaceSum = 0);
tstring &_xmlstrAddLine(tstring &out,const char *pTitle,const tstring &pValue,int iSpaceSum = 0);
tstring &_xmlstrAddLine(tstring &out,const char *pTitle,int Value,int iSpaceSum = 0);
tstring &_xmlstrAddLineNode(tstring &out,const char *pTitle,int iSpaceSum = 0);
//*
class SimpleXmlMaker
{
public:
	SimpleXmlMaker(int iDefaultSpace =0,const char *default_value = NULL);
	~SimpleXmlMaker();
	SimpleXmlMaker &AddHead(const char *ver = NULL,const char *encode = NULL);
	SimpleXmlMaker &AddNode(const char *title);
	SimpleXmlMaker &AddNode(const char *title,const char *fmt,...);
	SimpleXmlMaker &EndNode();
	SimpleXmlMaker &AddValueV(const char *title,const char *fmt,...);
	SimpleXmlMaker &AddValue(const char *title,const char *val,const char *if_val_empty = NULL);
	SimpleXmlMaker &AddValue(const char *title,const tstring &val,const char *if_val_empty = NULL);
	SimpleXmlMaker &AddValue(const char *title,int val,const char *if_val_empty = NULL);
	SimpleXmlMaker &AddText(const char *txt,int iSpace = -1);

	//void str(tstring &s);
	tstring &str() ;
	void str(const char *s,int ispacenum = -1);
	const char *c_str() const
	{
		return m_str.c_str();
	}
	static const char *space(int i);

public:
	tstring m_str;
	int m_iSpace;
	std::stack<tstring>	m_nodelst;

};
//*/

#endif
