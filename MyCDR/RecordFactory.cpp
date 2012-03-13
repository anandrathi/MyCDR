
#include "RecordFactory.h"

#include "CharRecord.h"
#include "GeneralRecord.h"

#include "FixedLocationRecord.h"
#include "ace/Log_Msg.h"
#include "FixedRecordContainer.h"
#include "CharRecordContainer.h"
#include "RecordContainerFactory.h"
#include "DirMoniter.h"
#include <iostream>
#include <sstream>
#include "DBConfig.h"

RecordFactory::RecordFactory(void)
{

}


RecordFactory::~RecordFactory(void)
{
}

bool RecordFactory::Getrecord(const std::string& path, RecContMap * r)
{
	ACE_TRACE("RecordFactory::Getrecord");
	RECORD_MAP::iterator it = _record_map.find(path);
	if(it==_record_map.end())
		return false;
	else
		*r=  it->second;

	return true;
}

bool RecordFactory::TestPaths(std::string & path)
{
	ACE_TRACE("RecordFactory::TestPaths");
	std::string srtPath = path;
	std::string srtPathDisc = path + std::string("/DISC") ;
	std::string srtPathDone = path + std::string("/DONE") ;
	ACE_DEBUG ((LM_ERROR, "(%D)(%t) test folders %s \n %s\n %s \n ",path.c_str() , srtPathDisc.c_str(), srtPathDone.c_str()   ));	
	if(!DirMoniter::testPath(srtPath))
	{
		ACE_ERROR ((LM_ERROR, "(%D)(%t) folder %s dosent exists  \n",path.c_str()));	
		return false;
	}
	if(!DirMoniter::testPath(srtPathDisc))
	{
		ACE_ERROR ((LM_ERROR, "(%D)(%t) folder %s dosent exists  \n",srtPathDisc.c_str()));	
		return false;
	}
	if(!DirMoniter::testPath(srtPathDone))
	{
		ACE_ERROR ((LM_ERROR, "(%D)(%t) folder %s dosent exists  \n",srtPathDone.c_str()));	
		return false;
	}
	return true;
}


bool RecordFactory::Init() 
{
	//this->duplicate = 0;
	bool isReturn = true;
	libconfig::Setting * psIN_ELEMENT = CONFIG::instance()->GetSettings("INELEMENT");
	std::string sdb_type ;
	std::string sdb_connstr ;
	bool isDBtype = CONFIG::instance()->GetString("INELEMENT_DB_TYPE", &sdb_type);
	bool isDBconn = CONFIG::instance()->GetString("INELEMENT_DB_CONN", &sdb_connstr);
	if(isDBtype && isDBconn)
	{
		_dbtype = sdb_type ;
		_dbconnstr = sdb_connstr ;
		this->DBInit(sdb_type, sdb_connstr);
	}
	if( InitGeneral(sdb_type, sdb_connstr) )
	{
		ACE_ERROR ((LM_ERROR, "(%t) InitGeneral is failed \n" ));	
	}

	//if(psIN_ELEMENT!=0) 
	//{
	//	libconfig::Setting & sIN_ELEMENT = *psIN_ELEMENT;
	//	if(sIN_ELEMENT.isList()) 
	//	{
	//		isReturn = InitInFile(sIN_ELEMENT);
	//	}
	//}
	return isReturn;
}

bool RecordFactory::InitGeneral(const std::string& sdb_type, const std::string& sdb_connstr) 
{
	bool isReturn = true;
	std::string key = "inputdb";
	DBConfig* _pDBConfig = DBConfig::Get(key);
	soci::session * psql = _pDBConfig->getsql() ;
	if(_pDBConfig==0)
	{
		isReturn = false;
	}

	soci::rowset<soci::row> rs = (psql->prepare << "Select NAME , TYPE , LEN from COMMONFIELDS");
	for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
	{
		soci::row const& rw = *it;
		std::string NAME  = rw.get<std::string>(0);
		std::string TYPE  = rw.get<std::string>(1); 
		int LEN			  = rw.get<int>(2);
		GENERALRECORD::instance()->push_back(NAME, TYPE, LEN);

		ACE_DEBUG ((LM_DEBUG, "(%D)(%t) NAME=(%s) , TYPE=(%s) , LEN=(%d) \n", NAME, TYPE, LEN ));
	}
	
	return isReturn;

}

