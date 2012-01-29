#pragma once

#ifndef _AR_RECORD_H
#define  _AR_RECORD_H

#include "CDRTypes.h"
#include <string>
#include <vector>
#include "boost/multi_array.hpp"
#include "MyCDRBASE.h"


struct RecordDataName
{
	//typedef std::vector<char> DATACELL;
	std::string cell; 
	std::string field;
	unsigned long len;
	void * _recposval;
};

struct RecordData_For_FixSize
{
	typedef boost::multi_array<char, 3> RECORD_ARRAY_TYPE;
	typedef RECORD_ARRAY_TYPE::index index;
	RECORD_ARRAY_TYPE *_arr;
	RecordData_For_FixSize(){}
	bool Init(int fieldCount , int fieldLen , int num_of_records);
};


struct RecordDetails
{
	std::string	PATH;
	std::string	FILE_EXP;
	std::string	ELEMENT_NAME;
	std::string	SCRIPT_PATH;
	std::string	SCRIPT;

};

class Record : public MyCDRBASE
{
public:
	//typedef std::vector<RecordData> ARRAY_RECORDDATA;
	typedef std::vector<RecordDataName> RECORDDATANAME;

protected:
	RECKEYS _keys;
	RecordDetails _RecordDetails;
	//ARRAY_RECORDDATA _RecordData ;
	int _recLen;
	unsigned int _LogLevel;
public:
	RECORDDATANAME _recordDataName;
	int BUFFER_SIZE;

	unsigned int getrecLen(void){ return _recLen;}
	unsigned int getLogLevel(void){ return _LogLevel;}
	void setLogLevel(unsigned int level){ _LogLevel= level;}
	
	Record(void);
	~Record(void);
	const RECKEYS &  GetKeys() ;
	virtual bool GetKey(std::string) const;
	virtual bool AddKey(std::string k, std::string d=""); 

	std::string & operator[](const std::string key) ;
	bool operator[](const std::string key) const;
	virtual const char * GetValue(const std::string&  key)=0;//{return 0;};
	virtual bool  GetValueStr(const std::string&  key, std::string& )=0;//{ return  std::string(""); };
};

#endif