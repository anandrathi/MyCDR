#pragma once

#ifndef _MYDATETIME_H
#define _MYDATETIME_H

#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/date_time/local_time/local_time.hpp>

class MyDateTime
{
public:
	//std::string _tz;
	boost::shared_ptr<boost::local_time::posix_time_zone> _ptz;
	//boost::gregorian::date _d;
	boost::local_time::local_time_facet * output_facet; 
	boost::local_time::local_time_input_facet * input_facet ;
	boost::local_time::local_date_time _ldt;
	//boost::posix_time::ptime _t;
	std::stringstream * pss;

	void initSS();

public:
	~MyDateTime();
	MyDateTime();
	MyDateTime(std::string tz);


	MyDateTime(std::string tz, std::time_t & t);
	MyDateTime(std::string tz, boost::posix_time::ptime & t);

	MyDateTime(std::string tz, std::string & fmt , std::string & str );


	const std::string ToString() const ;
	std::string getFormatedtime(std::string  fmt);
	void setFormatedtime(std::string  fmt, std::string  str);
	time_t gettime_t();
	tm  get_tm();
	std::string  get_posix_tz();

	void Set_time_t(std::time_t  t);
	void Set_FromStrTZ(std::string  fmt, std::string  str);
	void Set_FromStr(std::string  t);
	void addDay(int i) ;

	void addHours(int i) ;

	void addMin(int i) ;

	void addseconds(int i) ;

	void decrementDay(int i) ;

	void decrementHours(int i) ;

	void decrementMin(int i) ;

	void decrementSeconds(int i) ;
};

#endif