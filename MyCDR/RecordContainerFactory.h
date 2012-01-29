#pragma once
#include <ace/Singleton.h>
#include <ace/Null_mutex.h>
class RecordContainer;
class CharRecordContainer;
class FixedRecordContainer;

class RecordContainerFactory
{
public:
	RecordContainerFactory(void);
	~RecordContainerFactory(void);
	RecordContainer * GetRecordContainer(const char* );
};

typedef ACE_Singleton<RecordContainerFactory, ACE_Null_Mutex> RECORDCONTAINERFACTORY;
