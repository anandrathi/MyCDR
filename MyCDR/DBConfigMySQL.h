#ifndef DBCONFIGMYSQL_H
#define DBCONFIGMYSQL_H

#include "DBConfig.h"

class DBConfigMySQL : public DBConfig
{
		friend class DBConfig;

public:
	DBConfigMySQL(const std::string& connectString);
	virtual bool Init();
	~DBConfigMySQL();
};

#endif