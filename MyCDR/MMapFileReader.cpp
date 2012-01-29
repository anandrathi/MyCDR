#include "MMapFileReader.h"
#include "ace/Log_Msg.h"

MMapFileReader::MMapFileReader(void)
{
}


MMapFileReader::~MMapFileReader(void)
{
}

 int MMapFileReader::ReadFile(const char * path)  
{
	ACE_TRACE("MMapFileReader::ReadFile");
	int ret = _pool.map(path,-1,O_RDONLY,ACE_DEFAULT_FILE_PERMS); 
	if(ret==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%t) MMapFileReader::ReadFile ret==0 %d , %s \n", ret, path));
		_ptr = (char*)_pool.addr();
	}
	ACE_DEBUG ((LM_DEBUG, "(%t) MMapFileReader::ReadFile ret==%d , ptr = %ul path =%s \n", ret, _ptr, path));
	return ret;
}

void MMapFileReader::close()
{
	ACE_TRACE("MMapFileReader::close");
	reset();
}
int MMapFileReader::reset()
{
	ACE_TRACE("MMapFileReader::reset");
	_pool.close();
	_ptr = 0;
	return 0;
}

void MMapFileReader::dump(void)
{
		ACE_DEBUG ((LM_DEBUG, "(%t) MMapFileReader %ul\n",	_ptr ));
		_pool.dump();
}
