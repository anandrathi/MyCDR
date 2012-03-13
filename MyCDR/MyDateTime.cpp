#include "MyDateTime.h"
//_ptz(new boost::local_time::posix_time_zone(tz)), 


MyDateTime::~MyDateTime()
{ 
	if(pss )
		delete pss ;
}

MyDateTime::MyDateTime():
	pss(0),
	_ldt(boost::posix_time::not_a_date_time) 
{ 

	initSS(); 
}

MyDateTime::MyDateTime(std::string tz):
pss(0),
_ldt(boost::posix_time::second_clock::local_time(), boost::shared_ptr<boost::local_time::posix_time_zone> (new boost::local_time::posix_time_zone(tz)) ) 
{ 
	initSS(); 
}

	//		_ptz(new boost::local_time::posix_time_zone(tz)),

	MyDateTime::MyDateTime(std::string tz, std::time_t & t):
		pss(0),
		_ldt(boost::posix_time::from_time_t(t), 
		boost::shared_ptr<boost::local_time::posix_time_zone> (new boost::local_time::posix_time_zone(tz)) )
	{ 
		initSS(); 
	}
	MyDateTime::MyDateTime(std::string tz, boost::posix_time::ptime & t):
		pss(0),
		_ldt(t, boost::shared_ptr<boost::local_time::posix_time_zone> (new boost::local_time::posix_time_zone(tz))  )
	{ 
		initSS(); 
	}

	MyDateTime::MyDateTime(std::string tz, std::string & fmt , std::string & str ): 
		pss(0),
		_ldt(boost::local_time::not_a_date_time, boost::shared_ptr<boost::local_time::posix_time_zone> (new boost::local_time::posix_time_zone(tz)) ) 
	{ 
		initSS();
		setFormatedtime(fmt, str);
	}

	void MyDateTime::initSS()
	{
		pss = new std::stringstream ;
		std::stringstream &ss = *pss;
		output_facet = new boost::local_time::local_time_facet ;
		input_facet = new boost::local_time::local_time_input_facet;
		ss.imbue(std::locale(std::locale::classic(), output_facet));
  		ss.imbue(std::locale(ss.getloc(), input_facet));
	}

	const std::string MyDateTime::ToString() const {
		return _ldt.to_string();
    }

	std::string MyDateTime::getFormatedtime(std::string  fmt)
	{
		std::stringstream &ss = *pss;
		output_facet->format(fmt.c_str());
		ss.clear();
		ss << _ldt;
		return ss.str();
	}
	void MyDateTime::setFormatedtime(std::string  fmt, std::string  str)
	{
		std::stringstream &ss = *pss;
		input_facet->format(fmt.c_str());
		ss.clear();
		ss.str( str.c_str() );
		ss >> _ldt;

		return ;

	}
	time_t MyDateTime::gettime_t()
	{
		tm t_tm = boost::local_time::to_tm(_ldt);
		time_t t = mktime(&t_tm);
		return t;
	}

	tm  MyDateTime::get_tm()
	{
		return boost::local_time::to_tm(_ldt);
	}

	void MyDateTime::Set_time_t(std::time_t  t)
	{
		std::stringstream &ss = *pss;
		ss << t << _ldt.zone_as_posix_string();
		ss >> _ldt;
	}

	void MyDateTime::Set_FromStrTZ(std::string  fmt, std::string  str)
	{
		setFormatedtime(fmt, str);
	}
	
	void MyDateTime::Set_FromStr(std::string  t)
	{
		std::stringstream tss;
		tss.str(t.c_str());
		tss >> _ldt;
	}

	std::string  MyDateTime::get_posix_tz()
	{
		return _ldt.zone_as_posix_string();
	}

	void MyDateTime::addDay(int i) 
	{
		boost::gregorian::days dd(i);
		_ldt +=  dd;
		//boost::posix_time::ptime t2 = _t + dd;
	}

	void MyDateTime::addHours(int i) 
	{
		//boost::posix_time::ptime t1 = _t  + boost::posix_time::hours(i);
		_ldt +=  boost::posix_time::hours(i);
		//_t = t1;
	}

	void MyDateTime::addMin(int i) 
	{
		_ldt +=  boost::posix_time::minutes(i);
		//boost::posix_time::ptime t1 = _t  + boost::posix_time::minutes(i);
		//_t = t1;
	}

	void MyDateTime::addseconds(int i) 
	{
		_ldt +=  boost::posix_time::seconds(i);
		//boost::posix_time::ptime t1 = _t  + boost::posix_time::seconds(i);
		//_t = t1;
	}

	void MyDateTime::decrementDay(int i) 
	{
		boost::gregorian::days dd(i);
		_ldt -=  dd;
	}

	void MyDateTime::decrementHours(int i) 
	{
		_ldt -=  boost::posix_time::hours(i);
		//boost::posix_time::ptime t1 = _t  - boost::posix_time::hours(i);
		//_t = t1;
	}

	void MyDateTime::decrementMin(int i) 
	{
		_ldt -=  boost::posix_time::minutes(i);
		//boost::posix_time::ptime t1 = _t  - boost::posix_time::minutes(i);
		//_t = t1;
	}

	void MyDateTime::decrementSeconds(int i) 
	{
		_ldt -=  boost::posix_time::seconds(i);
		//boost::posix_time::ptime t1 = _t  - boost::posix_time::seconds(i);
		//_t = t1;
	}
