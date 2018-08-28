#include "SocketAPI\SocketAPI.h"
#include "mc_acsii.h"


McAcsii::McAcsii(const char * ip, uint16_t port)
	: m_ipaddr(ip)
	, m_port(port)
	, m_sockfd(-1)
{
}

McAcsii::~McAcsii()
{
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

int McAcsii::Read(const char *addr, uint16_t length, string & val)
{
	int ret = GenerateReadCmd(addr, length);
	if (ret < 0)
	{
		return ret;
	}

	ret = SendAndRecv(val);
	return 0;
}

int McAcsii::Write(const char * addr, const char * val)
{
	return 0;
}

int McAcsii::AnalysisAddress(const char * addr)
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
		WLogError("输入的类型不支持，请重新输入");
		break;
	}

	return len;
}

int McAcsii::GenerateReadCmd(const char *addr, uint16_t length)
{
	int len = AnalysisAddress(addr);
	if (len < 0)
	{
		return -1;
	}

	m_plccmd[0] = 0x35;                                      // 副标题
	m_plccmd[1] = 0x30;
	m_plccmd[2] = 0x30;
	m_plccmd[3] = 0x30;
	m_plccmd[4] = '0'; // BuildBytesFromData(NetworkNumber)[0];                // 网络号
	m_plccmd[5] = '0'; // BuildBytesFromData(NetworkNumber)[1];
	m_plccmd[6] = 0x46;                         // PLC编号
	m_plccmd[7] = 0x46;
	m_plccmd[8] = 0x30;                         // 目标模块IO编号
	m_plccmd[9] = 0x33;
	m_plccmd[10] = 0x46;
	m_plccmd[11] = 0x46;
	m_plccmd[12] = '0'; // BuildBytesFromData(NetworkStationNumber)[0];         // 目标模块站号
	m_plccmd[13] = '0'; // BuildBytesFromData(NetworkStationNumber)[1];
	m_plccmd[14] = 0x30;                         // 请求数据长度
	m_plccmd[15] = 0x30;
	m_plccmd[16] = 0x31;
	m_plccmd[17] = 0x38;
	m_plccmd[18] = 0x30;                         // CPU监视定时器
	m_plccmd[19] = 0x30;
	m_plccmd[20] = 0x31;
	m_plccmd[21] = 0x30;
	m_plccmd[22] = 0x30;                        // 批量读取数据命令
	m_plccmd[23] = 0x34;
	m_plccmd[24] = 0x30;
	m_plccmd[25] = 0x31;
	m_plccmd[26] = 0x30;                         // 以点为单位还是字为单位成批读取
	m_plccmd[27] = 0x30;
	m_plccmd[28] = 0x30;
	m_plccmd[29] = m_McDataTypePtr->GetDataType() == 0 ? 0x30 : 0x31;// analysis.Content1.DataType == 0 ? (byte)0x30 : (byte)0x31;
	m_plccmd[30] = m_McDataTypePtr->GetAsciiCode().c_str()[0] ;// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[0];                          // 软元件类型
	m_plccmd[31] = m_McDataTypePtr->GetAsciiCode().c_str()[1] ;// Encoding.ASCII.GetBytes(analysis.Content1.AsciiCode)[1];


	char buf[8] = { 0 };
	if (m_McDataTypePtr->GetFromBase() == 10)
	{
		snprintf(buf, sizeof(buf), "%06d", len);
	}
	else if (m_McDataTypePtr->GetFromBase() == 16)
	{
		snprintf(buf, sizeof(buf), "%06X", len);
	}

	m_plccmd[32] = buf[0];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[0];                   // 起始地址的地位
	m_plccmd[33] = buf[1];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[1];
	m_plccmd[34] = buf[2];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[2];
	m_plccmd[35] = buf[3];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[3];
	m_plccmd[36] = buf[4];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[4];
	m_plccmd[37] = buf[5];// BuildBytesFromAddress(analysis.Content2, analysis.Content1)[5];

	char tmp[8] = {0};
	snprintf(tmp, sizeof(tmp), "%04X", length);
	m_plccmd[38] = tmp[0]; // BuildBytesFromData(length)[0];                                                      // 软元件点数
	m_plccmd[39] = tmp[1]; // BuildBytesFromData(length)[1];
	m_plccmd[40] = tmp[2]; // BuildBytesFromData(length)[2];
	m_plccmd[41] = tmp[3]; // BuildBytesFromData(length)[3];

	return 0;
}

int McAcsii::SendAndRecv(string & val)
{
	int ret = SendWithTimeout(m_sockfd, m_plccmd, 42);
	if (ret != 42)
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

	val = string(buf);
	return 0;
}


