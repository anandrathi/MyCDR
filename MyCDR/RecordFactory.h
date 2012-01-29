#pragma once
#include <map>
#include <string>
//#include "Record.h"
#include "Config.h"
#include <list>
#include "MyCDRBASE.h"
#include <ace/Singleton.h>
#include <ace/Null_mutex.h>

class Record;
class FixedRecordContainer;
class CharRecordContainer;
class RecordContainer ;
class ConfigHelper
{
libconfig::Setting * psIN_ELEMENT;
};

struct RecContMap{ 
	Record * _Record;
	RecordContainer * _RecordContainer;
};

class RecordFactory : public MyCDRBASE
{

protected:
	std::string PATH;
	std::string FILE_EXP;
	std::string ELEMENT_NAME;
	std::string NAME;
	std::string SCRIPT_PATH;

	std::string TYPE	;
	int FIELDSEPERATOR	;
	int LINE_SEPERATOR	; 
	int NUMBER_OF_FIELDS ; 
	int MAX_FIELD_LEN	 ;
	int duplicate;
	int BUFFER_SIZE;
	unsigned int _LogLevel;

	bool _isInit;
	bool TestPaths(std::string & path);
	bool InitInFile(const libconfig::Setting &element) ;
	bool InitCSV(const libconfig::Setting &FILETYPE, const libconfig::Setting &FIELDS, const char * );
	bool InitFix(const libconfig::Setting &FILETYPE, const libconfig::Setting &FIELDS, const char *);
	bool Getrecord(const std::string& path, RecContMap * r);
public:
	typedef std::map<std::string, RecContMap > RECORD_MAP;
	typedef std::list<Record* > LISTCSV;
	typedef std::list<Record* > LISTFIX;
	typedef std::map<std::string, int > UNQCOUNTER_STR;
	//typedef std::map<std::string,  Record* > UNQCOUNTER_STR;

	LISTCSV _listcsv;
	LISTFIX _listfix;
	UNQCOUNTER_STR _pathMap;
	UNQCOUNTER_STR _pathMapDup;
	RECORD_MAP _record_map;
	RecordFactory(void);
	~RecordFactory(void);
	bool Init() ;
	bool InitGeneral();
	bool isInit(){return _isInit;}
	void dump(void);
};

typedef ACE_Singleton<RecordFactory, ACE_Null_Mutex> RECORDFACTORY;
