#pragma once
#ifndef RECORDFACTORY_H
#define RECORDFACTORY_H

#include <map>
#include <string>
//#include "Record.h"
#include "Config.h"
#include <list>
#include "MyCDRBASE.h"
#include <ace/Singleton.h>
#include <ace/Null_mutex.h>

#include "Record.h"

class Record;
class FixedRecordContainer;
class CharRecordContainer;
class RecordContainer ;
class ConfigHelper
{
libconfig::Setting * psIN_ELEMENT;
};

struct RecContMap { 
	RecordDetails _RecordDetails;
	RecordContainer * _RecordContainer;
};

class RecordFactory : public MyCDRBASE
{

protected:
	std::string INELEMENT_DB_TYPE;
	std::string TYPE	;
	std::string _dbtype ;
	std::string _dbconnstr ;

	bool _isInit;
	bool TestPaths(std::string & path);
	bool Getrecord(const std::string& path, RecContMap * r);
public:
	typedef std::map<std::string, RecContMap > RECORD_MAP;
	//typedef std::list<Record* > LISTCSV;
	//typedef std::list<Record* > LISTFIX;
	typedef std::map<std::string, int > UNQCOUNTER_STR;
	//typedef std::map<std::string,  Record* > UNQCOUNTER_STR;

	//LISTCSV _listcsv;
	//LISTFIX _listfix;
	//UNQCOUNTER_STR _pathMap;
	//UNQCOUNTER_STR _pathMapDup;
	RECORD_MAP _record_map;
	RecordFactory(void);
	~RecordFactory(void);
	bool Init() ;
	bool InitGeneral(const std::string& sdb_type, const std::string& sdb_connstr);
	//bool InitGeneral();
	bool isInit(){return _isInit;}
	bool DBInit(const std::string& sdb_type, const std::string& sdb_connstr) ;
	void dump(void);
};


typedef ACE_Singleton<RecordFactory, ACE_Null_Mutex> RECORDFACTORY;

#endif //RECORDFACTORY_H