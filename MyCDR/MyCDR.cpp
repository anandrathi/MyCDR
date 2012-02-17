// MyCDR.cpp : Defines the entry point for the console application.
//

#include <boost/filesystem.hpp>
#include "stdafx.h"
#include "Config.h"
#include "RecordFactory.h"
#include "Driver.h"
#include "UnitTest.h"
#include <string>
#include "Logger.h"

void sql3test();
boost::filesystem::path cp =  boost::filesystem::current_path();


#define TEST_CHK(OKX)  { \
std::cout << "**************************" << #OKX  << "***************************************************\n" ;	\
std::cout << "Testing " << #OKX << "\n" ;	\
	if(OKX) \
	{		\
		std::cout << "Test "<< #OKX << "Pass\n"; \
	}	\
	else	\
	{	std::cout << "Test " << #OKX << "Pass\n";}   \
	} \
std::cout << "*********************************** END *********************************************\n" ;	


void RunTest(void)
{
	UnitTest u;
	TEST_CHK( u.TestScriptConstantVar() );
	TEST_CHK( u.TestFixIterator() );
	TEST_CHK( u.TestCharIterator() );

	//TEST_CHK( u.TestCharRecordParse() );
	//TEST_CHK( u.TestMMapFileReader() );
	//TEST_CHK( u.TestCharFileRead() );

	//TEST_CHK( u.TestFILESCANTIMEOUT() );
	//TEST_CHK( u.TestDirMoniter() );
	//TEST_CHK( u.TestTCLBind() );
	//TEST_CHK( u.TestMMapFileReader() );
	return;
}
int main(int argc, _TCHAR* argv[])
{
//	sql3test();
	if(boost::filesystem::current_path().is_complete())
	{
		std::cout << "Path is_complete \n"; 
	}
	else
	{
		std::cout << "is  Not complete \n"; 
	}
	//std::cout << cp.string();
	unsigned int level;

	CONFIG::instance()->_CurrentDir = 	cp.string();
	CONFIG::instance()->Init();
	CONFIG::instance()->GetRootLogLevel(&level);
	LoggerCallback * p = LoggerCallback::RegisterCallBack();
	p->setLevelProcess(level);

	RECORDFACTORY::instance()->Init();
	//RECORDFACTORY::instance()->InitGeneral();

	//RunTest();
	//return 0;
	DRIVER::instance()->Init();
	DRIVER::instance()->Run();
//	RECORDFACTORY::instance()->dump();

	return 0;
}

#include "sqlite3.h"

 static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
 }


void sql3test()
{
	sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
  
    rc = sqlite3_open("sqltest.arc", &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
    }
    rc = sqlite3_exec(db, "drop TABLE t;", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    rc = sqlite3_exec(db, "CREATE TABLE t(x INTEGER, y, z);", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    rc = sqlite3_exec(db, "insert into  t values ('10','20','30');", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);


}