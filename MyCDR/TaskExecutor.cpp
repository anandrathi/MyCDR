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
#include "Record.h"

int TaskExecutor::staticTaskCount=0;


TaskExecutor::TaskExecutor(void)
{
	ACE_TRACE("TaskExecutor::TaskExecutor");
	_pDirMoniter = 0;
	staticTaskCount++;
	TaskCount=staticTaskCount;
}

void TaskExecutor::DBCreateRate(void)	
{
	ACE_TRACE("TaskExecutor::DBCreateRate");
	char str[600];	
	char *zErrMsg = 0;
    int rc;
	time_t ct;
	time(&ct);
	sprintf(str, "%04d_%ul",TaskCount, ct);

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
pLoggerCallback->setLevelThread( this->_RecContMap._RecordDetails.LOGLEVEL  );
while(1)
{
	ACE_Message_Block *mb;
	Counter_Guard counter_guard (active_threads_);


	ACE_DEBUG((LM_DEBUG, "(%D)(%t)  TaskExecutor::svc sleep for %d seconds \n", CONFIG::instance()->iFILESCANTIMEOUT));
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
			ACE_ERROR((LM_ERROR, "(%D)(%t) TaskExecutor::svc srtPath=%s srtPathDisc=%s srtPathDone=%s\n",  srtPath.c_str(),  srtPathDisc.c_str(), srtPathDone.c_str() ));
			try{
				_RecContMap._RecordContainer->Populate(srtPath);
				Recorditerator * pRecorditerator = _RecContMap._RecordContainer->GetRecordIterator();
		
			//if(pRecorditerator->Next()!=Recorditerator::END)
				pRecorditerator->begin();
				//pRecorditerator->InitScriptVar( pRecordTable , pScript );
				bool markBadFile=false;
				while(1) 
				{
					pRecorditerator->Next();
					Recorditerator::RECORDPOSITION pos = pRecorditerator->now();
					if(pos==pRecorditerator->RECORDPOSITION_END)
					{
						break;
					}
					if(pRecorditerator->getState()!=Recorditerator::RECORDERROR_GOOD ) //Recorditerator::RECORDPOSITION_OK
					{
						markBadFile=true;
						break;
					}
					_RecordStatus=0;
					_FileStatus=0;
					//this->setTCLVar(this->tcli.get());
					this->setScriptVar();
					pRecorditerator->setScriptVar( pRecordTable );
					pScript->Run();
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
				//DirMoniter::rename( srtPath, srtPathDone );
			}//try
			catch(std::exception & e)
			{
				ACE_ERROR((LM_ERROR, "(%D)(%t) TaskExecutor RecordContainer->Populate failed for %s \n", (char*)it->c_str()	));
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
		char str[1024];
		va_list vl;
        va_start(vl, s);
        scvprintf(s, vl);
		vsnprintf (str, 1024 , s, vl);
        va_end(vl);
		ACE_ERROR ((LM_ERROR, "[%s]\n", str));
}


SQInteger errorhandler(HSQUIRRELVM v) {
    const SQChar *sErr = 0;
    if(sq_gettop(v)>=1) {
        if(SQ_SUCCEEDED(sq_getstring(v,2,&sErr)))   {
            std::cerr << _SC("A Script Error Occured: ") << sErr;
			ACE_ERROR ((LM_ERROR, "(%D)(%t) A Script Error Occured: = %s \n", (char*)sErr ));

        }
        else {
            std::cerr << _SC("An Unknown Script Error Occured.") << sErr;
			ACE_ERROR ((LM_ERROR, "(%D)(%t) An Unknown Script Error Occured. = %s \n", (char*)sErr ));
        }
    }
    return 0;
}

std::string TaskExecutor::* TS[99] = {
&TaskExecutor::s1 , &TaskExecutor::s2, &TaskExecutor::s3, &TaskExecutor::s4, &TaskExecutor::s5, &TaskExecutor::s6, &TaskExecutor::s7, &TaskExecutor::s8, &TaskExecutor::s9, &TaskExecutor::s10,
&TaskExecutor::s11 , &TaskExecutor::s12, &TaskExecutor::s13, &TaskExecutor::s14, &TaskExecutor::s15, &TaskExecutor::s16, &TaskExecutor::s17, &TaskExecutor::s18, &TaskExecutor::s19, &TaskExecutor::s20,
&TaskExecutor::s21 , &TaskExecutor::s22, &TaskExecutor::s23, &TaskExecutor::s24, &TaskExecutor::s25, &TaskExecutor::s26, &TaskExecutor::s27, &TaskExecutor::s28, &TaskExecutor::s29, &TaskExecutor::s30,
&TaskExecutor::s31 , &TaskExecutor::s32, &TaskExecutor::s33, &TaskExecutor::s34, &TaskExecutor::s35, &TaskExecutor::s36, &TaskExecutor::s37, &TaskExecutor::s38, &TaskExecutor::s39, &TaskExecutor::s40,
&TaskExecutor::s41 , &TaskExecutor::s42, &TaskExecutor::s43, &TaskExecutor::s44, &TaskExecutor::s45, &TaskExecutor::s46, &TaskExecutor::s47, &TaskExecutor::s48, &TaskExecutor::s49, &TaskExecutor::s50,
&TaskExecutor::s51 , &TaskExecutor::s52, &TaskExecutor::s53, &TaskExecutor::s54, &TaskExecutor::s55, &TaskExecutor::s56, &TaskExecutor::s57, &TaskExecutor::s58, &TaskExecutor::s59, &TaskExecutor::s60,
&TaskExecutor::s61 , &TaskExecutor::s62, &TaskExecutor::s63, &TaskExecutor::s64, &TaskExecutor::s65, &TaskExecutor::s66, &TaskExecutor::s67, &TaskExecutor::s68, &TaskExecutor::s69, &TaskExecutor::s70,
&TaskExecutor::s71 , &TaskExecutor::s72, &TaskExecutor::s73, &TaskExecutor::s74, &TaskExecutor::s75, &TaskExecutor::s76, &TaskExecutor::s77, &TaskExecutor::s78, &TaskExecutor::s79, &TaskExecutor::s80,
&TaskExecutor::s81 , &TaskExecutor::s82, &TaskExecutor::s83, &TaskExecutor::s84, &TaskExecutor::s85, &TaskExecutor::s86, &TaskExecutor::s87, &TaskExecutor::s88, &TaskExecutor::s89, &TaskExecutor::s90,
&TaskExecutor::s91 , &TaskExecutor::s92, &TaskExecutor::s93, &TaskExecutor::s94, &TaskExecutor::s95, &TaskExecutor::s96, &TaskExecutor::s97, &TaskExecutor::s98, &TaskExecutor::s99 
};

double TaskExecutor::* TD[99] = {
&TaskExecutor::i1 , &TaskExecutor::i2, &TaskExecutor::i3, &TaskExecutor::i4, &TaskExecutor::i5, &TaskExecutor::i6, &TaskExecutor::i7, &TaskExecutor::i8, &TaskExecutor::i9, &TaskExecutor::i10,
&TaskExecutor::i11 , &TaskExecutor::i12, &TaskExecutor::i13, &TaskExecutor::i14, &TaskExecutor::i15, &TaskExecutor::i16, &TaskExecutor::i17, &TaskExecutor::i18, &TaskExecutor::i19, &TaskExecutor::i20,
&TaskExecutor::i21 , &TaskExecutor::i22, &TaskExecutor::i23, &TaskExecutor::i24, &TaskExecutor::i25, &TaskExecutor::i26, &TaskExecutor::i27, &TaskExecutor::i28, &TaskExecutor::i29, &TaskExecutor::i30,
&TaskExecutor::i31 , &TaskExecutor::i32, &TaskExecutor::i33, &TaskExecutor::i34, &TaskExecutor::i35, &TaskExecutor::i36, &TaskExecutor::i37, &TaskExecutor::i38, &TaskExecutor::i39, &TaskExecutor::i40,
&TaskExecutor::i41 , &TaskExecutor::i42, &TaskExecutor::i43, &TaskExecutor::i44, &TaskExecutor::i45, &TaskExecutor::i46, &TaskExecutor::i47, &TaskExecutor::i48, &TaskExecutor::i49, &TaskExecutor::i50,
&TaskExecutor::i51 , &TaskExecutor::i52, &TaskExecutor::i53, &TaskExecutor::i54, &TaskExecutor::i55, &TaskExecutor::i56, &TaskExecutor::i57, &TaskExecutor::i58, &TaskExecutor::i59, &TaskExecutor::i60,
&TaskExecutor::i61 , &TaskExecutor::i62, &TaskExecutor::i63, &TaskExecutor::i64, &TaskExecutor::i65, &TaskExecutor::i66, &TaskExecutor::i67, &TaskExecutor::i68, &TaskExecutor::i69, &TaskExecutor::i70,
&TaskExecutor::i71 , &TaskExecutor::i72, &TaskExecutor::i73, &TaskExecutor::i74, &TaskExecutor::i75, &TaskExecutor::i76, &TaskExecutor::i77, &TaskExecutor::i78, &TaskExecutor::i79, &TaskExecutor::i80,
&TaskExecutor::i81 , &TaskExecutor::i82, &TaskExecutor::i83, &TaskExecutor::i84, &TaskExecutor::i85, &TaskExecutor::i86, &TaskExecutor::i87, &TaskExecutor::i88, &TaskExecutor::i89, &TaskExecutor::i90,
&TaskExecutor::i91 , &TaskExecutor::i92, &TaskExecutor::i93, &TaskExecutor::i94, &TaskExecutor::i95, &TaskExecutor::i96, &TaskExecutor::i97, &TaskExecutor::i98, &TaskExecutor::i99 
};

void RegisterInScripts(	HSQUIRRELVM _vm, Sqrat::RootTable * pRootTable );

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
  //this->_script_path = _RecContMap._RecordContainer->getScriptPath();
  this->_script_path = _RecContMap._RecordDetails.SCRIPT;
  
	//RECORD_MAP::iterator it = _RecContMap._Record->_recordDataName.begin();
    //RECORD_MAP::iterator it = _RecContMap._Record->_recordDataName.begin();
  RecordDetails::FIELDDETAILS::iterator it  = _RecContMap._RecordDetails.GetFieldDetails()->begin();
  RecordDetails::FIELDDETAILS::iterator eit  = _RecContMap._RecordDetails.GetFieldDetails()->end();
	//Record::RECORDDATANAME::iterator it = _RecContMap._Record->_recordDataName.begin();
  
  for(; it!= eit  ;it++)
	{
		unsigned long  i=0;
		SQChar *varName = (SQChar *)it->NAME.c_str();
		SQChar *addr = const_cast<SQChar*>( "" ); 
		pRecordTable->SetValue( varName, addr ); 
	}

  RegisterInScripts( _vm, pRootTable);
  pScript->CompileFile( this->_script_path);
}