bool RecordFactory::DBInit(const std::string& sdb_type, const std::string& sdb_connstr) 
{
	bool isReturn = true;
	std::string key = "inputdb";
	DBConfig* _pDBConfig = DBConfig::Create(key, sdb_type, sdb_connstr);
	soci::session * psql = _pDBConfig->getsql() ;

	// PATH , FILE_EXP,	ELEMENT_NAME, TYPE,	SCRIPT,	LOGLEVEL 
	// BUFFER_SIZE, FIELDSEPERATOR , LINE_SEPERATOR , FIELDS , MAX_FIELD_LEN , NUMBER_OF_FIELDS 

	if(_pDBConfig==0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) Database connection not created DB Type(%s) connect str(%s)\n", sdb_type.c_str() , sdb_connstr.c_str()));	
		throw 
		isReturn = false;

	}
	try  {
	soci::rowset<soci::row> rs = (psql->prepare << "Select PATH , FILE_EXP,	ELEMENT_NAME, TYPE,	SCRIPT,	\
												    LOGLEVEL , BUFFER_SIZE, FIELDSEPERATOR , LINE_SEPERATOR , FIELDS , MAX_FIELD_LEN , NUMBER_OF_FIELDS \
												    from INELEMENT");

	// iteration through the resultset:
	int count = 0;
	for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
	{
		count++;
		soci::row const& rw = *it;
		std::string PATH = rw.get<std::string>(0);;
		RecordDetails rdTemp;
		_record_map[ PATH ]._RecordDetails = rdTemp;
		RecordDetails * prdTemp = &_record_map[ PATH ]._RecordDetails ;
		RecordDetails & rd = * prdTemp ;

		rd.PATH  = PATH;
		rd.FILE_EXP   = rw.get<std::string>(1); 
		rd.ELEMENT_NAME   = rw.get<std::string>(2);
		rd.TYPE   = rw.get<std::string>(3);
		rd.SCRIPT   = rw.get<std::string>(4);
		rd.LOGLEVEL   = rw.get<int>(5);
		rd.BUFFER_SIZE = rw.get<int>(6);
		rd.FIELDSEPERATOR  = rw.get<int>(7);
		rd.LINE_SEPERATOR = rw.get<int>(8); 
		rd.FIELDS = rw.get<int>(9); 
		rd.MAX_FIELD_LEN = rw.get<int>(10); 
		rd.NUMBER_OF_FIELDS = rw.get<int>(11); 
		
	}
	if(count == 0) 
	{
			ACE_DEBUG ((LM_DEBUG, "(%D)(%t) No records in INELEMENT \n"));	
			abort();
	}
	count = 0;
	RECORD_MAP::iterator recit = _record_map.begin();
	for(;recit != _record_map.end();recit++ )
	{
		count++;
		if(recit->second._RecordDetails.TYPE=="F")
		{
			std::string squery = "select NAME , START , END from IN_FIELD_DETAILS where PATH = '" +  recit->second._RecordDetails.PATH  + std::string("' order by seq");  
			soci::rowset<soci::row> rsfields = (psql->prepare << squery);
			int iseq=0;
			for (soci::rowset<soci::row>::const_iterator it = rsfields.begin(); it != rsfields.end(); ++it)
			{
				iseq++;
				soci::row const& rw = *it;
				std::string NAME  = rw.get<std::string>(0);
				int START   = rw.get<int>(1); 
				int END = rw.get<int>(2);
				recit->second._RecordDetails.AddFIXField(NAME , START , END, iseq);
			}
			recit->second._RecordContainer = RECORDCONTAINERFACTORY::instance()->GetRecordContainer(recit->second._RecordDetails.TYPE.c_str(), &recit->second._RecordDetails) ;
		}
		else if(recit->second._RecordDetails.TYPE=="C")
		{
			std::string squery = "select NAME , POS, LEN from IN_FIELD_DETAILS where PATH = '" +  recit->second._RecordDetails.PATH  + std::string("' order by seq");  
			soci::rowset<soci::row> rsfields = (psql->prepare << squery);
			int iseq=0;
			for (soci::rowset<soci::row>::const_iterator it = rsfields.begin(); it != rsfields.end(); ++it)
			{
				iseq++;
				soci::row const& rw = *it;
				std::string NAME = rw.get<std::string>(0);
				int POS   = rw.get<int>(1); 
				int LEN = rw.get<int>(2);
				recit->second._RecordDetails.AddCVSField(NAME , POS, LEN, iseq);
			}
			recit->second._RecordContainer = RECORDCONTAINERFACTORY::instance()->GetRecordContainer(recit->second._RecordDetails.TYPE.c_str(), &recit->second._RecordDetails) ;
		}

		//recit->second._RecordDetails._Fielddetails.
	}

	if(count == 0) 
	{
			ACE_DEBUG ((LM_DEBUG, "(%D)(%t) No records in IN_FIELD_DETAILS \n"));	
			abort();
	}
	}
	catch (soci::soci_error & e)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t) Exception sql RecordFactory::DBInit %s \n", e.what()));	
		abort();
	}
	return isReturn;
}


void RecordFactory::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t) RecordFactory::dump \n"));	
	//ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  LINE_SEPERATOR=%s: NUMBER_OF_FIELDS=%s : MAX_FIELD_LEN=%s \n",	
	//	PATH.c_str() ,
	//	FILE_EXP.c_str() , 
	//	ELEMENT_NAME.c_str() ));

	RECORD_MAP::iterator recit = _record_map.begin();
	for(;recit != _record_map.end();recit++ )
	{
		recit->second._RecordDetails.dump(std::cout);
	}

}
