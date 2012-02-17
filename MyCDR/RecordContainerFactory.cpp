
#include "RecordContainerFactory.h"
#include "CharRecordContainer.h"
#include "FixedRecordContainer.h"
#include <string>


RecordContainerFactory::RecordContainerFactory(void)
{
	_CharRecordContainer = 0; 
	_FixedRecordContainer = 0;

}


RecordContainerFactory::~RecordContainerFactory(void)
{
}


RecordContainer * RecordContainerFactory::GetRecordContainer(const char* RecordType, RecordDetails * pRecordDetails)
{
	ACE_TRACE("RecordContainerFactory::GetRecordContainer");

	if(strcmp(RecordType,"C")==0)
	{	
		if(_CharRecordContainer==0 )
		{
			_CharRecordContainer = new CharRecordContainer(pRecordDetails); 
		}
		return _CharRecordContainer;
		
	} else if(strcmp(RecordType,"F")==0)
	{
		if(_FixedRecordContainer==0)
		{
			_FixedRecordContainer = new FixedRecordContainer(pRecordDetails);
		}
		return _FixedRecordContainer;
	}
	return 0;
}