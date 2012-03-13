#include <string>
#include "MyDateTime.h"
#include <sqrat.h>



void RegisterInScripts(	HSQUIRRELVM _vm, Sqrat::RootTable * pRootTable )
{
	//MyDateTime(std::string tz);
	//MyDateTime(std::string tz, std::time_t & t);
	//MyDateTime(std::string tz, boost::posix_time::ptime & t);
	//MyDateTime(std::string tz, std::string & fmt , std::string & str );

	 Sqrat::Class<MyDateTime> mydtm(_vm,true);
	 
	 mydtm.Func(_SC("_tostring"), &MyDateTime::ToString ) 
	.Func(_SC("getFormatedtime"), &MyDateTime::getFormatedtime )
	.Func(_SC("setFormatedtime"), &MyDateTime::setFormatedtime )
	.Func(_SC("gettime_t"), &MyDateTime::gettime_t )
	.Func(_SC("get_tm"), &MyDateTime::get_tm )
	.Func(_SC("get_posix_tz"), &MyDateTime::get_posix_tz )
	.Func(_SC("Set_time_t"), &MyDateTime::Set_time_t )
	.Func(_SC("Set_FromStrTZ"), &MyDateTime::Set_FromStrTZ  )
	.Func(_SC("Set_FromStr"), &MyDateTime::Set_FromStr )
	.Func(_SC("addDay"), &MyDateTime::addDay )
	.Func(_SC("addHours"), &MyDateTime::addHours )
	.Func(_SC("addMin"), &MyDateTime::addMin )
	.Func(_SC("addseconds"), &MyDateTime::addseconds )
	.Func(_SC("decrementDay"), &MyDateTime::decrementDay )
	.Func(_SC("decrementHours"), &MyDateTime::decrementHours )
	.Func(_SC("decrementMin"), &MyDateTime::decrementMin )
	.Func(_SC("decrementSeconds"), &MyDateTime::decrementSeconds );
 	 pRootTable->Bind(_SC("DateTime"), mydtm);

}