#include "DBConfigSQLLITE.h"



bool DBConfigSQLLITE::Init()
{
	try {
	this->m_backEnd = const_cast<soci::backend_factory *>(soci::factory_sqlite3());
	this->m_sql = new soci::session(*m_backEnd, m_connectString);
	}
	catch(...)
	{
		return false;
	}
	return DBConfig::Init();
}