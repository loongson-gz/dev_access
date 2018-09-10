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
	@breif; Set  �ӿ�
	@param[in]: key ������ �� "control"
	@param[in]: val	����ֵ
	@return: 0: ִ�гɹ� 1: ִ��ʧ��
	*/
	virtual int Set(const char *key, const char *val) { return 0; }

	/*
	@breif; Get  �ӿ�
	@param[in]: key ������ �� "name"
	@param[out]: ret�����ص�����
	@return: 0: ִ�гɹ� 1: ִ��ʧ��
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
