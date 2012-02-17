#include "DBConfigMySQL.h"
#include "ace/Log_Msg.h"

#include "soci.h"
#include "soci-mysql.h"



DBConfigMySQL::DBConfigMySQL(const std::string& connectString):DBConfig(connectString) 
{
	bool isInit = Init();
	if(isInit==false)
	{
		throw "DBConfigMySQL construction fail";
	}
}


DBConfigMySQL::~DBConfigMySQL(void)
{
	delete this->m_sql;
	delete this->m_backEnd ;
}


bool DBConfigMySQL::Init()
{
	try {
		this->m_backEnd = const_cast<soci::backend_factory *>( soci::factory_mysql() );
		this->m_sql = new soci::session(*m_backEnd, m_connectString);
	}
	catch(...)
	{
		ACE_ERROR ((LM_ERROR, "(%t) DBConfigMySQL::Init() failed  conn str = %s \n", m_connectString.c_str() ));
		return false;
	}
	return DBConfig::Init();
}