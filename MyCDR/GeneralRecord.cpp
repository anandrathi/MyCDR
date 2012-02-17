#include "GeneralRecord.h"
#include "ace/Log_Msg.h"
#include "sqlite3.h"
#include "ace/Log_Msg.h"
//#include "cpptcl.h"
#include "sqlite3.h"


GeneralRecord::GeneralRecord(void)
{
}


GeneralRecord::~GeneralRecord(void)
{
}

void GeneralRecord::push_back(const std::string&  key, std::string&  type, int len)
{
	GENREC_INFO gi;
	if( type == "T" || type == "t" )
		gi.type=GENREC_INFO::RECTYPE::TEXT;
	else if( type == "N" || type == "n" )
		gi.type=GENREC_INFO::RECTYPE::NUMBER;
	else if( type == "D"|| type == "d" )
		gi.type=GENREC_INFO::RECTYPE::DATE;

	gi.len = len;
	_value[key]=gi;
}


const char * GeneralRecord::GetValue(const std::string&  k)
{
	ACE_TRACE("GeneralRecord::GetValue");
	GVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(k);
	if(it == _value.end()) {
		return 0;
	}
	return it->second.val;
}

bool GeneralRecord::GetValueStr(const std::string&  key, std::string& val)
{
	ACE_TRACE("GeneralRecord::GetValueStr");
	GVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(key);
	if(it == _value.end()) {
		return false;
	}
	val.clear();
	val.append(it->second.val);
	return true;
}

void GeneralRecord::setSQLLITE(void *ptcl)
{



}


void GeneralRecord::dump(void)
{
	//ACE_ERROR ((LM_DEBUG, "(%t) GeneralRecord NUMBER_OF_FIELDS=%d : MAX_FIELD_LEN=%d \n",	NUMBER_OF_FIELDS, 	MAX_FIELD_LEN));
	GVAL_MAP::iterator i = _value.begin();
	for(;i!=_value.end() ; i++)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) GeneralRecord Name = %s  pos = %d len = %d type=%d \n", i->first.c_str() , i->second.len, i->second.type));
	}
	
}
