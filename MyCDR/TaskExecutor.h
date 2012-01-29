#pragma once

#ifndef AR_TASKEXECUTOR_H
#define AR_TASKEXECUTOR_H
//#include <ace/Task.h>
#include <ace/Task_T.h>
#include <ace/Mutex.h>
#include "RecordFactory.h"
//#include "cpptcl.h"
#include <sqrat.h>

class LoggerCallback;
class DirMoniter;
class FileReader;
struct sqlite3 ;

class TaskExecutor : public ACE_Task<ACE_MT_SYNCH>
{
protected:
	typedef ACE_Task<ACE_MT_SYNCH> inherited;
	DirMoniter * _pDirMoniter;
	sqlite3 *_db;
	int _RecordStatus;
	int _FileStatus;
	LoggerCallback *_LoggerCallback ;
public:
	static int staticTaskCount;
	int TaskCount;
	TaskExecutor(void);
	~TaskExecutor(void);
	std::string path ;
	RecContMap _RecContMap;
	int open (int pool_size = 1);
    void DBCreateRate(void);	
  virtual int open (void *void_data)
  {
    return inherited::open (void_data);
  }
	
  //void setTCLVar(void * ptcl);
  void setScriptVar(void );
  void InitScript();

  virtual int close (u_long flags = 0);
  int svc (void);
  typedef ACE_Atomic_Op<ACE_Mutex, int> counter_t;

  counter_t active_threads_;
  //Tcl::interpreter tcli;
  //Tcl_Interp * _ti;
  int enqueue (ACE_Event_Handler *handler);
protected:
	Sqrat::Script _script;
	HSQUIRRELVM _vm;
	Sqrat::RootTable * pRootTable;
	Sqrat::ConstTable * pConstTable;
	Sqrat::Table *pRecordTable;
	Sqrat::Script *pScript;
	std::string _script_path;
};

#endif