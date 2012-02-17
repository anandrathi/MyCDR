
#ifndef DBCONFIGSQLLITE_H
#define DBCONFIGSQLLITE_H

#include "DBConfig.h"

class DBConfigSQLLITE : public DBConfig
{
	friend class DBConfig;
public:

protected:
	DBConfigSQLLITE(const std::string & connectString):DBConfig(connectString) {}
	virtual bool Init();
	~DBConfigSQLLITE();
};

#endif
