#pragma once

#ifndef AR_LOGGER_H
#define AR_LOGGER_H
#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"
#include "ace/SString.h"
#include <fstream>

class LoggerCallback: public ACE_Log_Msg_Callback
{
	/// Flag for testing verbose logging.
	int verbose_logging_;
	/// Flag for testing recursive callback logging.
	int _recursive;
	unsigned int  _prio ;
	std::string _filename;
	std::ofstream _outStream;
	unsigned long _logSize;
	unsigned long _MaxlogSize;
	ACE_TCHAR verbose_msg[ACE_Log_Record::MAXVERBOSELOGMSGLEN];
	bool isFileOpen;

public:

	inline unsigned int  getLevel(){return _prio;}
	inline void setLevelThread(unsigned int  p){_prio=p; ACE_LOG_MSG->priority_mask (p, ACE_Log_Msg::THREAD);  }
	inline void setLevelProcess(unsigned int  p){_prio=p; ACE_LOG_MSG->priority_mask (p, ACE_Log_Msg::PROCESS);  }
	LoggerCallback();
	inline void verbose (int be_verbose) {  this->verbose_logging_ = be_verbose;}
	inline 	void log (ACE_Log_Record &log_record) ;
	static LoggerCallback *  RegisterCallBack();
};
 
class Logger 
{
public:
	Logger(void);
	~Logger(void);
};

#endif