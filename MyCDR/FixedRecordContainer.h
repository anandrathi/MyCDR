#pragma once
#ifndef AR_FIXEDRECORDCONTAINER_H
#define AR_FIXEDRECORDCONTAINER_H
#include "RecordContainer.h"
#include "FixedRecordIteraor.h"
#include "FixedLocationRecord.h"
class FixedRecordContainer : public RecordContainer
{
protected:
	FixedRecordIteraor fi;
	FixedLocationRecord* _pFixedLocationRecord;

public:
	void setFixedLocationRecord(FixedLocationRecord* pFixedLocationRecord);
	FixedRecordContainer(void);
	~FixedRecordContainer(void);
	virtual int Populate(const std::string& ) ;
	void dump(void);

};

#endif