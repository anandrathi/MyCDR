
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
	ACE_DEBUG ((LM_ERROR, "(%t) test folders %s \n %s\n %s \n ",path.c_str() , srtPathDisc.c_str(), srtPathDone.c_str()   ));	
	if(!DirMoniter::testPath(srtPath))
	{
		ACE_ERROR ((LM_ERROR, "(%t) folder %s dosent exists  \n",path.c_str()));	
		return false;
	}
	if(!DirMoniter::testPath(srtPathDisc))
	{
		ACE_ERROR ((LM_ERROR, "(%t) folder %s dosent exists  \n",srtPathDisc.c_str()));	
		return false;
	}
	if(!DirMoniter::testPath(srtPathDone))
	{
		ACE_ERROR ((LM_ERROR, "(%t) folder %s dosent exists  \n",srtPathDone.c_str()));	
		return false;
	}
	return true;
}

//bool RecordFactory::InitInFile(const libconfig::Setting &pelement) 
//{
//	
//	ACE_TRACE("RecordFactory::InitInFile");
//	bool isOk= true; 
//	int count = pelement.getLength();
//
//	for(int i = 0; i < count; ++i)
//	{
//		const libconfig::Setting &element = pelement[i];
//
//		// Only output the record if all of the expected fields are present.
//		//libconfig::Setting FILE_TYPE;
//
//		if(!(element.lookupValue("PATH", this->PATH)
//			&& element.lookupValue("FILE_EXP", this->FILE_EXP)
//			&& element.lookupValue("ELEMENT_NAME", this->ELEMENT_NAME)
//			&& element.lookupValue("TYPE", this->TYPE)
//			))
//			continue;
//		if(! element.exists("FILE_TYPE"))
//			continue;
//		if(! element.exists("FIELDS"))
//			continue;
//		if(!element.lookupValue( "SCRIPT", this->SCRIPT_PATH ))
//		{
//			/* 
//			(LM_ERROR , LM_CRITICAL, LM_ALERT, LM_EMERGENCY)
//			*/
//			ACE_ERROR ((LM_ERROR, "SCRIPT file dosent exists  \n" ));	
//		}
//
//		if(!element.lookupValue( "LOGLEVEL", this->_LogLevel ))
//		{
//			/* 
//			(LM_ERROR , LM_CRITICAL, LM_ALERT, LM_EMERGENCY)
//			*/
//			this->_LogLevel = (LM_ERROR , LM_CRITICAL, LM_ALERT, LM_EMERGENCY);
//		}
//
//		if(!this->TestPaths(this->PATH))
//		{
//			ACE_ERROR ((LM_ERROR, "(%t) One of the Paths not exixsts \n"));	
//			ACE_ERROR ((LM_ERROR, "(%t) %s \n",this->PATH.c_str()));	
//			ACE_ERROR ((LM_ERROR, "(%t) %s/DISC \n",this->PATH.c_str()));	
//			ACE_ERROR ((LM_ERROR, "(%t) %s/DONE \n",this->PATH.c_str()));	
//			abort();
//		}
//		if(! element.lookupValue("BUFFER_SIZE", BUFFER_SIZE))
//		{
//			BUFFER_SIZE=4000;
//		}
//	
//		//libconfig::Setting & FILE_TYPE = element["FILE_TYPE"]["TYPE"];
//		const libconfig::Setting &elementFILETYPE = element["FILE_TYPE"];
//	
//		if( _pathMap.find(this->PATH) != _pathMap.end())
//		{
//			_pathMap[this->PATH]=0;
//		}
//		else
//		{
//			_pathMap[this->PATH] =  _pathMap[this->PATH] + 1;
//			_pathMapDup[this->PATH] = _pathMap[this->PATH];
//			this->duplicate = 1;
//		}
//
//		const libconfig::Setting &elementFIELDS =  element["FIELDS"] ;
//		if(TYPE=="CSV")
//		{
//			bool isCSV;
//			isCSV= InitCSV(elementFILETYPE, elementFIELDS,TYPE.c_str()) ;
//			if(isCSV)
//			{
//				_record_map[this->PATH]._RecordContainer->setScriptPath(this->SCRIPT_PATH);
//				_record_map[this->PATH]._RecordContainer->setPath(this->PATH);
//				_record_map[this->PATH]._RecordContainer->setBufferSize(BUFFER_SIZE);
//				_record_map[this->PATH]._Record->setLogLevel(this->_LogLevel);
//			}
//			isOk &= isCSV;
//
//		} else if(TYPE=="FIXED")
//		{
//			bool isFIX;
//			isFIX = InitFix(elementFILETYPE, elementFIELDS, TYPE.c_str()) ;
//			if(isFIX )
//			{
//				//_record_map[this->PATH]._RecordContainer = RECORDCONTAINERFACTORY::instance()->GetRecordContainer(TYPE.c_str()) ;
//				_record_map[this->PATH]._RecordContainer->setScriptPath(this->SCRIPT_PATH);
//				_record_map[this->PATH]._RecordContainer->setPath(this->PATH);
//				_record_map[this->PATH]._RecordContainer->setBufferSize(BUFFER_SIZE);
//				_record_map[this->PATH]._Record->setLogLevel(this->_LogLevel);
//			}
//			isOk &= isFIX;
//		}
//		//std::cout << PATH << FILE_EXP <<  ELEMENT_NAME << std::endl;
//
//	}
//
//	if(this->duplicate == 1)
//	{
//		isOk=false;
//	}
//	return isOk;
//}


