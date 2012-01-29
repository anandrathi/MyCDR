
#include "FileReader.h"
#include "MMapFileReader.h"
#include "ace/Log_Msg.h"


FileReader::FileReader()
{
	ACE_TRACE("FileReader::FileReader");
	_FileReaderStrategy = new MMapFileReader;
	_data=0;
}


FileReader::~FileReader(void)
{
}

int FileReader::ReadFullFile(std::string p)
{
	ACE_TRACE("FileReader::ReadFullFile");
	path=p;
	int ret = _FileReaderStrategy->ReadFile(path.c_str());
	if(ret==-1)
	{
		_data = 0;
		return ret;
	}
	_data = _FileReaderStrategy->getPtr();
	return ret;
}

void FileReader::close(void)
{
	ACE_TRACE("FileReader::close");
	_FileReaderStrategy->close();
}

	
void FileReader::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) RecordContainer FileStat.FileSizeBytes  =%ul, FileStat.NumberOfLines =%ul \n",	_FileStat.FileSizeBytes , _FileStat.NumberOfLines));
	if(_FileReaderStrategy)
	{	ACE_DEBUG ((LM_DEBUG, "(%t) RecordContainer _FileReaderStrategy  =%ul, \n",	_FileReaderStrategy));
		_FileReaderStrategy->dump();
	}
	ACE_DEBUG ((LM_DEBUG, "(%t) RecordContainer path=%s, \n",	path.c_str()));
	
}