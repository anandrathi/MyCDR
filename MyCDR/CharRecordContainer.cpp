#include "CharRecordContainer.h"
#include "CharRecordIteraor.h"
#include "ace/Log_Msg.h"
#include "FileReader.h"
#include <vector>
CharRecordContainer::CharRecordContainer(RecordDetails * pRecordDetails ):RecordContainer(pRecordDetails)
{
	ACE_TRACE("CharRecordContainer::CharRecordContainer");
	RecordContainer::_Recorditerator = 	new CharRecordIteraor(pRecordDetails); 
	
	//int len=pRecordDetails->MAX_FIELD_LEN;
	//int fieldcount=pRecordDetails->_Fielddetails.size();

	//this->_recordholder.reserve(_pCharRecord->_value.size());
	//ACE_DEBUG ((LM_DEBUG, "(%t) CharRecordContainer record len=%d \n", len));
	//_data.Init(pRecordDetails->MAX_FIELD_LEN, fieldcount , pRecordDetails->BUFFER_SIZE);
	//ci.SetRecordDetails( pRecordDetails ) ;
}

CharRecordContainer::~CharRecordContainer(void)
{

}

int  CharRecordContainer::Populate(const std::string& p)
{
	ACE_TRACE("CharRecordContainer::Populate");
	CharRecordIteraor & ci = *(CharRecordIteraor *)RecordContainer::_Recorditerator ; 
	int ret = RecordContainer::Populate(p);
	ACE_DEBUG ((LM_DEBUG, "(%t) ret(%d) = RecordContainer::Populate(p) \n", ret));
	if(ret==-1) 
	{
		throw std::exception("RecordContainer::Populate Unscessful");
	}
	_FileReader->size();
	const char *data =  this->_FileReader->getData();
	ci.setData( const_cast<char*>(data) ) ; 
	ci.setfilesize( this->_FileReader->size() ); 
	ACE_DEBUG ((LM_DEBUG, "(%t) filesize(%d) \n", ci.getfilesize()));
	return ret;
}


void CharRecordContainer::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecordContainer \n"));
	CharRecordIteraor & ci = *(CharRecordIteraor *)RecordContainer::_Recorditerator ; 
	ci.dump();
}


bool RecordData_For_VarSize::Init(int RecLen , int FieldCount, int RecCount)
{
	_RecLen = RecLen;
	_RecCount = RecCount;
	try {
		_Rec = new char [RecLen + (FieldCount )];
		//_arr = new char [RecCount * (RecLen + FieldCount + 1)  ];
	}
	catch(...)
	{
		ACE_ERROR((LM_ERROR, "(%t) Memory failed  (RecCount* (RecLen+FieldCount+1) ) bytes= %d RecCount :%d,  RecLen :%d, FieldCount :%d\n", RecCount* (RecLen+FieldCount+1), RecCount,  RecLen, FieldCount  ));
		ACE_DEBUG((LM_DEBUG, "(%t) Memory failed  (RecCount* (RecLen+FieldCount+1) ) bytes= %d RecCount :%d,  RecLen :%d, FieldCount :%d\n", RecCount* (RecLen+FieldCount+1), RecCount,  RecLen, FieldCount  ));
		return false;
	}
	return true;
}
