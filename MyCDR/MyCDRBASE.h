#pragma once

#ifndef MYCDRBASE_H
#define MYCDRBASE_H
#include <ACE/Trace.h>
class MyCDRBASE
{
public:
	MyCDRBASE(void);
	virtual void dump(void )=0;
	virtual ~MyCDRBASE(void);
};

#endif