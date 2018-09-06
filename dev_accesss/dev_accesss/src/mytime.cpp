#include "mytime.h"

time_t MyTime::GetTimestmap()
{
	time_t now = time(0);
	m_now = now;
	return now;
}


std::string MyTime::GetTimeString(time_t t)
{
	tm *p = localtime(&t);
	char buf[128] = {0};
	snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
		1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	std::string str(buf);
	return str;
}

std::string MyTime::GetTimeString1(time_t t)
{
	tm *p = localtime(&t);
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), "%04d%02d%02d%02d%02d%02d",
		1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	std::string str(buf);
	return str;
}