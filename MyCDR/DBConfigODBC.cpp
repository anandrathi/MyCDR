#include "DBConfigODBC.h"
#include "soci-odbc.h"



DBConfigODBC::~DBConfigODBC(void)
{
	delete this->m_sql;
	delete this->m_backEnd ;
}


bool DBConfigODBC::Init()
{
	try {
		this->m_backEnd = const_cast<soci::backend_factory *>( soci::factory_odbc() );
		this->m_sql = new soci::session(*m_backEnd, m_connectString);
	}
	catch(...)
	{
		return false;
	}
	return DBConfig::Init();
}


