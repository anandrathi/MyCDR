#pragma once

#ifndef  AR_FILEREADER_H
#define  AR_FILEREADER_H
#include "CDRTypes.h"
#include "MyCDRBASE.h"
#include "FileReaderStrategy.h"
class FileReaderStrategy;
struct FileStat
{
	AR_UINT64 FileSizeBytes;
	AR_UINT64 NumberOfLines;
};

class FileReader  : public MyCDRBASE
{
protected:
	FileStat _FileStat;
	std::string path;
	FileReaderStrategy * _FileReaderStrategy;
	char * _data;
public:
	FileReader();
	~FileReader(void);
	void close(void);
	const char * getData() {return _data;}
	int ReadFullFile(std::string );
	const FileStat& GetFileStat() const {return _FileStat;}
	void GetIterator(){};
	virtual long size(void){ return _FileReaderStrategy->size();}
	void dump(void);
};



#endif