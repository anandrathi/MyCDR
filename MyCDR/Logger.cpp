#include "Config.h"
#include "Logger.h"
#include "ace/Thread.h"
#include "ace/OS.h"
#include <iostream>


Logger::Logger(void)
{
}


Logger::~Logger(void)
{
}
#include "CDRTypes.h"

static void getLogFilename(char * tLofFile)
{
	time_t t;
	ACE_OS::time(&t);
	ACE_thread_t tid= ACE_Thread::self();
	sprintf(tLofFile, "LOG/log.txt"  );

}

LoggerCallback::LoggerCallback()
{
	_logSize=0;
	_MaxlogSize=16*ONELOGGERMB;
	isFileOpen=false;
	//CONFIG::instance()->GetRootLogLevel(&this->_prio);
	//setLevel(this->_prio);
	
}


void LoggerCallback::log (ACE_Log_Record &log_record) 
{
	int use_log_msg = 0;
	char tLofFile[255];
	if(_logSize>=_MaxlogSize)
	{
		std::cout << "LOG size " << _logSize << ">=" << _MaxlogSize << std::endl ;
		if(isFileOpen)
		{
			_outStream.close();
			isFileOpen=false;
		}
	}
	if(!isFileOpen)
	{
		getLogFilename(tLofFile);
		_outStream.open(tLofFile, fstream::app| fstream::out );
		if(!_outStream.is_open())
		{
			std::cerr << "Could not open " << _filename.c_str() << std::endl ;
			abort();
		}
		_logSize=0;
		isFileOpen=true;
	}
	if (!this->verbose_logging_)
	{
		/*ACE_OS::printf ("Recursive Logger callback = %s",
						ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ()));*/
		_outStream <<  log_record.msg_data ();
		std::cerr <<  log_record.msg_data ();
		_logSize +=log_record.length();
	}
	else
	{
		int result = log_record.format_msg (ACE_LOG_MSG->local_host (),
											ACE_LOG_MSG->flags (),
											verbose_msg);
		if (result == 0)
		{
			_outStream <<  verbose_msg;
			std::cerr <<  log_record.msg_data ();
			_logSize +=log_record.length();
		}
	}
	_outStream.flush();
	// Cleanup on the way out.
}

LoggerCallback *LoggerCallback::RegisterCallBack()
{
	LoggerCallback *pCallback = new LoggerCallback;

	ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
	ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
	ACE_LOG_MSG->msg_callback (pCallback);
	unsigned int ll;
	CONFIG::instance()->GetRootLogLevel(&ll);
	ACE_LOG_MSG->priority_mask(ll,ACE_Log_Msg::THREAD);
	return pCallback;
}