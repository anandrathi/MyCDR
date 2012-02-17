#pragma once
#ifndef _RECORDCONTAINERFACTORY_H
#define _RECORDCONTAINERFACTORY_H

#include <ace/Singleton.h>
#include <ace/Null_mutex.h>

#include "Record.h"

class RecordContainer;
class CharRecordContainer;
class FixedRecordContainer;

class RecordContainerFactory
{
public:
	CharRecordContainer * _CharRecordContainer; 
	FixedRecordContainer * _FixedRecordContainer;
	RecordContainerFactory(void);
	~RecordContainerFactory(void);
	RecordContainer * GetRecordContainer(const char* RecordType, RecordDetails * pRecordDetails);
};

typedef ACE_Singleton<RecordContainerFactory, ACE_Null_Mutex> RECORDCONTAINERFACTORY;

#endif // _RECORDCONTAINERFACTORY_H
