#include "Config.h"
#include "UnitTest.h"
#include "FixedRecordContainer.h"
#include "CharRecordContainer.h"
#include "cpptcl.h"
#include "RecordFactory.h"
#include "Driver.h"
#include "DirMoniter.h"
#include "MMapFileReader.h"
#include <map>
#include <iostream>
#include <string>
#include <sqrat.h>
#include "RecordFactory.h"

UnitTest::UnitTest(void)
{
}

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif

UnitTest::~UnitTest(void)
{
}
void printfunc(HSQUIRRELVM v,const SQChar *s,...) ;
SQInteger errorhandler(HSQUIRRELVM v);
bool UnitTest::TestScriptConstantVar(void)
{

  HSQUIRRELVM _vm = sq_open(1024);
#if SQUIRREL_VERSION_NUMBER >= 300
  sq_setprintfunc(_vm, printfunc, printfunc);
//  sq_setprintfunc(_vm, (SQPRINTFUNCTION)printf , (SQPRINTFUNCTION)printf );
#else
  sq_setprintfunc(_vm, printfunc);
//  sq_setprintfunc(_vm, (SQPRINTFUNCTION)printf );
#endif

  sq_newclosure(_vm, errorhandler,0);
  sq_seterrorhandler(_vm);
  char * names = "MyName" ;
  char * vals[] = {"Anand1" , "Anand2" , "Anand3" , "Anand4" , "Anand5"  };
  SQChar *varName = const_cast<SQChar*>(names);
  Sqrat::Table test(_vm);
  Sqrat::RootTable mRootTable(_vm);
  mRootTable.Bind(_SC("Test"), test);
  try{
  	  //Sqrat::ConstTable mConstTable(_vm);

	  Sqrat::Script script(_vm);
  	  SQChar *addr = const_cast<SQChar*>( vals[0] ); 
  	  //mConstTable.Const( varName, addr );
	  test.SetValue( varName, addr );
      script.CompileString(_SC(" \
		::print(\"Test.MyName=\"+ Test.MyName + \" \\n \"); \
							   "));
	  for(int i =0 ; i < 5 ; i++ )
	  {
         try 
		 {
  		    SQChar *addr = const_cast<SQChar*>( vals[i] ); 
  		    //mConstTable.Const( varName, addr );
			test.SetValue(varName, addr );
			//script.CompileString(_SC(" print(\"MyName=\"+ MyName + \" \\n \"); "));
 //     script.CompileString(_SC(" \
	//::print(\"MyName=\"+ MyName + \" \\n \"); \
	//::print(\"ANand= \\n \"); \
	//						   "));
            script.Run();
		 }
		 catch(Sqrat::Exception ex) 
		 {
			std::cerr << "Compile Failed: " << ex.Message();
		  }
	  }
  }
  catch(...)
  {
	  return false;
  }
  return true;
}

bool UnitTest::TestCharRecordParse(void)
{
	//RECORDFACTORY::instance()->Init();
	RecordFactory::RECORD_MAP::iterator itor =  RECORDFACTORY::instance()->_record_map.begin();
	for(;itor!=RECORDFACTORY::instance()->_record_map.end();itor++)
	{
		DirMoniter pDirMoniter(itor->first);
		PathRecordPair t;
		//	contains Record * _Record ; RecordContainer * _RecordContainer; TaskExecutor * _TaskExecutor;
		t._RecContMap._RecordContainer = itor->second._RecordContainer;
		pDirMoniter.Collect();
		DirMoniter::PATHVEC::iterator it = pDirMoniter._pathvec.begin();
		int i=0;
		for(;it!=pDirMoniter._pathvec.end();it++)
		{
			std::string srtPath = it->string();
			i++;
			if(!DirMoniter::testPath(srtPath))
			{
				return false;
			}
			t._RecContMap._RecordContainer->Populate( it->string() );
			Recorditerator * pRecorditerator = t._RecContMap._RecordContainer->GetRecordIterator();
			if(pRecorditerator->Next()!=Recorditerator::RECORDPOSITION_END)
			{
				std::cout << pRecorditerator->_RecordNumber << std::endl;
				if(pRecorditerator->getData()!=0)std::cout << pRecorditerator->getData();
				//this->setTCLVar(this->tcli.get());
			}
			
		}
		
	}
	//_path_record[itor->first]=t;
	return false;
}

