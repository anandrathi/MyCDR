#pragma once

#ifndef _AR_RECORDCONTAINER_H
#define _AR_RECORDCONTAINER_H
#include "Recorditerator.h"
#include "MyCDRBASE.h"

class FileReader;
//class Recorditerator;
#include <string> 
#include <vector> 


class RecordContainer : public MyCDRBASE
{
protected:
	
	Recorditerator * _Recorditerator;
	std::string _path;
	std::string _filename;
	std::string _script_path;
	std::string _script;

	int _bufferSize;
public:
	FileReader* _FileReader;
	void close(void);
	RecordContainer(void);
	~RecordContainer(void);
	void setScriptPath(std::string& scriptPath) {_script_path = scriptPath; }
	const std::string& getScriptPath(void) {return _script_path ; }
	void setBufferSize(unsigned long bufferSize) {_bufferSize = bufferSize; }
	int getBufferSize(void) {return _bufferSize ; }
	virtual int Populate(const std::string& ) ;
	void setPath(std::string path){_path= path;}
	Recorditerator * GetRecordIterator() {return _Recorditerator;}
	void dump(void);
};

#endif