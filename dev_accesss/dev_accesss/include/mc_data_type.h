#ifndef __MC_DATA_TYPE_H__
#define __MC_DATA_TYPE_H__

#include "common.h"


class McDataType
{
public:
	McDataType(char code, char type, const string & asciiCode, int fromBase);
	~McDataType();

	char GetDataCode()
	{
		return m_code;
	}

	char GetDataType()
	{
		return m_type;
	}

	string GetAsciiCode()
	{
		return m_asciiCode;
	}


	int GetFromBase()
	{
		return m_fromBase;
	}

private:
	char m_code;

	/// 数据的类型，0代表按字，1代表按位
	char m_type;

	string m_asciiCode;

	int m_fromBase;

};

typedef shared_ptr<McDataType> McDataTypePtr;


#endif // !__MC_DATA_TYPE_H__