bool TestCharIteratorN(void)
{
//	CharRecordIteraor charRecorditerator;
	return false;
}

bool TestFixIteratorN(void)
{
	std::map<std::string, std::string> s;
	RecordDetails fd;
	FixedRecordIteraor fixRecorditerator(&fd);
	
	fixRecorditerator.setData("0001 2  3 4  5 6 7 8");
		while(1) 
		{
			fixRecorditerator.Next();
			Recorditerator::RECORDPOSITION pos = fixRecorditerator.now();
			if(pos==fixRecorditerator.RECORDPOSITION_END)
			{
				break;
			}
			if(fixRecorditerator.getState()==Recorditerator::RECORDPOSITION_INVALID )
			{
				continue;
			}
			fixRecorditerator.setSTRVar(s);
			std::map<std::string, std::string>::iterator it=s.begin();
			for(;it!=s.end();it++)
			{
				std::cout << it->first << ":" << it->second << "\n";
			}
		}
	return false;
}

bool UnitTest::TestCharIterator(void)
{
	RecordDetails *fr=0;
	RecordFactory::RECORD_MAP::iterator itor = RECORDFACTORY::instance()->_record_map.begin();
	
	for(;itor!=RECORDFACTORY::instance()->_record_map.end();itor++)
	{
		PathRecordPair t;
		t._RecContMap._RecordContainer = itor->second._RecordContainer;
		t._RecContMap._RecordDetails = itor->second._RecordDetails;
		if(itor->first=="CSV")
		{
			fr= &t._RecContMap._RecordDetails;
		}
	}
	CharRecordIteraor charRecordIteraor(fr);
	std::string dstr(" 000000001, 000000002, 000000003, 000000004 ,000000005 ,000000006 ,000000007 ,000000008\n 000000002 ,000000003 ,000000004 ,000000005 ,000000006 ,000000007 ,000000008 ,000000009");
	charRecordIteraor.setData((char*)dstr.c_str());
	charRecordIteraor.setfilesize(dstr.length());

	while(1) 
	{
		std::map<std::string, std::string> s;
		charRecordIteraor.Next();
		Recorditerator::RECORDPOSITION pos = charRecordIteraor.now();
		if(pos==charRecordIteraor.RECORDPOSITION_END)
		{
			break;
		}
		if(charRecordIteraor.getState()==Recorditerator::RECORDPOSITION_INVALID )
		{
			continue;
		}
		charRecordIteraor.setSTRVar(s);
		std::map<std::string, std::string>::iterator it=s.begin();
		for(;it!=s.end();it++)
		{
			std::cout << it->first << ":" << it->second << "\n";
		}
	}
	return false;
}

bool UnitTest::TestFixIterator(void)
{
	
	RecordDetails *fr=0;
	RecordFactory::RECORD_MAP::iterator itor = RECORDFACTORY::instance()->_record_map.begin();

	for(;itor!=RECORDFACTORY::instance()->_record_map.end();itor++)
	{
		PathRecordPair t;
		t._RecContMap._RecordContainer = itor->second._RecordContainer;
		t._RecContMap._RecordDetails = itor->second._RecordDetails;
		if(itor->first=="FIX")
		{
			fr= &t._RecContMap._RecordDetails;
		}
	}
	FixedRecordIteraor fixRecorditerator(fr);
	std::string dstr(" 000000001 000000002 000000003 000000004 000000005 000000006 000000007 000000008\n 000000002 000000003 000000004 000000005 000000006 000000007 000000008 000000009");
	fixRecorditerator.setData((char*)dstr.c_str());
	fixRecorditerator.setfilesize(dstr.length());

	while(1) 
	{
		std::map<std::string, std::string> s;
		fixRecorditerator.Next();
		Recorditerator::RECORDPOSITION pos = fixRecorditerator.now();
		if(pos==fixRecorditerator.RECORDPOSITION_END)
		{
			break;
		}
		if(fixRecorditerator.getState()==Recorditerator::RECORDPOSITION_INVALID )
		{
			continue;
		}
		fixRecorditerator.setSTRVar(s);
		std::map<std::string, std::string>::iterator it=s.begin();
		for(;it!=s.end();it++)
		{
			std::cout << it->first << ":" << it->second << "\n";
		}
	}
	return true;
}

