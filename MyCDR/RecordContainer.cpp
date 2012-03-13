#include "RecordContainer.h"
#include "FileReader.h"
#include "CDRTypes.h"
#include "ace/Log_Msg.h"

#include "Record.h"
RecordContainer::RecordContainer(RecordDetails * pRecordDetails ):_RecordDetails(pRecordDetails)
{
	ACE_TRACE("FileReader::ReadFullFile");

	_FileReader = new FileReader;
	_Recorditerator=0;
}

RecordContainer::~RecordContainer(void)
{
	ACE_TRACE("RecordContainer::~RecordContainer");
	delete _FileReader ;
	_FileReader=0;
}

int RecordContainer::Populate(const std::string& fileame) 
{
	ACE_TRACE("RecordContainer::Populate");
	_filename= fileame;
//	std::string fullpath = _path + "/" + _filename;
	//std::string fullpath = _filename;
	return _FileReader->ReadFullFile(_filename);
}

void RecordContainer::close(void)
{
	ACE_TRACE("RecordContainer::close");
	_FileReader->close();
}

void RecordContainer::dump(void)
{
	if(_Recorditerator) 
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  RecordContainer _Recorditerator=%d \n",	_Recorditerator));
	}
	if(_FileReader) 
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  RecordContainer _FileReader=%d \n",	_FileReader));
	}
	ACE_DEBUG((LM_DEBUG, "(%D)(%t)  CharRecord path = %s  ; 	 \n", _path.c_str() ));
}
