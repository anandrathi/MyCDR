#pragma once
#include "Recorditerator.h"
#include "CharRecord.h"
#include <string>
class RecordData_For_VarSize;

class CharRecordIteraor : public Recorditerator
{
	friend class CharRecord;
	friend class FileReader;
public:
	CharRecord* _CharRecord;
	RecordData_For_VarSize * _RecordData_For_VarSize;
	char FIELDSEPERATOR ;
	char LINE_SEPERATOR ; 
	CharRecordIteraor(void);
	~CharRecordIteraor(void);
public:
	virtual void begin(void);
	virtual bool value(Record * & );
	virtual AR_INT64 Prev(void);
	virtual AR_INT64 Next(void);
	virtual AR_INT64 getPos(void);
	virtual bool isEnd(void );
	virtual void goBegin(void );
	virtual void goEnd(void );
	virtual AR_INT64 goTo(AR_INT64 index);
	void dump(void);
	Record*	getRecord(void){return _CharRecord;};

//	virtual void setTCLVar(void *);
	virtual void setScriptVar(void *);
	virtual void setSTRVar(std::map<std::string, std::string> &s);
	virtual void InitScriptVar(void *p, void * s);
};

