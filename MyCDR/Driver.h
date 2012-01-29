#pragma once

#ifndef AR_DRIVER_H
#define AR_DRIVER_H

#include <ace/Singleton.h>
#include <ace/Null_mutex.h>
#include "RecordFactory.h"

#include <map>

class Record;
class  TaskExecutor;
struct PathRecordPair{
	//Record * _Record;
	TaskExecutor * _TaskExecutor;
	RecContMap _RecContMap; 
};

//class RecContMap;
class Driver
{
protected:
	bool isReadyToRun;
	typedef std::map<std::string, PathRecordPair> PATH_RECORD_MAP;
	PATH_RECORD_MAP _path_record;
public:
	Driver(void);
	~Driver(void);
	int Run();
	int Init();
};

typedef ACE_Singleton<Driver, ACE_Null_Mutex> DRIVER;

#endif