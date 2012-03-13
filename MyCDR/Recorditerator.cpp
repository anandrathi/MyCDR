#include "Recorditerator.h"
#include "ace/Log_Msg.h"

//Recorditerator::Recorditerator(void)

Recorditerator::~Recorditerator(void)
{

}

void Recorditerator::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t) Recorditerator\n"));
}
