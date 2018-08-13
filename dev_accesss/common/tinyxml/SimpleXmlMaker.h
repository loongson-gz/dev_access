
/*

.	SimpleXMLMaker: 方便构造xml字符串
SimpleXMLMaker maker;
maker.SimpleAddNode("Get");			// "<Get/>"
maker.SimpleAddValue("val1","v");	// "<Get> <val1> v </val1> </Get>"
maker.AddValue("/Root/Get/Val","xxx"); //可以直接添加多层节点,用"/"分割.
maker.AddValue("/Root/Get:1/Val:2","mmm");//如果某节点有多个名字相同的子节点,
							//可以用 ":数字" 的方式指定使用第几个节点.从0开始计算.
maker.Goto("/");
for(int i=0;i<2;i++)// for 批量添加示例.
{
	maker.SimpleAddNode("Set");//添加Set节点,并进入Set节点.
	maker.SimpleAddValue("val",i);//在Set节点添加 val=i 的值.
	maker.Goto(".."); //返回上一层节点.
}

std::string str;
maker.str(str); 或  str=maker.str(); 可以获取字符串.

str的值:
<?xml version="1.0" encoding="gbk" ?>
<Get>
	<val1>v</val1>
</Get>
<Root>
	<Get>
		<Val>xxx</Val>
	</Get>
	<Get>
		<Val />
		<Val />
		<Val>mmm</Val>
	</Get>
</Root>
<Set>
	<val>0</val>
</Set>
<Set>
	<val>1</val>
</Set>

分析器:

SimpleXMLMaker parse;
parse.Parse(str.c_str());
int i;
parse.GetValue("/Node1/Node2:2/Title",i);//获取一个值.可直接指定详细的路径.

//对于多个同一名字的父节点的情况,可用for + Goto/Next 来遍历:
for(bool it = parse.Goto("/Set"); it ;it = parse.Next("Set"))
{
	parse.GetValue("",i);
	cout<<i<<",";
}
输出:  0,1

//如果要遍历当前节点的所有子节点,可使用 for + GotoFirstChild/Next 来遍历.
for(bool it = parse.GotoFirstChild();it;it=parse.Next())
{
	const char *pTitle = parse.GetNodeName();//获取当前节点的名字
	const char *pVal = parse.GetValue(""); //获取当前节点的值.
}
parse.Goto("..");//注意:因为: GotoFirstChild() 已经改变了当前节点,遍历完成后需要返回上一层节点

*/

#ifndef _SIMPLE_XML_PARSER_H__
#define _SIMPLE_XML_PARSER_H__

//#include <sstream>
//#include "Common/tinyxml/tinyxml.h"
//#include <stack>
#include <string>
#include <vector>

class TiXmlElement;
class TiXmlDocument;
class TiXmlNode;


int SplitStr_x(const char *str, const char *spstr, std::vector<std::string> &ret_result);
int SplitStr_x(const std::string &str, const std::string &spstr, std::vector<std::string> &ret_result);



/*
.	简易xml生成器/解释器:
.
.	path二次解释规则:
.	如果是 / 开头,表示从根节点开始找,否则表示从当前节点开始.
.	特殊值:  根节点:	/
.			 当前位置: (空)		.	./
.			 上一节点:	..	../.
.
.	path格式: node1/node2:num/node3:num2/title ...
.	在节点后面可以跟随冒号+数字,表示查找第几个子节点.用于有多个节点的情况.
.	数字从0开始计算
.	如:
.			/Get/Set		Get下面的Set节点
.			/Get:1/Set:2	第1个Get节点下第2个Set节点
.	注意: 添加节点的时候,如果使用指定索引方式添加,且前一个索引是不存在的,将会自动添加该索引.
.	即:	如果添加 /Get:2 , 但 /Get:0 和 /Get:1 不存在,
.		就会先把 /Get:0 和 /Get:1 先加上,再添加 /Get:2
.
.	注: 参数名为:  path / title :     会 对路径进行二次解释.
.				 n_path / n_title : 不会 对路径进行二次解释

*/

class SimpleXMLParser
{
public:
	typedef const char cchar;
public:

	//输出字符串
	void str(std::string &out_str, bool isCleanStr = true);
	std::string str();

public:
	SimpleXMLParser();
	SimpleXMLParser(cchar *xml_str);
	~SimpleXMLParser();
	bool Parse(cchar *xml);
	bool ParseFile(cchar *xml_file);
public:

