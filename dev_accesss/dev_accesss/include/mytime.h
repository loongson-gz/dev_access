#ifndef __MY_TIME_H__
#define __MY_TIME_H__

#include <iostream>
#include <string>
#include <time.h>

class MyTime {
public:
	time_t GetTimestmap();
	std::string GetTimeString(time_t t);
	std::string GetTimeString1(time_t t);

private:
	time_t m_now;

};

#endif