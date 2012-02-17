
#ifndef AR_FIXEDLOCATIONRECORD_H
#define AR_FIXEDLOCATIONRECORD_H

#include <string>
#include <map>
#include "Record.h"
#include "MyCDRBASE.h"

//struct FixedLocationRecord_Val {
//	int seq;
//	int start;
//	int end;
//	char * val_ptr;
//	FixedLocationRecord_Val():start(-1),end(-1),val_ptr(0){}
//};
//
//struct REC_POS_VAL{
//char *  val;
//int seq;
//int pos;
//int len;
//REC_POS_VAL():val(0),pos(-1),len(-1){}
//};

//class FixedLocationRecord : public Record
//{
//	bool isInitialized;
//	char * _recordVal;
//	typedef std::map<std::string, FixedLocationRecord_Val> _CVAL_MAP ;
////	int _recLen;
//public:
//	char LINE_SEPERATOR ; 
//	int NUMBER_OF_FIELDS; 
//
//	_CVAL_MAP	_value;
//	FixedLocationRecord(void);
//	~FixedLocationRecord(void);
//	virtual const char *GetValue(const std::string&  key);
//	virtual bool  GetValueStr(const std::string&  key, std::string& );
//	virtual void dump(void );
//	void setFixedLocationRecord_Val(std::string & NAME , FixedLocationRecord_Val & t) ;
//
//};

#endif