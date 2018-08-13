//#include "stdafx.h"
#include <time.h>

#include "CSimpleLog.h"

using namespace std;
unsigned char CSimpleLog::g_iNum = 1;
unsigned char CSimpleWrite::g_iNum = 1;

CSimpleLog::CSimpleLog()
{
	m_time[0]=0;
	g_file = NULL;
	m_iId = -1;
	m_iNum = 0;
	m_iFilesize = 0;
	m_isDisableTime = false;
}

CSimpleLog::~CSimpleLog()
{
	//if (m_of.open())
	//{
	//	m_of.close();
	//}
	if (g_file)
	{
		fclose(g_file);
		g_file = NULL;
	}
}

void CSimpleLog::SetPrev(std::string s,std::string fname)
{
	m_prev_str = s;
	m_fname = fname;
}
const char* CSimpleLog::GetBytesStr(std::string &out,const void *bytes,int iLen)
{
	out.clear();
	if (bytes && iLen>0)
	{
		out.reserve(iLen * 3 + 3);
		char buf[16];
		const unsigned char *pByte = (const unsigned char*)bytes;
		for (int i=0;i<iLen;i++)
		{
			sprintf(buf,"%02x ",pByte[i]);
			out += buf;
		}
	}
	return out.c_str();
}
void CSimpleLog::Printf(const char *fmt,...)
{
	va_list va;
	va_start(va,fmt);
	PrintfV(fmt,va);
	va_end(va);
}
bool CSimpleLog::_create_file()
{
	if (!g_file)
	{
		if (m_fname.empty())
		{
			m_fname = "./csimplelog";
		}
		if (m_iId == -1)
		{
			m_iId = g_iNum++;
		}else
		{
			m_iNum++;
		}
		char buf[1024];
		sprintf(buf,"./%s_%08d_%d.log",m_fname.c_str(),m_iId,m_iNum);

		g_file = fopen(buf,"w");//m_of.open(m_fname.c_str());
		if (!g_file)
		{
			return false;
		}
	}
	return true;
}
void CSimpleLog::PrintfV(const char *fmt,void *arg_lst)
{
	CResGuard_SimpleLog::Defend autoLock(m_lock);
	if (!_create_file())
	{
		return ;
	}
	if (!m_isDisableTime)
	{
		print_time();
	}
	//fprintf(g_file,"%s %s | ",GetTime(),m_prev_str.c_str());

	//char buf[2048];

	va_list va = (va_list)arg_lst;
	m_iFilesize += vfprintf(g_file,fmt,va);

	m_iFilesize += fprintf(g_file,"\n");
	fflush(g_file);

	if (m_iFilesize > 50*1024*1024)
	{
		fclose(g_file);
		g_file = NULL;
	}

}


const char* CSimpleLog::GetTime()
{
	time_t now = ::time(NULL);
	struct tm t = *localtime(&now);
	sprintf(m_time,"%04d-%02d-%02d %02d:%02d:%02d",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
	return m_time;
}

void CSimpleLog::print_time()
{
	if (!_create_file())
	{
		return ;
	}
	time_t now = ::time(NULL);
	if (now == m_lastTime)
	{
		return;
	}
	m_lastTime = now;
	struct tm t = *localtime(&now);
	m_iFilesize += fprintf(g_file,"--- %04d-%02d-%02d %02d:%02d:%02d :\n",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

}




//-----------
CSimpleWrite::CSimpleWrite()
{
	g_file = NULL;
}
CSimpleWrite::~CSimpleWrite()
{
	if (g_file)
	{
		fclose(g_file);
		g_file = NULL;
	}
}

void CSimpleWrite::SetPrev(std::string fname /* = */ ,std::string ext)
{
	m_fname = fname;
	m_ext= ext;
}
void CSimpleWrite::write(const void* data,int iLen)
{
	if (!g_file)
	{
		if (m_fname.empty())
		{
			m_fname = "./csimplewrite";
		}
		if (m_ext.empty())
		{
			m_ext = "binary";
		}
		int i = g_iNum++;
		char buf[1024];
		sprintf(buf,"./%s_%08d.%s",m_fname.c_str(),i,m_ext.c_str());

		g_file = fopen(buf,"wb");//m_of.open(m_fname.c_str());
		if (!g_file)
		{
			return;
		}
	}

	fwrite((char*)data,iLen,1,g_file);
	fflush(g_file);
}


#ifdef _CSIMPLE_LOG_STACK
void CStackLog::OnOutput(LPCSTR szText)
{
	Printf("%s",szText);
}
void CStackLog::PrintStack()
{
	CResGuard_SimpleLog::Defend autoLock(m_lock);
	print_time();
	m_isDisableTime = true;
	Printf("------- stack begin -----------");
	ShowCallstack();
	Printf("------- stack end -----------");
	m_isDisableTime = false;
}
void CStackLog::PrintStack(const char*fmt,...)
{
	CResGuard_SimpleLog::Defend autoLock(m_lock);
	print_time();
	m_isDisableTime = true;
	//Printf("------- stack begin -----------");
	va_list av;
	va_start(av,fmt);
	PrintfV(fmt,av);
	va_end(av);
	ShowCallstack();
	Printf("------- stack end -----------");
	m_isDisableTime = false;
}
void CStackLog::PrintStackV(const char*fmt,void *arg_lst)
{
	CResGuard_SimpleLog::Defend autoLock(m_lock);
	m_isDisableTime = true;
	print_time();
	PrintfV(fmt,arg_lst);
	ShowCallstack();
	Printf("------- stack end -----------");
	m_isDisableTime = false;
}

#endif