//bool RecordFactory::InitCSV(const libconfig::Setting &FILETYPE, const libconfig::Setting &FIELDS, const char * typestr) 
//{
//
//	bool isReturn = true;
//	int fieldcount=0;	
//	if(!FIELDS.isList())
//	{
//		isReturn =  false;
//		return isReturn;
//	}
//	if(!(FILETYPE.lookupValue("FIELDSEPERATOR", this->FIELDSEPERATOR)
//		&& FILETYPE.lookupValue("LINE_SEPERATOR", this->LINE_SEPERATOR)
//		&& FILETYPE.lookupValue("NUMBER_OF_FIELDS", this->NUMBER_OF_FIELDS)
//		&& FILETYPE.lookupValue("MAX_FIELD_LEN", this->MAX_FIELD_LEN)
//		))
//	{
//		isReturn =  false;
//		return isReturn;
//	}
//
//	int iBUFFER_SIZE=BUFFER_SIZE;
//	if(! FILETYPE.lookupValue("BUFFER_SIZE", iBUFFER_SIZE))
//	{
//		iBUFFER_SIZE=BUFFER_SIZE;
//	}
//
//	CharRecord * pCharRecord = new  CharRecord;
//	pCharRecord->BUFFER_SIZE=iBUFFER_SIZE;
//	
//	pCharRecord->NUMBER_OF_FIELDS	=	this->NUMBER_OF_FIELDS ; 
//	pCharRecord->MAX_FIELD_LEN		= 	this->MAX_FIELD_LEN	;
//	pCharRecord->FIELDSEPERATOR		=	this->FIELDSEPERATOR	;
//	pCharRecord->LINE_SEPERATOR		=	this->LINE_SEPERATOR	;	
//	fieldcount	=	FIELDS.getLength();
//	for(int i = 0; i < fieldcount; ++i)
//	{
//		const libconfig::Setting &elementFIELD =  FIELDS[i];
//		//std::string NAME;
//		REC_POS_VAL t;
//
//		if( ! 
//			(
//			elementFIELD.lookupValue("NAME",NAME)
//			&& elementFIELD.lookupValue("POS", t.pos )
//			&& elementFIELD.lookupValue("LEN", t.len )
//			)
//			)
//		{
//			isReturn =  false;
//			break;
//		}
//		t.seq=i+1;
//		pCharRecord->setREC_POS_VAL(NAME,t);
//	}
//	if(isReturn==false)
//	{
//		delete pCharRecord;
//		pCharRecord=0;
//	}
//	else
//	{
//		_record_map[this->PATH]._Record =  pCharRecord;
//		_record_map[this->PATH]._RecordContainer = RECORDCONTAINERFACTORY::instance()->GetRecordContainer(typestr) ;
//		CharRecordContainer* pCharRecordContainer =  dynamic_cast<CharRecordContainer*>(_record_map[this->PATH]._RecordContainer);
//		pCharRecordContainer->setBufferSize(iBUFFER_SIZE);
//		pCharRecordContainer->setCharRecord(pCharRecord);
//		_listcsv.push_back(pCharRecord);
//		pCharRecord=0;
//	}
//
//	return isReturn;
//}