	/*
	.	跳转到某个节点.(一般用来返回根节点)
	.注:1)跳转会改变当前节点位置.
	.		2)当节点不存在时跳转失败
	.这个函数和 AddNode 的区别:
	.	AddNode : 如果路径不存在,则创建该路径,并跳转到最新创建的节点.
	.	Goto	: 如果路径不存在,则跳转失败,且不改变当前路径.
	*/
	bool Goto(cchar *path);

	/*
	.	跳转到当前节点第一个子节点.
	.不会对path进行二次解释.
	*/
	bool GotoFirstChild(cchar *n_path = NULL);

	//跳到当前节点下一个节点.
	//不对path进行二次解释
	//会修改当前节点位置.
	bool Next(cchar *path = NULL);


	/* 获取节点的名字. (不修改当前节点位置)
	. 参数一般是 "." ".." 等不确定的路径.(已经确定的路径就没有必要获取了)
	. 如果获取失败,返回 NULL
	*/
	cchar *GetNodeName(cchar *path = NULL);

	//------ xml 解释器

	//获取节点的值.(返回 false 时,不修改输出参数.)
	//title = "" 或 "." 或 NULL 表示获取当前节点的值.
	cchar *GetValue(cchar *title = NULL);
	bool GetValue(cchar *title, std::string &out);
	bool GetValue(cchar *title, char *out, unsigned int iOutBufSize = 0);
	bool GetValue(cchar *title, int &out);
	bool GetValue(cchar *title, unsigned int &out);
	bool GetValue(cchar *title, short &out);
	bool GetValue(cchar *title, unsigned short &out);
	bool GetValue(cchar *title, char &out);
	bool GetValue(cchar *title, unsigned char &out);
	bool GetValue(cchar *title, bool &out);
	bool GetValue(cchar *title, float &out);
	bool GetValue(cchar *title, double &out);
	bool GetValue(cchar *title, time_t &t);
	//获取二进制数据.. 要求节点的值必须是十六进制数组.
	//格式必须是以下其中一种:
	//		AA BB CC DD
	//		AA,BB,CC,DD
	//		a b,c , d
	//		AABBCCDD
	//		0xAA 0xBB 0xCC 0xDD
	//		0xAA0xBB0xCC0xDD
	//函数会尽可能对字符串进行十六进制翻译转换,
	//但在源字符串有错误或有歧义的情况下不保证准确性.
	//参数: out : 输出的缓冲区
	//.		in_out_bufsize : 同时作为输入/输出参数.
	//.						输入: out缓冲区的大小.
	//.						输出: out缓冲区已经使用的大小.
	//.本函数可以无错误解释 AddValue() 生成的十六进制字符串.
	bool GetValue(cchar *title, unsigned char *out, int &in_out_bufsize);

protected:
	bool _create_xml_(cchar *xml_str = NULL);
	TiXmlElement *_find_node(TiXmlElement *parent, cchar *title, int index, int *out_i = NULL, bool *out_isFind = NULL);
	bool _goto(cchar *path, TiXmlElement *&out);
protected:
	TiXmlDocument *m_doc;//
	TiXmlElement *m_currNode;//当前节点

};