bool UnitTest::TestCharFileRead(void)
{
	//RECORDFACTORY::instance()->Init();
	RecordFactory::RECORD_MAP::iterator itor =  RECORDFACTORY::instance()->_record_map.begin();
	for(;itor!=RECORDFACTORY::instance()->_record_map.end();itor++)
	{
		DirMoniter pDirMoniter(itor->first);
		PathRecordPair t;
		//	contains Record * _Record ; RecordContainer * _RecordContainer; TaskExecutor * _TaskExecutor;
		t._RecContMap._RecordContainer = itor->second._RecordContainer;
		pDirMoniter.Collect();
		DirMoniter::PATHVEC::iterator it = pDirMoniter._pathvec.begin();
		int i=0;
		for(;it!=pDirMoniter._pathvec.end();it++)
		{
			std::string srtPath = it->string();
			i++;
			if(!DirMoniter::testPath(srtPath))
			{
				return false;
			}

			t._RecContMap._RecordContainer->Populate( it->string() );
			RecordDetails::FIELDDETAILS::iterator it = t._RecContMap._RecordDetails.GetFieldDetails()->begin();
			RecordDetails::FIELDDETAILS::iterator eit = t._RecContMap._RecordDetails.GetFieldDetails()->end();
			//Record::RECORDDATANAME::iterator it = t._RecContMap._RecordContainer->GetRecordIterator()->getRecord()->_recordDataName.begin();
			//Record::RECORDDATANAME::iterator ite =  t._RecContMap._RecordContainer->GetRecordIterator()->getRecord()->_recordDataName.end();
			std::cout << "";
			int recno = 0;
			for(;it!=eit;it++)
			{
				recno++;
				std::cout << recno << " : ";
				std::cout << it->NAME << " : ";
				std::cout << &it->_RecordData.cell[0] << " : ";
				//RecordDataName::DATACELL::iterator citend = it->cell.end();
			}
		}
		//_path_record[itor->first]=t;
	}
	return false;
}

bool UnitTest::TestMMapFileReader(void)
{
	MMapFileReader mMap;
	int ret = mMap.ReadFile("1.txt");
	if( ret==0)
	{
		mMap.close();
		return true;
	}
	mMap.close();
	return false;
}

bool UnitTest::TestCharRecord(void)
{
	RecordDetails cr;
	cr.FIELDSEPERATOR=',';
	cr.LINE_SEPERATOR='\n';

	std::string fields[8] = {"CALLER_NUMBER" , "CALLED_NUMBER" , "TIME", "DURATION", "CALLER_IMEI", "CALLED_IMEI", "CALLER_MSC", "CALLED_msc" };	

	cr.AddCVSField(fields[0] , 1 , 20, 1);
	cr.AddCVSField(fields[1] , 2 , 20, 1);
	cr.AddCVSField(fields[2], 3 , 20, 1);
	cr.AddCVSField(fields[3], 4 , 20, 1);
	cr.AddCVSField(fields[4], 5 , 20, 1);
	cr.AddCVSField(fields[5], 6 , 20, 1);
	cr.AddCVSField(fields[6], 7 , 20, 1);
	cr.AddCVSField(fields[7], 8 , 20, 1);	
	cr.MAX_FIELD_LEN=60;

	cr.dump(std::cout );
	return true;
}

