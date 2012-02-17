#include "FixedRecordContainer.h"
#include "ace/Log_Msg.h"
#include "FileReader.h"
FixedRecordContainer::FixedRecordContainer(RecordDetails * pRecordDetails ):RecordContainer(pRecordDetails)
{
	ACE_TRACE("FixedRecordContainer::FixedRecordContainer");

	RecordContainer::_Recorditerator =  new FixedRecordIteraor(pRecordDetails );
	int len=0;
	int fcnt=0;
	//fi.SetRecordDetails( pRecordDetails );
	//fi.LINE_SEPERATOR = _RecordDetails->LINE_SEPERATOR;
	//this->_recordholder.reserve(_pCharRecord->_value.size());
	ACE_DEBUG ((LM_DEBUG, "(%t) FixedRecordContainer record len=%d \n", len));
	//_data.Init(len, fcnt, _pCharRecord->BUFFER_SIZE);
	//fi._FixedLocationRecord = _pFixedLocationRecord;
	//fi._reclen = fi._FixedLocationRecord->getrecLen();
	//fi._RecordData_For_VarSize = &this->_data;

}


FixedRecordContainer::~FixedRecordContainer(void)
{
}

int  FixedRecordContainer::Populate(const std::string& p)
{
	ACE_TRACE("FixedRecordContainer::Populate");
	FixedRecordIteraor & fi = *(FixedRecordIteraor *)RecordContainer::_Recorditerator ;
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
	FixedRecordIteraor &fi = *(FixedRecordIteraor *)RecordContainer::_Recorditerator ;
	ACE_DEBUG ((LM_DEBUG, "(%t) FixedRecordContainer \n"));
	fi.dump();
}
