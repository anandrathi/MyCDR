#pragma once
#ifndef AR_CHARRECORD_H
#define AR_CHARRECORD_H
#include "Record.h"
#include <vector>
#include "ace/Log_Msg.h"
//#include <fstream>

struct REC_POS_VAL{
char *  val;
int seq;
int pos;
int len;
REC_POS_VAL():val(0),pos(-1),len(-1){}
};


class CharRecord : public Record
{
public:
	typedef std::map<std::string, REC_POS_VAL> _CVAL_MAP ;
protected:

public:
	_CVAL_MAP	_value;
	char FIELDSEPERATOR ;
	char LINE_SEPERATOR ; 
	int NUMBER_OF_FIELDS; 
	int MAX_FIELD_LEN;
	void dump(void);
	void setREC_POS_VAL(std::string & NAME , REC_POS_VAL & t) ;
	virtual const char *GetValue(const std::string&  key);
	virtual bool  GetValueStr(const std::string&  key, std::string& );
	
	CharRecord(void);
	~CharRecord(void);
};

#endif