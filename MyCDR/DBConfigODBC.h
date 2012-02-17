#ifndef DBCONFIGODBC_H
#define DBCONFIGODBC_H
#include "dbconfig.h"

class DBConfigODBC 	: public DBConfig
{
	friend class DBConfig;

public:

protected:
	DBConfigODBC(const std::string& connectString):DBConfig(connectString) {}
	virtual bool Init();
	~DBConfigODBC();
};


#endif