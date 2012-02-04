
#include "DBConfig.h"

bool DBConfig::Init()
{
	return true;
}
DBConfig::~DBConfig()
{
	delete m_backEnd ;
	delete m_sql;
}
