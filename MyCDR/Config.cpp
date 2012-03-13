#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Config.h"
#include "CDRTypes.h"
#include "ace/Log_Msg.h"
#include <stdio.h>


Config::Config(void):isInit(false)
{
	root=0;
};
Config::~Config(void)
{
}

void Config::dump()
{
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  iFILESCANTIMEOUT =  %d \n", iFILESCANTIMEOUT));
	//ACE_Log_Msg * plog = ACE_Log_Msg::instance ();
	//FILE * f =  FILE *fdopen(stdout, "w");
	//cfg.write();
}

libconfig::Setting *  Config::GetSettings(const char * node)
{
	ACE_TRACE("Config::GetSettings");
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetSettings return null \n" ));
		return 0;
	}
  if(! root->exists(node)) 
  {
	return 0;
  }	
  else 
  {
	return &(*root)[node];
  }
}

bool  Config::GetLong(const char * node, long * i)
{
	ACE_TRACE("Config::GetLong");
	if(i==0)
		return false;
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetSettings return null \n" ));
		return false;
	}
  if(! root->exists(node)) 
  {
	return false;
  }	
  else 
  {
	*i = (*root)[node].operator long();
	return  true;
  }
}

bool Config::GetRootLogLevel(unsigned int* level)
{
	ACE_TRACE("Config::GetLogLevel");
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetLogLevel return null \n" ));
		return false;
	}
  if(! root->exists("LOGLEVEL")) 
  {
	return false;
  }	
  else 
  {
	if(!(*root)["LOGLEVEL"].isList())
	{
		  return false;
	}
	int len = (*root)["LOGLEVEL"].getLength();
	*level=0;
	for(int i = 0; i < len ; ++i)
	{
		*level |=(*root)["LOGLEVEL"][i].operator unsigned long();
	}
	return  true;
  }
}

bool Config::GetLogLevel(libconfig::Setting &setting, unsigned int* level)
{
	ACE_TRACE("Config::GetLogLevel");
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetLogLevel return null \n" ));
		return false;
	}
  if(! setting.exists("LOGLEVEL")) 
  {
	return false;
  }	
  else 
  {
	if(!setting["LOGLEVEL"].isList())
	{
		  return false;
	}
	int len = setting["LOGLEVEL"].getLength();
	*level=0;
	for(int i = 0; i < len ; ++i)
	{
		*level |=setting["LOGLEVEL"][i].operator unsigned long();
	}
	return  true;
  }

}

bool  Config::GetInt(const char * node, int * i)
{
	ACE_TRACE("Config::GetInt");
	if(i==0)
		return false;
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetInt return null \n" ));
		return false;
	}
  if(! root->exists(node)) 
  {
	return false;
  }	
  else 
  {
	  if((*root)[node].isNumber())
	  {
		*i = (*root)[node].operator int();
		return  true;
	  }

	return  false;
  }
}

bool  Config::GetDouble(const char * node, double  *d)
{
	ACE_TRACE("Config::GetDouble");
	if(d==0)
		return false;
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetDouble return null \n" ));
		return false;
	}
  if(! root->exists(node)) 
  {
	return false;
  }	
  else 
  {
	*d = (*root)[node].operator double();
	return  true;
  }
}


bool  Config::GetString(const char * node, std::string * s)
{
	ACE_TRACE("Config::GetString");
	if(s==0)
		return false;
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::GetString return null \n" ));
		return false;
	}
  if(! root->exists(node)) 
  {
	return false;
  }	
  else 
  {
	*s = (*root)[node].operator std::string();
	return  true;

	return  false;
  }
}

bool Config::InitiFILESCANTIMEOUT(void)
{
	ACE_TRACE("Config::InitiFILESCANTIMEOUT");
	//libconfig::Setting &root = *this->root;//cfg.getRoot();
	if(root==0)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Config::InitiFILESCANTIMEOUT return null \n" ));
		return false;
	}
	if(! root->exists("FILESCANTIMEOUT")) 
	{
		ACE_ERROR ((LM_DEBUG, "(%t)  Config  FILESCANTIMEOUT not found  iFILESCANTIMEOUT = 5 default \n" ));
		iFILESCANTIMEOUT = 5;
		return false;
	}	
	else 
	{
		libconfig::Setting & SettingFILESCANTIMEOUT = (*root)["FILESCANTIMEOUT"];
		if (SettingFILESCANTIMEOUT.isNumber())
		{
			iFILESCANTIMEOUT = (int)SettingFILESCANTIMEOUT;
			return true;
		}
		else 
		{
			ACE_ERROR ((LM_ERROR, "(%t)  Config  FILESCANTIMEOUT is not number iFILESCANTIMEOUT = 5 default \n" ));
			iFILESCANTIMEOUT = 5;
			return false;
		}
	}
	return true;
}

/*
  // Find the 'movies' setting. Add intermediate settings if they don't yet
  // exist.
  libconfig::Setting &root = cfg.getRoot();

  if(! root.exists("inventory"))
    root.add("inventory", libconfig::Setting::TypeGroup);

  libconfig::Setting &inventory = root["inventory"];

  if(! inventory.exists("movies"))
    inventory.add("movies", Setting::TypeList);

  Setting &movies = inventory["movies"];

  // Create the new movie entry.
  Setting &movie = movies.add(Setting::TypeGroup);

  movie.add("title", Setting::TypeString) = "Buckaroo Banzai";
  movie.add("media", Setting::TypeString) = "DVD";
  movie.add("price", Setting::TypeFloat) = 12.99;
  movie.add("qty", Setting::TypeInt) = 20;

  // Write out the updated configuration.
  try
  {
    cfg.writeFile(output_file);
    cerr << "Updated configuration successfully written to: " << output_file
         << endl;

  }
  catch(const FileIOException &fioex)
  {
    cerr << "I/O error while writing file: " << output_file << endl;
    return(EXIT_FAILURE);
  }

  return(EXIT_SUCCESS);
*/



bool Config::Init(void)
{
	ACE_TRACE("Config::Init");
	static const char *output_file = CONFIG_FILE_IN;

	// Read the file. If there is an error, report it and exit.
	try
	{
		cfg.readFile(CONFIG_FILE_IN);
		this->root = &cfg.getRoot();
	}
	catch(const libconfig::FileIOException &fioex)
	{	
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t) Config::Init I/O error while reading file. %s\n", fioex.what() ));
		std::cerr << "I/O error while reading file." << fioex.what() << std::endl;
		return(EXIT_FAILURE);
	}
	catch(const libconfig::ParseException &pex)
	{
		ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  Parse error at  %s : %d : %s\n", pex.getFile(), pex.getLine()	, pex.getError() ));
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
					<< " - " << pex.getError() << std::endl;
		return(EXIT_FAILURE);
	}
	InitiFILESCANTIMEOUT();
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t)  iFILESCANTIMEOUT =  %d \n", iFILESCANTIMEOUT));
  isInit=true;
}
