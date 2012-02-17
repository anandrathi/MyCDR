
#include "DBConfig.h"
#include "DBConfigSQLLITE.h"
#include "DBConfigMySQL.h"
#include "DBConfigODBC.h"

DBConfig::DBMAP DBConfig::_dbMap;
bool DBConfig::Init()
{
	return true;
}

DBConfig::~DBConfig()
{
	delete m_backEnd ;
	delete m_sql;
}

DBConfig* DBConfig::Get( const std::string& key ) 
{ 
	DBMAP::iterator it = _dbMap.find(key) ;
	if( it == _dbMap.end() )
	{
		return 0;
	}
	else 
		return it->second;
}

DBConfig* DBConfig::Create(const std::string &key , const std::string &dbtype , const std::string &connstr )
{
	//ODBC , POSTGRESQL, ORACLE , MYSQL, SQLITE 
	if(dbtype=="MYSQL")
	{
		return Create(key , MYSQL, connstr );
	}
	else if(dbtype=="ODBC")
	{
		return Create(key , ODBC , connstr );
	}
	else if(dbtype=="SQLITE")
	{
		return Create(key , SQLITE , connstr );
	}
	else if(dbtype=="ORACLE")
	{
		return 0;
	}
	else if(dbtype=="POSTGRESQL")
	{
		return 0;
	}
}

DBConfig* DBConfig::Create(const std::string &key , EBACKEND be , const std::string &connstr )
{
	DBConfig* tDBConfig = Get( key ) ;
	if(tDBConfig)
		return tDBConfig;

	switch(be)
	{
	case MYSQL:
		tDBConfig =  new DBConfigMySQL(connstr);
		break;
	case SQLITE:
		tDBConfig =  new DBConfigSQLLITE(connstr);
		break;
	case ODBC:
		tDBConfig =  new DBConfigODBC(connstr);
		break;
	case POSTGRESQL:
	case ORACLE: 
	default:
		break;
	};

	if(tDBConfig)
	{
		_dbMap[key] =  tDBConfig;
	}
	return tDBConfig;
}

