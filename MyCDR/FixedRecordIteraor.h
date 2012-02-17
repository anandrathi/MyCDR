
#ifndef AR_FIXEDRECORDITERAOR_H 
#define AR_FIXEDRECORDITERAOR_H 
#include "Recorditerator.h"
#include "FixedLocationRecord.h"

class FixedLocationRecord;
class FixedRecordIteraor :public Recorditerator
{

	friend class FileReader;

public:

	FixedLocationRecord* _FixedLocationRecord;
	//RecordData_For_VarSize * _RecordData_For_VarSize;
	FixedRecordIteraor(RecordDetails*	pRecordDetails);
	~FixedRecordIteraor(void);
	int _reclen;
public:
//	FixedLocationRecord* _FixedLocationRecord;
	virtual void begin(void);
//	virtual bool value(Record * & );
	virtual AR_INT64 Prev(void);
	virtual AR_INT64 Next(void);
	virtual AR_INT64 getPos(void);
	virtual bool isEnd(void );
	virtual void goBegin(void );
	virtual void goEnd(void );
	virtual AR_INT64 goTo(AR_INT64 index);
	void dump(void);
//	virtual void setTCLVar(void *);
	virtual void setSTRVar(std::map<std::string, std::string> &s);
	virtual void setScriptVar(void *);
	virtual void InitScriptVar(void *p, void *s);

public:
};

#endif