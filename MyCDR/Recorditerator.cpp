#include "Recorditerator.h"
#include "ace/Log_Msg.h"

Recorditerator::Recorditerator(void)
{
	_RecordNumber=0;
	_TotalRecords=0;
}

Recorditerator::~Recorditerator(void)
{
}

void Recorditerator::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) Recorditerator\n"));
}
