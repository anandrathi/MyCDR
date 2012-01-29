#include "CharRecordContainer.h"
#include "CharRecordIteraor.h"
#include "ace/Log_Msg.h"
#include "FileReader.h"
#include <vector>
CharRecordContainer::CharRecordContainer(void)
{
	ACE_TRACE("CharRecordContainer::CharRecordContainer");
	RecordContainer::_Recorditerator = 	&ci; 
	_pCharRecord=0;
}

void CharRecordContainer::setCharRecord(CharRecord* pCharRecord) 
{
	ACE_TRACE("CharRecordContainer::setCharRecord");
	_pCharRecord=pCharRecord;
	int len=0;
	int fcnt=0;
	CharRecord::_CVAL_MAP::iterator it = _pCharRecord->_value.begin();
	ci.FIELDSEPERATOR=_pCharRecord->FIELDSEPERATOR;
	ci.LINE_SEPERATOR=_pCharRecord->LINE_SEPERATOR;

	//this->_recordholder.reserve(_pCharRecord->_value.size());
	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecordContainer record len=%d \n", len));
	//_data.Init(len, fcnt, _pCharRecord->BUFFER_SIZE);
	ci._CharRecord = _pCharRecord;
	ci._RecordData_For_VarSize = &this->_data;

}

CharRecordContainer::~CharRecordContainer(void)
{
}

int  CharRecordContainer::Populate(const std::string& p)
{
	ACE_TRACE("CharRecordContainer::Populate");
	int ret = RecordContainer::Populate(p);
	ACE_DEBUG ((LM_DEBUG, "(%t) ret(%d) = RecordContainer::Populate(p) \n", ret));
	if(ret==-1) 
	{
		throw std::exception("RecordContainer::Populate Unscessful");
	}
	_FileReader->size();
	const char *data =  this->_FileReader->getData();
	ci.setData(const_cast<char*>(data)) ; 
	ci.setfilesize( this->_FileReader->size()); 
	ACE_DEBUG ((LM_DEBUG, "(%t) filesize(%d) \n", ci.getfilesize()));
	return ret;
}


void CharRecordContainer::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) FixedRecordContainer \n"));
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
