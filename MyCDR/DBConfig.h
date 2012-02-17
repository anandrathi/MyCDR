#ifndef DBCONFIG_H
#define DBCONFIG_H

#include "soci.h"
#include "soci-sqlite3.h"



class DBConfig
{
public:
	enum EBACKEND {ODBC , POSTGRESQL, ORACLE , MYSQL, SQLITE } ;
	typedef std::map<std::string , DBConfig* > DBMAP;
	DBConfig(const std::string & connectString):m_connectString(connectString) {}
	virtual bool Init()=0;
	virtual ~DBConfig();

	static DBConfig* Create( const std::string& key , EBACKEND eb, const std::string& connstr );
	static DBConfig* Get( const std::string& key ) ;
	static DBConfig* Create(const std::string &key , const std::string &dbtype , const std::string &connstr );
	soci::session * getsql() {return m_sql;}

protected:
	static DBMAP _dbMap;
	const std::string m_connectString;
	soci::backend_factory *m_backEnd ;//= *soci::factory_sqlite3();
	soci::session *m_sql;
private:

};

#endif