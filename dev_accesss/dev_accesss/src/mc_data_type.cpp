#include "mc_data_type.h"

McDataType::McDataType(char code, char type, const string & asciiCode, int fromBase)
	: m_code(code)
	, m_type(type)
	, m_asciiCode(asciiCode)
	, m_fromBase(fromBase)
{
}

McDataType::~McDataType()
{
}
