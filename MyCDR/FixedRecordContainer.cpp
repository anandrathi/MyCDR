#include "FixedRecordContainer.h"
#include "ace/Log_Msg.h"
#include "FileReader.h"
FixedRecordContainer::FixedRecordContainer(void)
{
	ACE_TRACE("FixedRecordContainer::FixedRecordContainer");
	RecordContainer::_Recorditerator = &fi;
	_pFixedLocationRecord=0;

}

void FixedRecordContainer::setFixedLocationRecord(FixedLocationRecord* pFixedLocationRecord) 
{
	ACE_TRACE("CharRecordContainer::setCharRecord");
	_pFixedLocationRecord=pFixedLocationRecord;
	int len=0;
	int fcnt=0;
	//FixedLocationRecord::_CVAL_MAP::iterator it = _pCharRecord->_value.begin();
	//fi.FIELDSEPERATOR=_pCharRecord->FIELDSEPERATOR;
	fi.LINE_SEPERATOR=_pFixedLocationRecord->LINE_SEPERATOR;

	//this->_recordholder.reserve(_pCharRecord->_value.size());
	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecordContainer record len=%d \n", len));
	//_data.Init(len, fcnt, _pCharRecord->BUFFER_SIZE);
	fi._FixedLocationRecord = _pFixedLocationRecord;
	fi._reclen = fi._FixedLocationRecord->getrecLen();
	//fi._RecordData_For_VarSize = &this->_data;

}


FixedRecordContainer::~FixedRecordContainer(void)
{
}

int  FixedRecordContainer::Populate(const std::string& p)
{
	ACE_TRACE("FixedRecordContainer::Populate");
	int ret = RecordContainer::Populate(p);
	ACE_DEBUG ((LM_DEBUG, "(%t) ret(%d) = RecordContainer::Populate(p) \n", ret));

	if(ret==-1) 
	{
		throw std::exception("FixedRecordContainer::Populate Unscessful");
	}
	_FileReader->size();

	const char *data =  this->_FileReader->getData();
	fi.setData(const_cast<char*>(data)) ; 
	fi.setfilesize(this->_FileReader->size()); 
	
	ACE_DEBUG ((LM_DEBUG, "(%t) filesize(%d) \n", fi.getfilesize()));
	return ret;
}

void FixedRecordContainer::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) FixedRecordContainer \n"));
	fi.dump();
}
