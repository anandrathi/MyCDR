#include "Driver.h"
#include "TaskExecutor.h"

Driver::Driver(void)
{
}


Driver::~Driver(void)
{

}



int Driver::Init()
{
	ACE_TRACE("Driver::Init");
	RecordFactory * pRecordFactory = RECORDFACTORY::instance();
	//	RECORD_MAP::iterator it = _record_map.find(path);
	//if(it==_record_map.end())

	RecordFactory::RECORD_MAP::iterator itor =  pRecordFactory->_record_map.begin();
	for(;itor!=pRecordFactory->_record_map.end();itor++)
	{
		PathRecordPair t;
		//	contains Record * _Record ; RecordContainer * _RecordContainer; TaskExecutor * _TaskExecutor;
		t._RecContMap = itor->second;
		_path_record[itor->first]=t;
	}
	return 0;

}

int Driver::Run()
{
	ACE_TRACE("Driver::Run");
	PATH_RECORD_MAP::iterator itor =  _path_record.begin();
	for(;itor!=_path_record.end();itor++)
	{
		TaskExecutor * te = new TaskExecutor;
		te->path = itor->first;
		PathRecordPair &t = itor->second;
		te->_RecContMap = t._RecContMap;
		te->open();
		itor->second._TaskExecutor = te;
	}

	ACE_Thread_Manager::instance()->wait();
	return 0;
}
