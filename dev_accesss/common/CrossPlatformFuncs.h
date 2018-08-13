/*******************************************************************************
 *CrossPlatformFuncs.h
 *******************************************************************************/

#ifndef CROSSPLATFORMFUNCS_H
#define CROSSPLATFORMFUNCS_H

#ifdef WIN32 
	#ifndef _WIN_
		#define _WIN_
	#endif
#endif

#ifdef WIN64
	#ifndef _WIN_
		#define _WIN_
	#endif
#endif

#ifndef SLEEP_S
	#ifdef _WIN_
		#define SLEEP_S(a)  Sleep(a*1000)
	#else
		#define SLEEP_S(a)  sleep(a)
	#endif
#endif

#ifndef SLEEP_MS
	#ifdef _WIN_
		#define SLEEP_MS(a)  Sleep(a)
	#else
		#define SLEEP_MS(a)  usleep(a*1000)
	#endif
#endif

#ifndef SNPRINTF
#ifdef _WIN_
	#define SNPRINTF sprintf_s
#else
	#define SNPRINTF snprintf
#endif
#endif

#ifdef _WIN_
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#endif

//包含一些公共头文件()
#include <stdlib.h>
#include <stdio.h>

#endif