//bool RecordFactory::InitFix(const libconfig::Setting &FILETYPE, const libconfig::Setting &FIELDS, const char * typestr)
//{
//	bool isReturn = true;
//	int fieldcount=0;	
//
//	if(!FIELDS.isList())
//	{
//		return false;
//	}
//	if(!(
//		FILETYPE.lookupValue("LINE_SEPERATOR", this->LINE_SEPERATOR)
//		&& FILETYPE.lookupValue("NUMBER_OF_FIELDS", this->NUMBER_OF_FIELDS)
//		))
//	{
//		return false;
//	}
//	FixedLocationRecord * pFixedLocationRecord = new  FixedLocationRecord;
//	pFixedLocationRecord->NUMBER_OF_FIELDS	=	this->NUMBER_OF_FIELDS ; 
//	pFixedLocationRecord->LINE_SEPERATOR	=	this->LINE_SEPERATOR	;	
//	int iBUFFER_SIZE=BUFFER_SIZE;
//	if(! FILETYPE.lookupValue("BUFFER_SIZE", iBUFFER_SIZE))
//	{
//		iBUFFER_SIZE=BUFFER_SIZE;
//	}
//	pFixedLocationRecord->BUFFER_SIZE=iBUFFER_SIZE;
//	fieldcount	=	FIELDS.getLength();
//	for(int i = 0; i < fieldcount; ++i)
//	{
//		const libconfig::Setting &elementFIELD =  FIELDS[i];
//		//std::string NAME;
//		FixedLocationRecord_Val t;
//		if( ! 
//			(
//			elementFIELD.lookupValue("NAME",NAME)
//			&& elementFIELD.lookupValue("START", t.start )
//			&& elementFIELD.lookupValue("END", t.end )
//			)
//			)
//		{
//			isReturn =  false;
//			break;
//		}
//		t.seq=i+1;
//
//		pFixedLocationRecord->setFixedLocationRecord_Val(NAME,t);
//	}
//	if(isReturn==false)
//	{
//		delete pFixedLocationRecord;
//		pFixedLocationRecord=0;
//	}
//	else
//	{
//		_listfix.push_back(pFixedLocationRecord);
//		_record_map[this->PATH]._Record =  pFixedLocationRecord;
//		_record_map[this->PATH]._RecordContainer = RECORDCONTAINERFACTORY::instance()->GetRecordContainer(typestr) ;
//		FixedRecordContainer* pFixedRecordContainer =  dynamic_cast<FixedRecordContainer*>(_record_map[this->PATH]._RecordContainer);
//		pFixedRecordContainer->setBufferSize(iBUFFER_SIZE);
//		pFixedRecordContainer->setFixedLocationRecord(pFixedLocationRecord);
//		pFixedLocationRecord=0;
//	}
//	return isReturn;
//}
//
//bool RecordFactory::InitGeneral() 
//{
//	std::string NAME;
//	std::string TYPE;
//	int LEN;
//
//	libconfig::Setting * psCOMMONFIELDS	= CONFIG::instance()->GetSettings("COMMONFIELDS");
//	if(psCOMMONFIELDS!=0) 
//	{
//		libconfig::Setting & sCOMMONFIELDS = *psCOMMONFIELDS;
//		if(sCOMMONFIELDS.isList()) 
//		{
//			int count = sCOMMONFIELDS.getLength();
//			for(int i = 0; i < count; ++i)
//			{
//				const libconfig::Setting &element = sCOMMONFIELDS[i];
//				if(! element.exists("NAME"))
//				{
//					ACE_ERROR ((LM_ERROR, "(%t) RecordFactory::InitGeneral  element.exists(NAME)\n"));	
//					return false;
//				}
//				if(! element.exists("TYPE"))
//				{
//					ACE_ERROR ((LM_ERROR, "(%t) RecordFactory::InitGeneral  element.exists(TYPE)\n"));	
//					return false;
//				}
//				if(! element.exists("LEN"))
//				{
//					ACE_ERROR ((LM_ERROR, "(%t) RecordFactory::InitGeneral  element.exists(LEN)\n"));	
//					return false;
//				}
//				if(!(element.lookupValue("NAME",NAME) 	&& element.lookupValue("TYPE", TYPE ) && element.lookupValue("LEN", LEN ) ))
//				{
//					return false;
//				}
//				GENREC_INFO t;
//				if(TYPE=="TEXT")
//				{
//					t.type = GENREC_INFO::RECTYPE::TEXT;
//				}
//				if(TYPE=="NUMBER")
//				{
//					t.type = GENREC_INFO::RECTYPE::NUMBER;
//				}
//				if(TYPE=="DATE")
//				{
//					t.type = GENREC_INFO::RECTYPE::DATE;
//				} else {
//					t.type = GENREC_INFO::RECTYPE::TEXT;
//				}
//				t.len=LEN;
//				GENERALRECORD::instance()->_value[NAME]=t;
//				GENERALRECORD::instance()->len += LEN;
//			}
//			std::string sql ;
//			sql = " CREATE TABLE RATED ( ";
//			std::stringstream ss;
//			
//			ss<< sql;
//			GeneralRecord::GVAL_MAP::iterator it =  GENERALRECORD::instance()->_value.begin();
//			int sql_cols = GENERALRECORD::instance()->_value.size();
//			for(int i=0;it!=GENERALRECORD::instance()->_value.end();it++, i++)
//			{
//				ss << it->first;
//				if(i < sql_cols-1  )
//					ss << ", ";
//			}
//			ss << " ); ";
//			GENERALRECORD::instance()->sqlstr = ss.str();
//			ACE_DEBUG ((LM_DEBUG, "(%t) RecordFactory  sqlstr = %s \n", GENERALRECORD::instance()->sqlstr.c_str() ));	
//		}
//	}
//}

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

		ACE_DEBUG ((LM_DEBUG, "(%t) NAME=(%s) , TYPE=(%s) , LEN=(%d) \n", NAME, TYPE, LEN ));
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
			ACE_DEBUG ((LM_DEBUG, "(%t) No records in INELEMENT \n"));	
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
			ACE_DEBUG ((LM_DEBUG, "(%t) No records in IN_FIELD_DETAILS \n"));	
			abort();
	}
	}
	catch (soci::soci_error & e)
	{
		ACE_DEBUG ((LM_DEBUG, "(%t) Exception sql RecordFactory::DBInit %s \n", e.what()));	
		abort();
	}
	return isReturn;
}


void RecordFactory::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) RecordFactory::dump \n"));	
	//ACE_DEBUG ((LM_DEBUG, "(%t)  LINE_SEPERATOR=%s: NUMBER_OF_FIELDS=%s : MAX_FIELD_LEN=%s \n",	
	//	PATH.c_str() ,
	//	FILE_EXP.c_str() , 
	//	ELEMENT_NAME.c_str() ));

	RECORD_MAP::iterator recit = _record_map.begin();
	for(;recit != _record_map.end();recit++ )
	{
		recit->second._RecordDetails.dump(std::cout);
	}

}
