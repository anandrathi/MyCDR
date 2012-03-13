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
//[
	std::string s1;
	std::string s2;
	std::string s3;
	std::string s4;
	std::string s5;
	std::string s6;
	std::string s7;
	std::string s8;
	std::string s9;
	std::string s10;
	std::string s11;
	std::string s12;
	std::string s13;
	std::string s14;
	std::string s15;
	std::string s16;
	std::string s17;
	std::string s18;
	std::string s19;
	std::string s20;
	std::string s21;
	std::string s22;
	std::string s23;
	std::string s24;
	std::string s25;
	std::string s26;
	std::string s27;
	std::string s28;
	std::string s29;
	std::string s30;
	std::string s31;
	std::string s32;
	std::string s33;
	std::string s34;
	std::string s35;
	std::string s36;
	std::string s37;
	std::string s38;
	std::string s39;
	std::string s40;
	std::string s41;
	std::string s42;
	std::string s43;
	std::string s44;
	std::string s45;
	std::string s46;
	std::string s47;
	std::string s48;
	std::string s49;
	std::string s50;
	std::string s51;
	std::string s52;
	std::string s53;
	std::string s54;
	std::string s55;
	std::string s56;
	std::string s57;
	std::string s58;
	std::string s59;
	std::string s60;
	std::string s61;
	std::string s62;
	std::string s63;
	std::string s64;
	std::string s65;
	std::string s66;
	std::string s67;
	std::string s68;
	std::string s69;
	std::string s70;
	std::string s71;
	std::string s72;
	std::string s73;
	std::string s74;
	std::string s75;
	std::string s76;
	std::string s77;
	std::string s78;
	std::string s79;
	std::string s80;
	std::string s81;
	std::string s82;
	std::string s83;
	std::string s84;
	std::string s85;
	std::string s86;
	std::string s87;
	std::string s88;
	std::string s89;
	std::string s90;
	std::string s91;
	std::string s92;
	std::string s93;
	std::string s94;
	std::string s95;
	std::string s96;
	std::string s97;
	std::string s98;
	std::string s99;
//]
//[
	double  i1;
	double  i2;
	double  i3;
	double  i4;
	double  i5;
	double  i6;
	double  i7;
	double  i8;
	double  i9;
	double  i10;
	double  i11;
	double  i12;
	double  i13;
	double  i14;
	double  i15;
	double  i16;
	double  i17;
	double  i18;
	double  i19;
	double  i20;
	double  i21;
	double  i22;
	double  i23;
	double  i24;
	double  i25;
	double  i26;
	double  i27;
	double  i28;
	double  i29;
	double  i30;
	double  i31;
	double  i32;
	double  i33;
	double  i34;
	double  i35;
	double  i36;
	double  i37;
	double  i38;
	double  i39;
	double  i40;
	double  i41;
	double  i42;
	double  i43;
	double  i44;
	double  i45;
	double  i46;
	double  i47;
	double  i48;
	double  i49;
	double  i50;
	double  i51;
	double  i52;
	double  i53;
	double  i54;
	double  i55;
	double  i56;
	double  i57;
	double  i58;
	double  i59;
	double  i60;
	double  i61;
	double  i62;
	double  i63;
	double  i64;
	double  i65;
	double  i66;
	double  i67;
	double  i68;
	double  i69;
	double  i70;
	double  i71;
	double  i72;
	double  i73;
	double  i74;
	double  i75;
	double  i76;
	double  i77;
	double  i78;
	double  i79;
	double  i80;
	double  i81;
	double  i82;
	double  i83;
	double  i84;
	double  i85;
	double  i86;
	double  i87;
	double  i88;
	double  i89;
	double  i90;
	double  i91;
	double  i92;
	double  i93;
	double  i94;
	double  i95;
	double  i96;
	double  i97;
	double  i98;
	double  i99;
//]
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