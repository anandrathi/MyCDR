#include "TaskExecutor.h"
#include "ace/Event_Handler.h"
#include "ace/OS.h"
//#include "ace/Event_Handler.h"
#include "ace/Synch_Traits.h"
#include "RecordContainer.h"
#include "ace/Log_Msg.h"
#include <ctime>
#include "sqlite3.h"
#include "GeneralRecord.h"
#include "DirMoniter.h"
#include "Logger.h"

int TaskExecutor::staticTaskCount=0;


TaskExecutor::TaskExecutor(void)
{
	ACE_TRACE("TaskExecutor::TaskExecutor");
	_pDirMoniter = 0;
	//_ti =  tcli.get();
	staticTaskCount++;
	TaskCount=staticTaskCount;
}

void TaskExecutor::DBCreateRate(void)	
{
	ACE_TRACE("TaskExecutor::DBCreateRate");
	char str[60];	
	char *zErrMsg = 0;
    int rc;
	time_t ct;
	time(&ct);
	sprintf(str, "%04d_%ul",TaskCount, ct);
	//Tclsqlite_Init(_ti);
	//int DbMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

    rc = sqlite3_open_v2(str, &_db, SQLITE_OPEN_NOMUTEX| SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0 );
    if( rc )
	{
	  ACE_ERROR((LM_ERROR, "(%t)  TaskExecutor::DBCreate failed for %s  %s \n", str, sqlite3_errmsg(_db) ));
      //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(_db));
      sqlite3_close(_db);
	  exit(1);
    }
	GENERALRECORD::instance()->setSQLLITE( _db );
}

TaskExecutor::~TaskExecutor(void)
{
}

int
TaskExecutor::open (int pool_size)
{
	ACE_TRACE("TaskExecutor::open");
	_pDirMoniter =  new DirMoniter(this->path);
	DBCreateRate();
	return this->activate (THR_NEW_LWP|THR_DETACHED, pool_size);
}

int
TaskExecutor::close (u_long flags)
{
	ACE_TRACE("TaskExecutor::close ");
	ACE_UNUSED_ARG(flags);

  /* Find out how many threads are currently active */
  int counter = active_threads_.value ();

  /* For each one of the active threads, enqueue a "null" event
    handler.  Below, we'll teach our svc() method that "null" means
    "shutdown".  */
  while (counter--)
    this->enqueue (0);

  /* As each svc() method exits, it will decrement the active thread
    counter.  We just wait here for it to reach zero.  Since we don't
    know how long it will take, we sleep for a quarter of a second
    between tries.  */
  while (active_threads_.value ())
    ACE_OS::sleep (ACE_Time_Value (0, 250000));

  return(0);
}

int
TaskExecutor::enqueue (ACE_Event_Handler *handler)
{

	ACE_TRACE("TaskExecutor::enqueue");
  void *v_data = (void *) handler;
  char *c_data = (char *) v_data;

  ACE_Message_Block *mb;

  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (c_data),
                  -1);

  if (this->putq (mb) == -1)
    {
      mb->release ();
      return -1;
    }

  return 0;
}

class Counter_Guard
{
public:
  Counter_Guard (TaskExecutor::counter_t &counter)
    : counter_ (counter)
  {
    ++counter_;
  }

  ~Counter_Guard (void)
  {
    --counter_;
  }

protected:
  TaskExecutor::counter_t &counter_;
};


void TaskExecutor::setScriptVar( void )
{
	ACE_TRACE("TaskExecutor::setScriptVar");
	unsigned long  i=0;

}

//void TaskExecutor::setTCLVar(void * ptcl)
//{
//	ACE_TRACE("TaskExecutor::setTCLVar");
//	unsigned long  i=0;
//	char *varName = "RecordStatus";
//	int type =TCL_LINK_INT ;
//	Tcl_Interp * _ti = (Tcl_Interp *)ptcl;
//	Tcl_LinkVar(_ti, varName, (char*)&_RecordStatus, type);
//	varName = "FileStatus";
//	Tcl_LinkVar(_ti, varName, (char*)&_FileStatus, type);
//}


