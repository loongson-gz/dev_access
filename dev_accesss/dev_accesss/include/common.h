#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>

#include "wLog\wLog.h"
#include "dev_access_type.h"
#include "error_code.h"

using namespace std;

#define DEBUG_WHERE WLogInfo("%s:%d %s", __FILE__, __LINE__, __FUNCTION__)

#define  ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#endif