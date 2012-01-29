
#include "FixedLocationRecord.h"
#include "ace/Log_Msg.h"


FixedLocationRecord::FixedLocationRecord(void)
{
	ACE_TRACE("FixedLocationRecord::FixedLocationRecord");
	isInitialized = false;
	FixedLocationRecord_Val f;
	_CVAL_MAP::iterator it =  _value.begin();
	for(; it!=_value.end();it++)
	{
		_value[it->first]=f;
	}
	_recLen=0;
}

FixedLocationRecord::~FixedLocationRecord(void)
{
}

const char * FixedLocationRecord::GetValue(const std::string&  k)
{
	ACE_TRACE("FixedLocationRecord::GetValue");

	if(!isInitialized)
		return 0;
	_CVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(k);
	if(it == _value.end()) {
		return 0;
	}
	return it->second.val_ptr;
}

 bool FixedLocationRecord::GetValueStr(const std::string&  k, std::string& val)
{
	ACE_TRACE("FixedLocationRecord::GetValueStr");
	if(!isInitialized)
		return false;

	_CVAL_MAP::const_iterator it = this->_value.end();
	it = _value.find(k);
	if(it == _value.end()) {
		return false;
	}
	val.clear() ;
	val.append((const char *)it->second.val_ptr);
	return true;
}


 void FixedLocationRecord::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) FixedLocationRecord LINE_SEPERATOR=%c: NUMBER_OF_FIELDS=%d :  \n",	
		(char)LINE_SEPERATOR , 
		NUMBER_OF_FIELDS 
		));
	_CVAL_MAP::iterator i = _value.begin();
	for(;i!=_value.end() ; i++)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) FixedLocationRecord Name = %s  start = %d end = %d \n", i->first.c_str() , i->second.start, i->second.end));
	}
	ACE_DEBUG((LM_DEBUG, "(%t) BUFFER_SIZE= %d \n", BUFFER_SIZE));

}


void FixedLocationRecord::setFixedLocationRecord_Val(std::string & NAME , FixedLocationRecord_Val & t) 
{  
	ACE_TRACE("FixedLocationRecord::setFixedLocationRecord_Val");
	if(this->_recordDataName.size() < t.seq)
	{
		this->_recordDataName.resize(t.seq);
	}
	_value[NAME] = t;
	RecordDataName rdt;
	int len = (t.end - t.start) + 1;
	_recLen+=len;
	ACE_DEBUG((LM_DEBUG, "(%t) _recLen= %d \n", _recLen));
	rdt.len = len;
	if(rdt.len<60)
		rdt.cell.resize(60+1,0);
	else
		rdt.cell.resize(rdt.len+1,0);
	rdt.field=NAME;
	_CVAL_MAP::iterator it = _value.find(NAME);
	if(it!=_value.end())
	{
		rdt._recposval = &(it->second);
	}
	this->_recordDataName[t.seq-1] = rdt;
}   