class SimpleXMLMaker
	: public SimpleXMLParser
{
public:
	typedef const char cchar;

public:
	SimpleXMLMaker();
	SimpleXMLMaker(cchar *xml_str);
	~SimpleXMLMaker();

	/*
	.	在当前位置添加一个节点.
	.注: 1)不允许有多个相同名字的节点出现.如果一个节点已经存在,则不会再添加新的节点.
	.		添加新节点可以通过强制指定节点序号的方式指定.
	.	 2)添加一个节点后,当前位置将切换为新添加的节点
	.这个函数和 Goto 的区别:
	.	AddNode : 如果路径不存在,则创建该路径,并跳转到最新创建的节点.
	.	Goto	: 如果路径不存在,则跳转失败,且不改变当前路径.
	*/
	bool AddNode(cchar *path);


	/*
	.	在当前位置添加一个节点.
	.注: 1) 允许添加多个相同的节点.
	.	 2) 添加新节点时不会对现有节点进行任何检查.
	.	 3) 不对path进行二次解释.path整个字符串将直接作为节点名称进行添加.
	.和AddNode的区别是,AddNode会对节点进行完整检查,而SimpleAddNode则不检查.
	*/
	bool SimpleAddNode(cchar *n_path);


	/*
	.	在当前位置添加一个值
	.注:1)不允许出现相同的名称.如果出现,多个值将会合并在一起.
	.	2)添加值不改变当前位置.
	.
	.十六进制方式:
	.	fmt		示例:
	.	0		aabbccdd
	.	1		aa bb cc dd
	.	2		0xaa 0xbb 0xcc 0xdd
	.	4		AABBCCDD
	.	5		AA BB CC DD
	.	6		0XAA 0XBB 0XCC 0XDD
	*/
	//bool AddValue(cchar *path,cchar *title,int val);
	bool AddValue(cchar *title, cchar *val);//字符串
	bool AddValue(cchar *title, const std::string &val);//字符串
	bool AddValue(cchar *title, int val);//整数
	bool AddValue(cchar *title, const unsigned int &val);//32位无符号
	bool AddValue(cchar *title, bool val);//true/false
	bool AddValue(cchar *title, const time_t &t);//time_t
	bool AddValue(cchar *title, const unsigned char *val, int bufflen , int fmt = 1); // 16进制字符串
	bool AddValue(cchar *title, const float &val);//
	bool AddValue(cchar *title, const double &val);//

	/* 更新值.
	.	如果需要改变已有节点的数据,请调用这个函数.
	.	如果title节点不存在,则等价于 AddValue()
	.
	.	此函数与 AddValue() 的区别是:
	.	对于已经存在的title , AddValue() 是将新的值叠加到旧的值里面., 等价于  += 操作
	.						UpdateValue() 则是用新的值替换旧的值.等价于 = 操作
	.	另外:如果title节点有子节点,则会清空所有子节点..
	*/
	bool UpdateValue(cchar *title, cchar *val);//字符串
	bool UpdateValue(cchar *title, const std::string &val);//字符串
	bool UpdateValue(cchar *title, int val);//整数
	bool UpdateValue(cchar *title, const unsigned int &val);//32位无符号
	bool UpdateValue(cchar *title, bool val);//true/false
	bool UpdateValue(cchar *title, const time_t &t);//time_t
	bool UpdateValue(cchar *title, const unsigned char *val, int bufflen , int fmt = 1); // 16进制字符串
	bool UpdateValue(cchar *title, const float &val);//
	bool UpdateValue(cchar *title, const double &val);//

	// 增加注释
	// 同一路径如果调用多次,注释将会叠加.
	bool AddComments(cchar *path, cchar *val);

	/*	添加一个值.
	.注: 1)允许出现相同的名称.
	.	 2)不对path进行二次解释.path整个字符串将直接作为节点名称进行添加.
	.	 3)添加值不改变当前位置
	.	 4)如果用for,或者需要添加多个相同的值,只能用 SimpleAddXxx()函数
	*/
	bool SimpleAddValue(cchar *n_title, cchar *val);
	bool SimpleAddValue(cchar *n_title, const std::string &val);//字符串
	bool SimpleAddValue(cchar *n_title, int val);//整数
	bool SimpleAddValue(cchar *n_title, const unsigned int &val);//32位无符号
	bool SimpleAddValue(cchar *n_title, bool val);//true/false
	bool SimpleAddValue(cchar *n_title, const time_t &t);//time_t
	bool SimpleAddValue(cchar *n_title, const unsigned char *val, int bufflen, int fmt = 1); // 16进制字符串
	bool SimpleAddValue(cchar *n_title, const float &val);//
	bool SimpleAddValue(cchar *n_title, const double &val);//

	/* 删除一个值.
	*/
	bool EraseValue(cchar *n_title);

	bool SaveToFile(cchar *szFile);

protected:
	TiXmlElement *_create_mult_node(cchar *path);//创建节点(如果不存在的话)
	TiXmlElement *_find_and_create_node(TiXmlElement *parent, cchar *title, int index); //查找或创建一个节点(如果不存在的话)

};



//编码转化
//int Utf8ToGb2312(char *sOut,int iMaxOutLen,const char *sIn,int iInLen);
//int Gb2312ToUtf8(char *sOut,int iMaxOutLen,const char *sIn,int iInLen);





#endif
