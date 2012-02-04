#include "soci.h"
#include "soci-sqlite3.h"

struct FIELD_DETAILS
{
	int BUFFER_SIZE;//=4000;
	int FIELDSEPERATOR ;//= 44;
	int LINE_SEPERATOR ;//= 10; 
	int FIELDS ;//= 8; 
	int MAX_FIELD_LEN ;//= 60;
	int NUMBER_OF_FIELDS ;//= 8; 
};

struct INFIELDS
{ 
	std::string NAME; //= "CALLER_NUMBER" ;  	
	int POS; //=1; 
	int LEN;//=20 ;
	int mapto_fieldid; // "CALLER_NUMBER" ;	
} ; 

struct INELEMENT 
{		
	std::string PATH ;//="CSV";
	std::string FILE_EXP;//="NOR*";
	std::string ELEMENT_NAME ;//= "NORTEL1";
	std::string TYPE ;//= "C/F";
	std::string SCRIPT ;//= "E:/ani/MyCDR/nortel1.nut";
	std::string LOGLEVEL ;//= (4,32,128); 
	FIELD_DETAILS _field_details ;
	INFIELDS _infields; 
}; 


class DBConfig
{
public:
	DBConfig(std::string connectString):m_connectString(connectString) {}
	virtual bool Init()=0;
	virtual ~DBConfig();

protected:
	std::string m_connectString;
	soci::backend_factory *m_backEnd ;//= *soci::factory_sqlite3();
	soci::session *m_sql;
private:

};