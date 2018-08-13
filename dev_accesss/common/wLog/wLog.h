#ifndef __W_LOG_H__
#define __W_LOG_H__

#define MAX_EVENT_TITLE 32
#define MAX_EVENT_TYPE  4

#define LOGTYPE_INFO    0
#define LOGTYPE_WARN    1
#define LOGTYPE_ERROR   2
#define LOGTYPE_DEBUG   3

#define DEFAULT_MIN_LOGFILE_SIZE     100  //默认最小文件个数
#define DEFAULT_MAX_LOGFILE_SIZE     1000 //默认文件个数
#define DEFAULT_DISC_RESERVE_MBYTES	 512  //默认磁盘保留空间
#define DEFAULT_FILE_SIZE (1024*5120)

#define CLEAN_TIME_INTERVAL		(1000*60*1) // 清理时间间隔，毫秒

#ifdef WIN32
typedef __int64 INT64;
#ifdef _DEBUG
#define __debug__
#endif
#else
typedef long long INT64;
#endif

#define WLogInit(dir, filename, minfiles, maxfiles, reserveMB, bLog)   \
	__W_LOG_PLAY::Init(dir, filename, (minfiles), (maxfiles), (reserveMB), (bLog));

#define WLogInfo(fmt,...)	\
	__W_LOG_PLAY::WriteLog(LOGTYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
//	__W_LOG_PLAY::WriteLog(LOGTYPE_INFO, __FUNCTION__, __LINE__,__FILE__ "[L%d]:" fmt,__LINE__, ##__VA_ARGS__);
//__W_LOG_PLAY::WriteLog(LOGTYPE_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define WLogWarn(fmt,...)	\
	__W_LOG_PLAY::WriteLog(LOGTYPE_WARN, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define WLogError(fmt,...)	\
	__W_LOG_PLAY::WriteLog(LOGTYPE_ERROR, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define WLogDebug(fmt,...)	\
	__W_LOG_PLAY::WriteLog(LOGTYPE_DEBUG, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#ifdef WIN32	
#define plat_snprintf(str,size,format,...) \
	_snprintf_s(str, size, _TRUNCATE, format, ##__VA_ARGS__); 
#else	
#define plat_snprintf(str,size,format,...) \
	snprintf(str, size, format, ##__VA_ARGS__); 
#endif

#define LOG_ADJUST_32BITS(x) (((x)+3) / 4 * 4)

#include <string>
#ifndef WIN32
#include <stdarg.h>
#endif

#include "../Locker/internallock.h"

namespace __W_LOG_PLAY
{
	void Init(const char *pDir,const char *pFileName, int iMinFiles, int iMaxFiles, int iReserveMBytes, bool bLog);
	void WriteLog(int iType, const char *pFun, int line, const char *fmt, ...);	
	void vWriteLog(int iType,const char *pFun, int line, const char *fmt, void *_va_);

	int GetFormatLen(const char* fmt, va_list argList);

	//将十六进制数据转化为字符串.
	//返回: 字符串的长度.pOutStr保存转换后的字符串
	//每一个十六进制字符占3个字符,外部需保证 pOutStr 的大小至少是 iLen*3+2;
	int HexArrayToString(char *pOutStr,void *hex_array,int iLen);
	int HexArrayToString(std::string &outstr,void *hex_array,int iLen);

	//打印系统信息(cpu/内存占用等)
	int PrintfSystemInfo();

	//获取写缓冲区锁.
	//用于需要连续输出数据的情况.一般情况请不要使用.
	CResGuard &GetLock();
};

#endif

