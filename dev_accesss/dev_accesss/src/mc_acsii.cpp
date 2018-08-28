#include "SocketAPI\SocketAPI.h"
#include "mc_acsii.h"


McAcsii::McAcsii(const char * ip, uint16_t port)
	: m_ipaddr(ip)
	, m_port(port)
	, m_sockfd(-1)
	, m_pPlcWriteCmd(nullptr)
{
}

McAcsii::~McAcsii()
{
	if (m_pPlcWriteCmd)
	{
		delete[] m_pPlcWriteCmd;
		m_pPlcWriteCmd = nullptr;
	}
}

int McAcsii::Init()
{
	m_sockfd = Connect(m_ipaddr.c_str(), m_port, SOCK_STREAM);
	if (m_sockfd < 0)
	{
		return -1;
	}
	SetSocketNBlock(m_sockfd, true);
	return 0;
}

int McAcsii::Read(const char *addr, uint16_t length, string &val)
{
	int ret = BuildReadCmd(addr, length);
	if (ret < 0)
	{
		return ret;
	}

	string str;
	ret = SendAndRecv(m_plcReadCmd, sizeof(m_plcReadCmd), str);
	if (ret < 0)
	{
		return ret;
	}
	ret = ParseReadResult(str, val);
	return ret;
}

int McAcsii::BuildReadCmd(const char *addr, uint16_t length)
{
	int len = AnalysisAddress(addr);
	if (len < 0)
	{
		return -1;
	}

	m_plcReadCmd[0] = 0x35;                                      // 副标题
	m_plcReadCmd[1] = 0x30;
	m_plcReadCmd[2] = 0x30;
	m_plcReadCmd[3] = 0x30;
	m_plcReadCmd[4] = '0'; // BuildBytesFromData(NetworkNumber)[0];                // 网络号
	m_plcReadCmd[5] = '0'; // BuildBytesFromData(NetworkNumber)[1];
	m_plcReadCmd[6] = 0x46;                         // PLC编号
	m_plcReadCmd[7] = 0x46;
	m_plcReadCmd[8] = 0x30;                         // 目标模块IO编号
	m_plcReadCmd[9] = 0x33;
	m_plcReadCmd[10] = 0x46;
	m_plcReadCmd[11] = 0x46;
	m_plcReadCmd[12] = '0'; // BuildBytesFromData(NetworkStationNumber)[0];         // 目标模块站号
	m_plcReadCmd[13] = '0'; // BuildBytesFromData(NetworkStationNumber)[1];
	m_plcReadCmd[14] = 0x30;                         // 请求数据长度
	m_plcReadCmd[15] = 0x30;
	m_plcReadCmd[16] = 0x31;
	m_plcReadCmd[17] = 0x38;
	m_plcReadCmd[18] = 0x30;                         // CPU监视定时器
	m_plcReadCmd[19] = 0x30;
	m_plcReadCmd[20] = 0x31;
	m_plcReadCmd[21] = 0x30;
	m_plcReadCmd[22] = 0x30;                        // 批量读取数据命令
	m_plcReadCmd[23] = 0x34;
	m_plcReadCmd[24] = 0x30;
	m_plcReadCmd[25] = 0x31;
	m_plcReadCmd[26] = 0x30;                         // 以点为单位还是字为单位成批读取
	m_plcReadCmd[27] = 0x30;
	m_plcReadCmd[28] = 0x30;
	m_plcReadCmd[29] = m_McDataTypePtr->GetDataType() == 0 ? 0x30 : 0x31;// analysis.Content1.DataType == 0 ? (byte)0x30 : (byte)0x31;
	m_plcReadCmd[30] = m_McDataTypePtr->GetAsciiCode().c_str()[0] ;// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[0];                          // 软元件类型
	m_plcReadCmd[31] = m_McDataTypePtr->GetAsciiCode().c_str()[1] ;// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[1];


	char buf[8] = { 0 };
	if (m_McDataTypePtr->GetFromBase() == 10)
	{
		snprintf(buf, sizeof(buf), "%06d", len);
	}
	else if (m_McDataTypePtr->GetFromBase() == 16)
	{
		snprintf(buf, sizeof(buf), "%06X", len);
	}

	m_plcReadCmd[32] = buf[0];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[0];                   // 起始地址的地位
	m_plcReadCmd[33] = buf[1];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[1];
	m_plcReadCmd[34] = buf[2];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[2];
	m_plcReadCmd[35] = buf[3];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[3];
	m_plcReadCmd[36] = buf[4];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[4];
	m_plcReadCmd[37] = buf[5];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[5];

	char tmp[8] = {0};
	snprintf(tmp, sizeof(tmp), "%04X", length);
	m_plcReadCmd[38] = tmp[0]; // BuildBytesFromData(length)[0];                                                      // 软元件点数
	m_plcReadCmd[39] = tmp[1]; // BuildBytesFromData(length)[1];
	m_plcReadCmd[40] = tmp[2]; // BuildBytesFromData(length)[2];
	m_plcReadCmd[41] = tmp[3]; // BuildBytesFromData(length)[3];

	return 0;
}

