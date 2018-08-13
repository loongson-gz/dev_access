#ifndef __SQL_BASE_H__
#define __SQL_BASE_H__

class SqlBase {
public:
	SqlBase();
	virtual ~SqlBase();

	virtual int Start() = 0;
	virtual int Stop() = 0;
};

#endif