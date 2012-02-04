#include "DBConfig.h"
#include "soci-sqlite3.h"

class DBConfigSQLLITE : public DBConfig
{
public:

protected:
	DBConfigSQLLITE(std::string connectString):DBConfig(connectString) {}
	virtual bool Init();
	~DBConfigSQLLITE();
};