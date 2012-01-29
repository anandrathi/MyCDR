#pragma once

#ifndef AR_CONFIG_H
#define AR_CONFIG_H

#include <ace/Singleton.h>
#include <ace/Null_mutex.h>
#include <libconfig.h++>
#include "MyCDRBASE.h"

class Config : public MyCDRBASE
{
protected:
	bool isInit;
	libconfig::Config cfg;
	libconfig::Setting *root ;
public:
	int iFILESCANTIMEOUT;
	std::string _CurrentDir;
	Config(void);
	~Config(void);
	bool Init();
	bool InitiFILESCANTIMEOUT(void);
	libconfig::Setting *  GetSettings(const char * node);
	bool  GetLong(const char * node, long * i);
	bool  GetInt(const char * node, int * i);
	bool  GetDouble(const char * node, double  *d);
	bool  GetString(const char * node, std::string  *s);
	bool GetLogLevel(libconfig::Setting &,unsigned int* level);
	bool GetRootLogLevel(unsigned int* level);
	void dump();
};

typedef ACE_Singleton<Config, ACE_Null_Mutex> CONFIG;

#endif