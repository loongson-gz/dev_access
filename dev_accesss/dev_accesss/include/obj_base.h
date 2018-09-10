#ifndef __OBJ_BASE_H__
#define __OBJ_BASE_H__

#include "dev_access_type.h"
#include <string>

class ObjBase {
public:
	ObjBase() {}
	~ObjBase() {}

	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int GetBaseType() = 0;

	/*
	@breif; Set  接口
	@param[in]: key 命令字 如 "control"
	@param[in]: val	参数值
	@return: 0: 执行成功 1: 执行失败
	*/
	virtual int Set(const char *key, const char *val) { return 0; }

	/*
	@breif; Get  接口
	@param[in]: key 命令字 如 "name"
	@param[out]: ret　返回的内容
	@return: 0: 执行成功 1: 执行失败
	*/
	virtual int Get(const char *key,  char *&val) { return 0; }
	virtual std::string GetUrl()
	{
		return m_url;
	}

public:
	std::string m_url;
};

#endif // !__PLC_BASE_H__
