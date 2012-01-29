#pragma once

#include "FileReaderStrategy.h"
#include <ace/Mem_Map.h>

class ACE_Lite_MMAP_Memory_Pool ;
class MMapFileReader : public FileReaderStrategy
{
	ACE_Mem_Map _pool;
public:
	MMapFileReader(void);
	~MMapFileReader(void);
	long size(void){return _pool.size();}
	virtual int ReadFile(const char * path)  ;
	void dump(void);
	virtual void close(void);

	int reset();
};

