#include "CharRecord.h"
#include "CDRTypes.h"
#include "ace/Log_Msg.h"


CharRecord::CharRecord(void)
{
	ACE_TRACE("CharRecord::CharRecord");
	RECKEYS::iterator it =  _keys.begin();
	for(; it!=_keys.end();it++)
	{
		REC_POS_VAL t;
		_value[it->first]=t;
	}
}


CharRecord::~CharRecord(void)
{
	ACE_TRACE("CharRecord::~CharRecord");
}

const char * CharRecord::GetValue(const std::string&  k)
{
	ACE_TRACE("CharRecord::GetValue");
	_CVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(k);
	if(it == _value.end()) {
		return 0;
	}
	return it->second.val;
}

bool CharRecord::GetValueStr(const std::string&  key, std::string& val)
{
	ACE_TRACE("CharRecord::GetValueStr");
	_CVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(key);
	if(it == _value.end()) {
		return false;
	}
	val.clear();
	val.append(it->second.val);
	return true;
}

void CharRecord::setREC_POS_VAL(std::string & NAME , REC_POS_VAL & t) 
{  
	ACE_TRACE("CharRecord::setREC_POS_VAL");
	if(this->_recordDataName.size() < t.seq)
	{
		this->_recordDataName.resize(t.seq);
	}
	_value[NAME] = t;
	RecordDataName rdt;
	rdt.len = t.len;
	if(t.len<60)
		rdt.cell.resize(60+1,0);
	else
		rdt.cell.resize(t.len+1,0);
	rdt.field=NAME;
	_CVAL_MAP::iterator it = _value.find(NAME);
	if(it!=_value.end())
	{
		rdt._recposval = &(it->second);
	}
	this->_recordDataName[t.seq-1] = rdt;
}   


void CharRecord::dump(void)
{

	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecord FIELDSEPERATOR=%c : LINE_SEPERATOR=%c: NUMBER_OF_FIELDS=%d : MAX_FIELD_LEN=%d \n",	
		FIELDSEPERATOR ,	
		LINE_SEPERATOR , 
		NUMBER_OF_FIELDS, 
		MAX_FIELD_LEN));

	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecord recordDataName size(%d)   _CVAL_MAP size(%d) \n",	
		this->_recordDataName.size() ,
		_value.size() ));

	_CVAL_MAP::iterator i = _value.begin();
	for(;i!=_value.end() ; i++)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) CharRecord Name = %s  pos = %d len = %d \n", i->first.c_str() , i->second.pos, i->second.len));
	}
	ACE_DEBUG((LM_DEBUG, "(%t) BUFFER_SIZE= %d \n", BUFFER_SIZE));
	
}