int
TaskExecutor::svc (void)
{
ACE_TRACE("TaskExecutor::svc");

LoggerCallback * pLoggerCallback =  LoggerCallback::RegisterCallBack();
pLoggerCallback->setLevelThread( this->_RecContMap._Record->getLogLevel() );
while(1)
{
	ACE_Message_Block *mb;
	Counter_Guard counter_guard (active_threads_);


	ACE_DEBUG((LM_DEBUG, "(%t) TaskExecutor::svc sleep for %d seconds \n", CONFIG::instance()->iFILESCANTIMEOUT));
	ACE_Time_Value getsleeptimet(  (CONFIG::instance()->iFILESCANTIMEOUT>0?CONFIG::instance()->iFILESCANTIMEOUT:5), 0);
	if( this->getq (mb,&getsleeptimet) != -1)
	{
		return 0;
	}
	else 
	{
		_pDirMoniter->Collect();
		DirMoniter::PATHVEC::iterator it = _pDirMoniter->_pathvec.begin();
		for(;it!=_pDirMoniter->_pathvec.end();it++)
		{
			std::string srtPath = it->string();
			std::string srtPathDisc = it->parent_path().string() + std::string("/DISC/") ;
			std::string srtPathDone = it->parent_path().string() +  std::string("/DONE/") ;
			srtPathDisc += it->filename().string() + ".disc";
			srtPathDone += it->filename().string() + ".done";
			ACE_ERROR((LM_ERROR, "(%t) TaskExecutor::svc srtPath=%s srtPathDisc=%s srtPathDone=%s\n",  srtPath.c_str(),  srtPathDisc.c_str(), srtPathDone.c_str() ));
			try{
				_RecContMap._RecordContainer->Populate(srtPath);
				Recorditerator * pRecorditerator = _RecContMap._RecordContainer->GetRecordIterator();
		
			//if(pRecorditerator->Next()!=Recorditerator::END)
				pRecorditerator->begin();
				pRecorditerator->InitScriptVar( pRecordTable , pScript );
				bool markBadFile=false;
				while(1) 
				{
					pRecorditerator->Next();
					Recorditerator::RECORDPOSITION pos = pRecorditerator->now();
					if(pos==pRecorditerator->RECORDPOSITION_END)
					{
						break;
					}
					if(pRecorditerator->getState()!=Recorditerator::RECORDPOSITION_OK)
					{
						markBadFile=true;
						break;
					}
					_RecordStatus=0;
					_FileStatus=0;
					//this->setTCLVar(this->tcli.get());
					this->setScriptVar();
					pRecorditerator->setScriptVar( pRecordTable );
					if(_RecordStatus==2)
					{
						//Discard the record.
					}
					if(_FileStatus!=0)
					{
						_RecContMap._RecordContainer->close();
						_pDirMoniter->rename(srtPath,srtPathDisc);
					}
				}//while
				_RecContMap._RecordContainer->close();
				DirMoniter::rename( srtPath, srtPathDone );
			}//try
			catch(std::exception & e)
			{
				ACE_ERROR((LM_ERROR, "(%t) TaskExecutor RecordContainer->Populate failed for %s \n", (char*)it->c_str()	));
				DirMoniter::rename(srtPath,srtPathDisc);
			}
		}//for
	}
}//while
return 0;
}

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif

void printfunc(HSQUIRRELVM v,const SQChar *s,...) {
        va_list vl;
        va_start(vl, s);
        scvprintf(s, vl);
        va_end(vl);
}


SQInteger errorhandler(HSQUIRRELVM v) {
    const SQChar *sErr = 0;
    if(sq_gettop(v)>=1) {
        if(SQ_SUCCEEDED(sq_getstring(v,2,&sErr)))   {
            std::cerr << _SC("A Script Error Occured: ") << sErr;
        }
        else {
            std::cerr << _SC("An Unknown Script Error Occured.") << sErr;
        }
    }
    return 0;
}

void TaskExecutor::InitScript()
{
  _vm = sq_open(1024);
#if SQUIRREL_VERSION_NUMBER >= 300
  sq_setprintfunc(_vm, printfunc, printfunc);
#else
  sq_setprintfunc(_vm, printfunc);
#endif

  sq_newclosure(_vm, errorhandler,0);
  sq_seterrorhandler(_vm);
  pRootTable = new Sqrat::RootTable(_vm);
  pConstTable = new Sqrat::ConstTable(_vm);
  pRecordTable = new Sqrat::Table(_vm);
  pRootTable->Bind(_SC("Record"), *pRecordTable);
  pScript = new Sqrat::Script(_vm);
  this->_script_path = _RecContMap._RecordContainer->getScriptPath();
  pScript->CompileFile( this->_script_path);
}