bool UnitTest::TestFixedRecordCOntainer(void)
{
	//REC_POS_VAL t;
	//CharRecord cr;
	RecordDetails cr;
	cr.FIELDSEPERATOR=',' ;
	cr.LINE_SEPERATOR='\n' ;
	std::string fields[8] = {"CALLER_NUMBER" , "CALLED_NUMBER" , "TIME", "DURATION", "CALLER_IMEI", "CALLED_IMEI", "CALLER_MSC", "CALLED_msc" };	

	cr.AddFIXField(fields[0] , 1 , 20, 1);
	cr.AddFIXField(fields[1] , 2 , 20, 1);
	cr.AddFIXField(fields[2], 3 , 20, 1);
	cr.AddFIXField(fields[3], 4 , 20, 1);
	cr.AddFIXField(fields[4], 5 , 20, 1);
	cr.AddFIXField(fields[5], 6 , 20, 1);
	cr.AddFIXField(fields[6], 7 , 20, 1);
	cr.AddFIXField(fields[7], 8 , 20, 1);	
	
	cr.MAX_FIELD_LEN=60;

	FixedRecordContainer fc(&cr);
	//fc.setPath("C:\\temp\\char_cdr.txt");
	//fc.setFixedLocationRecord(&cr);
	return false;
}


bool UnitTest::TestCharRecordCOntainer(void)
{
	//REC_POS_VAL t;
	RecordDetails cr;
	cr.FIELDSEPERATOR=',';
	cr.LINE_SEPERATOR='\n';

	std::string fields[8] = {"CALLER_NUMBER" , "CALLED_NUMBER" , "TIME", "DURATION", "CALLER_IMEI", "CALLED_IMEI", "CALLER_MSC", "CALLED_msc" };	

	cr.AddCVSField(fields[0] , 1 , 20, 1);
	cr.AddCVSField(fields[1] , 2 , 20, 1);
	cr.AddCVSField(fields[2], 3 , 20, 1);
	cr.AddCVSField(fields[3], 4 , 20, 1);
	cr.AddCVSField(fields[4], 5 , 20, 1);
	cr.AddCVSField(fields[5], 6 , 20, 1);
	cr.AddCVSField(fields[6], 7 , 20, 1);
	cr.AddCVSField(fields[7], 8 , 20, 1);	
	
	CharRecordContainer cc(&cr);
	cc.Populate("C:\\temp\\char_cdr.txt");
//	cc.setPath("C:\\temp\\char_cdr.txt");

	return false;
}
#include "DirMoniter.h"


bool UnitTest::TestDirMoniter(void)
{
	DirMoniter _DirMoniter(CONFIG::instance()->_CurrentDir + "\\CSV");
	if(_DirMoniter.Collect()!=0)
	{
		return false;
	}
	_DirMoniter.dump();
	DirMoniter::PATHVEC::iterator it = _DirMoniter._pathvec.begin();
	int i=0;
	for(;it!=_DirMoniter._pathvec.end();it++)
	{
		std::string srtPath = it->string();
		std::string srtPathDisc = it->parent_path().string() + std::string("/DISC/") ;
		std::string srtPathDone = it->parent_path().string() +  std::string("/DONE/") ;
		srtPathDisc += it->filename().string() + ".disc";
		srtPathDone += it->filename().string() + ".done";
		std::cout << "srtPath = " << srtPath << "srtPathDisc" << srtPathDisc << " srtPathDone " << srtPathDone ; 
		i++;
		if(!DirMoniter::testPath(srtPath))
		{
			return false;
		}
		if(i%2)
		{
			DirMoniter::rename(srtPath,srtPathDisc);
			if(!DirMoniter::testPath(srtPathDisc))
			{
				return false;
			}
		}
		else
		{
			DirMoniter::rename(srtPath,srtPathDone);
			if(!DirMoniter::testPath(srtPathDone))
			{
				return false;
			}
		}
	}
	return true;
}


