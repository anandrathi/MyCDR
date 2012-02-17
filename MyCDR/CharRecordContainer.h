#pragma once
#ifndef CHARRECORDCONTAINER_H
#define CHARRECORDCONTAINER_H
#include "RecordContainer.h"
#include "CharRecordIteraor.h"
#include "CharRecord.h"
#include <vector>

struct RecordData_For_VarSize
{
	//typedef boost::multi_array<char, 2> RECORD_ARRAY_TYPE;
	//typedef RECORD_ARRAY_TYPE::index index;
	//typedef std::vector<char> CARRAY;
	//typedef std::vector<
	//RECORD_ARRAY_TYPE *_arr;
	char *_Rec;
	char *_arr;
	unsigned long   _RecCount ;
	unsigned long   _RecLen ;
	unsigned long   _FieldCount;
	RecordData_For_VarSize(){_arr=0;_Rec=0; }
	bool Init(int RecLen , int FieldCount, int _RecCount);
};

class CharRecordContainer : public RecordContainer
{
protected:
	RecordData_For_VarSize _data;
	CharRecordIteraor * _ci;

public:
	CharRecordContainer(RecordDetails * pRecordDetails );
	~CharRecordContainer(void);
	virtual int  Populate(const std::string& ) ;
	void dump(void);

};


#endif