#pragma once

#ifndef _CHARRECORDITERAOR_H
#define _CHARRECORDITERAOR_H

#include "Recorditerator.h"
#include "CharRecord.h"
#include <string>
//class RecordData_For_VarSize;

class CharRecordIteraor : public Recorditerator
{
	friend class FileReader;
	//CharRecordIteraor(CharRecordIteraor&){};
public:
	//RecordData_For_VarSize * _RecordData_For_VarSize;
	CharRecordIteraor(RecordDetails*	pRecordDetails);
	~CharRecordIteraor(void);
public:
	virtual void begin(void);
	//virtual bool value(RecordDetails * & );
	virtual AR_INT64 Prev(void);
	virtual AR_INT64 Next(void);
	virtual AR_INT64 getPos(void);
	virtual bool isEnd(void );
	virtual void goBegin(void );
	virtual void goEnd(void );
	virtual AR_INT64 goTo(AR_INT64 index);
	void dump(void);
	

	virtual void setScriptVar(void *);
	virtual void setSTRVar(std::map<std::string, std::string> &s);
	virtual void InitScriptVar(void *p, void * s);
};

#endif