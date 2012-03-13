#include "FileReaderStrategy.h"
#include "ace/Log_Msg.h"


FileReaderStrategy::FileReaderStrategy(void)
{
	_ptr=0;
}


FileReaderStrategy::~FileReaderStrategy(void)
{
}

void FileReaderStrategy::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t) FileReaderStrategy \n"));
	if(_ptr)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t) FileReaderStrategy %ul\n",	_ptr ));
	}
}
