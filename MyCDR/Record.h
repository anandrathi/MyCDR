#pragma once

#ifndef _AR_RECORD_H
#define  _AR_RECORD_H

#include "CDRTypes.h"
#include <string>
#include <vector>
#include "boost/multi_array.hpp"
#include "MyCDRBASE.h"

struct RecordData
{
	//typedef std::vector<char> DATACELL;
	std::string cell; 
	std::string field;
	unsigned long len;
	void * _recposval;
};

//struct RecordData_For_FixSize
//{
//	typedef boost::multi_array<char, 3> RECORD_ARRAY_TYPE;
//	typedef RECORD_ARRAY_TYPE::index index;
//	RECORD_ARRAY_TYPE *_arr;
//	RecordData_For_FixSize(){}
//	bool Init(int fieldCount , int fieldLen , int num_of_records);
//};

struct Fielddetails
{ 

	Fielddetails():POS(-1), LEN(-1), START(-1),  END(-1) , mapto_fieldid(-1), val_ptr(0){}
	std::string NAME;
	int POS;
	int LEN;
	int START; 
	int END;
	int mapto_fieldid;
	char * val_ptr;
	int seq;
	RecordData _RecordData;
} ; 


struct RecordDetails
{

	typedef std::vector<Fielddetails> FIELDDETAILS;

	std::string PATH ;
	std::string FILE_EXP;
	std::string ELEMENT_NAME ;
	std::string TYPE ;
	std::string SCRIPT ;

	int  LOGLEVEL ;
	int BUFFER_SIZE;
	int FIELDSEPERATOR ;
	int LINE_SEPERATOR ;
	int FIELDS ;
	int MAX_FIELD_LEN ;
	int NUMBER_OF_FIELDS ;

	void AddFIXField( std::string & NAME , int START , int END , int iseq);
	void AddCVSField( std::string & NAME , int POS, int LEN , int iseq);
	void dump(std::ostream & os);
	FIELDDETAILS * GetFieldDetails() {return &_Fielddetails;}
protected:
		FIELDDETAILS _Fielddetails; 

};

//class Record : public MyCDRBASE
//{
//public:
//	//typedef std::vector<RecordData> ARRAY_RECORDDATA;
//	typedef std::vector<RecordDataName> RECORDDATANAME;
//
//protected:
//	RECKEYS _keys;
//	//RecordDetails _RecordDetails;
//	//ARRAY_RECORDDATA _RecordData ;
//	INELEMENT _inelement;
//	int _recLen;
//	unsigned int _LogLevel;
//public:
//	RECORDDATANAME _recordDataName;
//	int BUFFER_SIZE;
//
//	unsigned int getrecLen(void){ return _recLen; }
//	unsigned int getLogLevel(void){ return _LogLevel;}
//	void setLogLevel(unsigned int level){ _LogLevel= level;}
//	
//	Record(void);
//	~Record(void);
//	const RECKEYS &  GetKeys() ;
//	virtual bool GetKey(std::string) const;
//	virtual bool AddKey(std::string k, std::string d=""); 
//
//	std::string & operator[](const std::string key) ;
//	bool operator[](const std::string key) const;
//	virtual const char * GetValue(const std::string&  key)=0;//{return 0;};
//	virtual bool  GetValueStr(const std::string&  key, std::string& )=0;//{ return  std::string(""); };
//};

#endif