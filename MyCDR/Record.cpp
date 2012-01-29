#include "Record.h"
#include "CDRTypes.h"

#include "ace/Log_Msg.h"
Record::Record(void)
{
}


Record::~Record(void)
{
}


const RECKEYS &  Record::GetKeys()
{
	return this->_keys;
}

bool Record::AddKey(std::string k, std::string d) 
{
	ACE_TRACE("Record::AddKey");
	RECKEYS::iterator it = _keys.end();
	it = _keys.find(k);
	if(it == _keys.end()) {
		_keys[k]=d;
		return true;
	}
	return false;
}

bool Record::GetKey(std::string k)  const 
{
	ACE_TRACE("Record::GetKey");
	RECKEYS::const_iterator it = this->_keys.end();
	it = _keys.find(k);
	if(it == _keys.end()) {
		return false;
	}
	return true;
}

std::string & Record::operator[](const std::string  k) 
{
	ACE_TRACE("Record::operator[]");
	RECKEYS::iterator it = _keys.end();
	it = _keys.find(k);
	if(it == _keys.end()) {
		_keys[k];
		it = _keys.find(k);
		return it->second;
	}
	return it->second;
}

bool Record::operator[](const std::string k) const
{ 
	return GetKey(k); 
}

bool RecordData_For_FixSize::Init(int fieldCount , int fieldLen , int num_of_records)
{
	ACE_TRACE("RecordData_For_FixSize::Init");
	_arr = new RECORD_ARRAY_TYPE(boost::extents[num_of_records][fieldCount][fieldLen]);
	return true;
}

