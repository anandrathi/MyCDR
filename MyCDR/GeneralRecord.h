#pragma once
#ifndef _AR_GENERALRECORD_H
#define  _AR_GENERALRECORD_H

#include <string>
#include <map>
#include <ace/Singleton.h>
#include <ace/Null_mutex.h>
#include "MyCDRBASE.h"

struct GENREC_INFO {
	enum RECTYPE {TEXT , NUMBER , DATE};
	RECTYPE type;
	char *  val;
	int len;
	GENREC_INFO():val(0),len(-1),type(TEXT){}
};

class GeneralRecord : public MyCDRBASE
{
public:
	GeneralRecord(void);
	~GeneralRecord(void);
public:
	int len;
	std::string sqlstr;
	typedef std::map<std::string, GENREC_INFO> GVAL_MAP;
	GVAL_MAP _value;
	void dump(void);
	virtual const char *GetValue(const std::string&  key);
	virtual bool  GetValueStr(const std::string&  key, std::string& );
	void setSQLLITE(void *);
};

typedef ACE_Singleton<GeneralRecord, ACE_Null_Mutex> GENERALRECORD;

#endif