int McAcsii::AnalysisAddress(const char *addr)
{
	int len = -1;
	char type = addr[0];
	string val = ++addr;
	switch (type)
	{
	case 'M':
	case 'm':
	{
		m_McDataTypePtr.reset(new McDataType(0x90, 0x01, "M*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'X':
	case 'x':
	{
		m_McDataTypePtr.reset(new McDataType(0x9C, 0x01, "X*", 16));
		sscanf_s(val.c_str(), "%x", &len);
		break;
	}
	case 'Y':
	case 'y':
	{
		m_McDataTypePtr.reset(new McDataType(0x9D, 0x01, "Y*", 16));
		sscanf_s(val.c_str(), "%x", &len);
		break;
	}
	case 'D':
	case 'd':
	{
		m_McDataTypePtr.reset(new McDataType(0xA8, 0x00, "D*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'W':
	case 'w':
	{
		m_McDataTypePtr.reset(new McDataType(0xB4, 0x00, "W*", 16));
		sscanf_s(val.c_str(), "%x", &len);
		break;
	}
	case 'L':
	case 'l':
	{
		m_McDataTypePtr.reset(new McDataType(0x92, 0x01, "L*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'F':
	case 'f':
	{
		m_McDataTypePtr.reset(new McDataType(0x93, 0x01, "F*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'V':
	case 'v':
	{
		m_McDataTypePtr.reset(new McDataType(0x94, 0x01, "V*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'B':
	case 'b':
	{
		m_McDataTypePtr.reset(new McDataType(0xA0, 0x01, "B*", 16));
		sscanf_s(val.c_str(), "%x", &len);
		break;
	}
	case 'R':
	case 'r':
	{
		m_McDataTypePtr.reset(new McDataType(0xAF, 0x00, "R*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'S':
	case 's':
	{
		m_McDataTypePtr.reset(new McDataType(0x98, 0x01, "S*", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'Z':
	case 'z':
	{
		m_McDataTypePtr.reset(new McDataType(0xCC, 0x00, "Z*", 10));
		len = atoi(val.c_str());
		break;
	}

	case 'T':
	case 't':
	{
		m_McDataTypePtr.reset(new McDataType(0xC2, 0x00, "TN", 10));
		len = atoi(val.c_str());
		break;
	}
	case 'C':
	case 'c':
	{
		m_McDataTypePtr.reset(new McDataType(0xC5, 0x00, "CN", 10));
		len = atoi(val.c_str());
		break;
	}
	default:
		WLogError("输入的类型不支持，请重新输入, addr:%s", addr);
		break;
	}

	return len;
}

int McAcsii::Write(const char *addr, const char *val, string &strRet)
{
	int ret = BuildWriteCmd(addr, val);
	if (ret < 0)
	{
		return ret;
	}
	
	string str;
	ret = SendAndRecv(m_pPlcWriteCmd, m_iWriteLen, str);
	if (ret < 0)
	{
		return ret;
	}

	ret = ParseWriteResult(str, strRet);

	return ret;
}

int McAcsii::BuildWriteCmd(const char *addr, const char *val)
{
	int len = AnalysisAddress(addr);
	if (len < 0)
	{
		return -1;
	}

	int ret = 0;
	if (m_McDataTypePtr->GetDataType() == 0x01)
	{
		int iLen = strlen(val);
		iLen = (iLen % 2) == 0 ? iLen : (iLen + 1);
		char *buff = new char[iLen+1];
		memset(buff, 0, iLen+1);
		for (size_t i = 0; i < iLen; i++)
		{
			if (val[i] == 0x00)
			{
				buff[i] = 0x30;
			}
			else
			{
				buff[i] = 0x31;
			}
		}

		ret = DoBuildWriteCmd(addr, buff);
		delete[] buff;
	}
	else
	{
		int iLen = strlen(val);
		iLen = (iLen % 2) == 0 ? iLen : (iLen + 1);
		char *buff = new char[iLen*2+1];
		memset(buff, 0, iLen*2+1);
		string strVal(val);
		for (int i = 0; i < iLen / 2; i++)
		{
			string strTmp = strVal.substr(i * 2, 2);
			char tmp[8] = {0};
			snprintf(tmp, sizeof(tmp), "%04X", atoi(strTmp.c_str()));
			strcpy(buff + i*4, tmp);
		}
		ret = DoBuildWriteCmd(addr, buff);
		delete[] buff;
	}

	return ret;
}

int McAcsii::DoBuildWriteCmd(const char *addr, const char *val)
{
	int len = AnalysisAddress(addr);
	if (len < 0)
	{
		return -1;
	}
	if (m_pPlcWriteCmd)
	{
		delete[] m_pPlcWriteCmd;
		m_pPlcWriteCmd = nullptr;
	}

	int dataLen = 42 + strlen(val);
	m_iWriteLen = dataLen;
	m_pPlcWriteCmd = new char[dataLen];
	memset(m_pPlcWriteCmd, 0, dataLen);


	m_pPlcWriteCmd[0] = 0x35;                                      // 副标题
	m_pPlcWriteCmd[1] = 0x30;
	m_pPlcWriteCmd[2] = 0x30;
	m_pPlcWriteCmd[3] = 0x30;
	m_pPlcWriteCmd[4] = '0'; // BuildBytesFromData(NetworkNumber)[0];                // 网络号
	m_pPlcWriteCmd[5] = '0'; // BuildBytesFromData(NetworkNumber)[1];
	m_pPlcWriteCmd[6] = 0x46;                         // PLC编号
	m_pPlcWriteCmd[7] = 0x46;
	m_pPlcWriteCmd[8] = 0x30;                         // 目标模块IO编号
	m_pPlcWriteCmd[9] = 0x33;
	m_pPlcWriteCmd[10] = 0x46;
	m_pPlcWriteCmd[11] = 0x46;
	m_pPlcWriteCmd[12] = '0'; // BuildBytesFromData(NetworkStationNumber)[0];         // 目标模块站号
	m_pPlcWriteCmd[13] = '0'; // BuildBytesFromData(NetworkStationNumber)[1];

	char temp[8] = { 0 };
	snprintf(temp, sizeof(temp), "%04X", dataLen-18);
	m_pPlcWriteCmd[14] = temp[0]; // 请求数据长度
	m_pPlcWriteCmd[15] = temp[1];
	m_pPlcWriteCmd[16] = temp[2];
	m_pPlcWriteCmd[17] = temp[3];

	m_pPlcWriteCmd[18] = 0x30; // CPU监视定时器
	m_pPlcWriteCmd[19] = 0x30;
	m_pPlcWriteCmd[20] = 0x31;
	m_pPlcWriteCmd[21] = 0x30;
	m_pPlcWriteCmd[22] = 0x31; // 批量写入的命令
	m_pPlcWriteCmd[23] = 0x34;
	m_pPlcWriteCmd[24] = 0x30;
	m_pPlcWriteCmd[25] = 0x31;
	m_pPlcWriteCmd[26] = 0x30; // 子命令
	m_pPlcWriteCmd[27] = 0x30;
	m_pPlcWriteCmd[28] = 0x30;
	m_pPlcWriteCmd[29] = m_McDataTypePtr->GetDataType() == 0 ? (byte)0x30 : (byte)0x31;
	m_pPlcWriteCmd[30] = m_McDataTypePtr->GetAsciiCode().c_str()[0];// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[0];                          // 软元件类型
	m_pPlcWriteCmd[31] = m_McDataTypePtr->GetAsciiCode().c_str()[1];// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[1];

	char buf[8] = { 0 };
	if (m_McDataTypePtr->GetFromBase() == 10)
	{
		snprintf(buf, sizeof(buf), "%06d", len);
	}
	else if (m_McDataTypePtr->GetFromBase() == 16)
	{
		snprintf(buf, sizeof(buf), "%06X", len);
	}

	m_pPlcWriteCmd[32] = buf[0];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[0];                   // 起始地址的地位
	m_pPlcWriteCmd[33] = buf[1];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[1];
	m_pPlcWriteCmd[34] = buf[2];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[2];
	m_pPlcWriteCmd[35] = buf[3];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[3];
	m_pPlcWriteCmd[36] = buf[4];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[4];
	m_pPlcWriteCmd[37] = buf[5];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[5];

	// 判断是否进行位操作
	if (m_McDataTypePtr->GetDataType() == 1)
	{
		int iLen = strlen(val);
		char tmp[8] = {0};
		snprintf(tmp, sizeof(tmp), "%04X", iLen);
		m_pPlcWriteCmd[38] = tmp[0];                                                      // 软元件点数
		m_pPlcWriteCmd[39] = tmp[1];
		m_pPlcWriteCmd[40] = tmp[2];
		m_pPlcWriteCmd[41] = tmp[3];
	}
	else
	{
		int iLen = strlen(val)/4;
		char tmp[8] = { 0 };
		snprintf(tmp, sizeof(tmp), "%04X", iLen);
		m_pPlcWriteCmd[38] = tmp[0];                                                      // 软元件点数
		m_pPlcWriteCmd[39] = tmp[1];
		m_pPlcWriteCmd[40] = tmp[2];
		m_pPlcWriteCmd[41] = tmp[3];
	}
	strncpy(m_pPlcWriteCmd+42, val, strlen(val));
	return 0;
}

int McAcsii::SendAndRecv(const char *pData, int len, string & val)
{
	WLogInfo("send:%s", pData);
	int ret = SendWithTimeout(m_sockfd, pData, len);
	if (ret != len)
	{
		WLogError("Send failure:ret %d", ret);
		return -1;
	}
	char buf[1024] = {0};
	ret = RecvWithTimeout(m_sockfd, buf, sizeof(buf));
	if (ret <= 0)
	{
		WLogError("Recv failure:ret %d", ret);
		return -1;
	}

	WLogInfo("read:%s", buf);
	val = string(buf);
	return 0;
}

int McAcsii::ParseWriteResult(const string & str, string & val)
{
	string ret = str.substr(18, 4);
	if (atoi(ret.c_str()) != 0)
	{
		return -1;
	}
	return 0;
}

int McAcsii::ParseReadResult(const string & str, string & val)
{
	string ret = str.substr(18, 4);
	if (atoi(ret.c_str()) == 0)
	{
		if (m_McDataTypePtr->GetDataType() == 0x01)
		{
			int iLen = str.length() - 22;
			char *buf = new char[iLen+1];
			memset(buf, 0, iLen+1);
			for (size_t i = 22, j=0; i < str.length(); i++, j++)
			{
				if (str.c_str()[i] == 0x30)
				{
					buf[j] = 0x00;
				}
				else
				{
					buf[j] = 0x01;

				}
			}
			val = str.substr(0, 22);
			val.append(buf);
			delete[] buf;
		}
		else
		{
			int iLen = (str.length()-22)/2;
			char *buf = new char[iLen + 1];
			memset(buf, 0, iLen + 1);
			for (size_t i = 0; i < iLen / 2; i++)
			{
				string tmp = str.substr(i*4+22, 4);
				char val[4] = { 0 };
				snprintf(val, sizeof(val), "%x", atoi(tmp.c_str()));
				strncpy(buf+i*2, val, 2);
			}
			val = string(buf);
			delete[] buf;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}


