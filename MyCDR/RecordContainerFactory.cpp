
#include "RecordContainerFactory.h"
#include "CharRecordContainer.h"
#include "FixedRecordContainer.h"
#include <string>


RecordContainerFactory::RecordContainerFactory(void)
{
}


RecordContainerFactory::~RecordContainerFactory(void)
{
}


RecordContainer * RecordContainerFactory::GetRecordContainer(const char* RecordType)
{
	ACE_TRACE("RecordContainerFactory::GetRecordContainer");
	static CharRecordContainer _CharRecordContainer; 
	static FixedRecordContainer _FixedRecordContainer;

	if(strcmp(RecordType,"CSV")==0)
	{	
		return &_CharRecordContainer;
		
	} else if(strcmp(RecordType,"FIXED")==0)
	{
		return &_FixedRecordContainer;
		
	}
	return 0;
}