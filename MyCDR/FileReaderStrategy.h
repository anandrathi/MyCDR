#pragma once
#include <string>
#include "MyCDRBASE.h"
#include "ace/Log_Msg.h"

class FileReaderStrategy : public MyCDRBASE
{
protected:
	char * _ptr;
public:
	char * getPtr() {return _ptr;}
	virtual long size(void)=0;
	FileReaderStrategy(void);
	~FileReaderStrategy(void);
	int ReadFile(const std::string& path){return ReadFile(path.c_str());};
	virtual int ReadFile(const char * path)  =0;
	virtual int reset()=0;
	void dump(void);
	virtual void close(void)=0;


};