union u_rec
{
	long rinteger;
	char * rstring; //[1024]
	time_t rtime;
};

struct _recs
{
	enum RECORDTYPE {INVALIDRECS, INTEGER, STRING, DATETIME};
	RECORDTYPE _type;
	u_rec _rec ;
	_recs():_type(INVALIDRECS) 
	{
	}
	void setInt(long _rinteger) 
	{
		if(_type!=INTEGER)
			throw std::exception("invalid TYPE Integer in _recs") ;
		_rec.rinteger= _rinteger;
	}
	void setStr(const char * _rstring) 
	{ 
		if(_type!=STRING)
			throw std::exception("invalid TYPE STRING in _recs") ;
		_rec.rstring = new char[ strlen( _rstring ) ] ;
	}
	void setData(time_t _rtime) 
	{ 
		if(_type!=DATETIME)
			throw std::exception("invalid TYPE DATETIME in _recs") ;
		_rec.rtime = _rtime;
	}
};

/*
class Record
{
	std::map<std::string , _recs> _data;	
public:
	void add(std::string & s, _recs::RECORDTYPE rectype )
	{ 
		_recs ur;
		ur._type=rectype;
		_data[s]=ur; 
	};
	void deleteItem(std::string &n ) {_data.erase(n);}
	void reset(){_data.clear();}
	void setData(std::string& k, std::string& v) 
	{
		_recs &vt =  _data[k];
		switch(vt._type)
		{
		case _recs::INTEGER:
			vt.setInt(atol(v.c_str()));
			break;
		case _recs::DATETIME:
			break;
		case _recs::STRING:
			vt.setStr(v.c_str());
			break;
		default:

		};
	}
};
*/
//class RecordFactory
//{
	//Record _rec;
	//public:
/*
		RecordFactory::RecordFactory(){}
		void start() {_rec.reset();} ; 
		void AddFieldLong(std::string &n ) { _rec.add(n, _recs::INTEGER); }
		void AddFieldString(std::string &n ) { _rec.add(n,  _recs::STRING); }
		void AddFieldDateTime(std::string &n ) { _rec.add(n, _recs::DATETIME); }
		void deleteItem(std::string &n ) { _rec.deleteItem(n) ; }
		void *  createRecord() 
		{

		}
};*/


bool UnitTest::TestTCLBind(void)
{
	const char *varName = "MYVAR";
	int type =TCL_LINK_STRING | TCL_LINK_READ_ONLY;	
	Tcl::interpreter tcli;
	Tcl_Interp * _ti;
	_ti = tcli.get();
	char current_title_space[60];
    char *addr = current_title_space; 
	strncpy(current_title_space,"Hello World!", 60);
	int ret = Tcl_LinkVar(_ti, varName, (char*)&addr, type);
	if(TCL_OK!=ret)
		return false;

	std::string str = " set X   $MYVAR \n"
		" set Y [string range $MYVAR 0 3] \n"
		" puts $Y \n"
		" puts $X \n";

    // tcli.class_<Record>("Record", Tcl::no_init)
		  //.def("createRecord", &Record::setData)
    //      .def("AddFieldString", &Record::AddFieldString);

    // tcli.class_<RecordFactory>("RecordFactory", Tcl::init<>())
    //      .def("AddFieldLong", &RecordFactory::AddFieldLong)
    //      .def("AddFieldDateTime", &RecordFactory::AddFieldDateTime)
		  //.def("createRecord", &RecordFactory::createRecord)
    //      .def("AddFieldString", &RecordFactory::AddFieldString);

	try {
		tcli.eval(str);


	}
	catch(...){
		return false;
	}
	return true;
}

bool UnitTest::TestFILESCANTIMEOUT(void)
{
	CONFIG::instance()->Init();
	if(CONFIG::instance()->InitiFILESCANTIMEOUT())
	{
		return true;
	}else
	{
		return false;
	}
}
