#include "DirMoniter.h"
#include "Config.h"
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include "ace/Log_Msg.h"



template <>
class std::back_insert_iterator<DirMoniter::PATHVEC > 
		: public _Outit
	{	// wrap pushes to back of container as output iterator
public:
	typedef back_insert_iterator<DirMoniter::PATHVEC> _Myt;
	typedef DirMoniter::PATHVEC container_type;
	typedef DirMoniter::PATHVEC::const_reference const_reference;
	typedef DirMoniter::PATHVEC::value_type _Valty;

	explicit back_insert_iterator(DirMoniter::PATHVEC& _Cont)
		: container(&_Cont)
		{	// construct with container
		}

	_Myt& operator=(const _Valty& _Val)
		{	// push value into container
        
		if (boost::filesystem::is_regular_file(_Val))        // is p a regular file?   
		{
			ACE_DEBUG ((LM_DEBUG, "(%t) back_insert_iterator_ignoredir::Collect operator= %s is a file  \n",  _Val.string().c_str() ));	
			container->push_back(_Val);
		}else
		{
			ACE_ERROR ((LM_ERROR, "(%t) back_insert_iterator_ignoredir::Collect operator= %s is a file  \n",  _Val.string().c_str() ));	
		}
		return (*this);
		}

	_Myt& operator=(_Valty&& _Val)
		{	// push value into container
		if (boost::filesystem::is_regular_file(_Val))        // is p a regular file?   
		{
			ACE_DEBUG ((LM_DEBUG, "(%t) back_insert_iterator_ignoredir::Collect operator= %s is a file  \n",  _Val.string().c_str() ));	
			container->push_back(_STD forward<_Valty>(_Val));
		}else
		{
			ACE_ERROR ((LM_ERROR, "(%t) back_insert_iterator_ignoredir::Collect operator= %s is a file  \n",  _Val.string().c_str() ));	
		}
		return (*this);
		}

	_Myt& operator*()
		{	// pretend to return designated value
		return (*this);
		}

	_Myt& operator++()
		{	// pretend to preincrement
		return (*this);
		}

	_Myt operator++(int)
		{	// pretend to postincrement
		return (*this);
		}

protected:
	DirMoniter::PATHVEC *container;	// pointer to container
	};

std::back_insert_iterator<DirMoniter::PATHVEC > back_inserter_dirignore(DirMoniter::PATHVEC& _Cont)
{	

	return (std::back_insert_iterator<DirMoniter::PATHVEC >(_Cont));
}

bool DirMoniter::testPath(std::string& tpath)
{
	if (boost::filesystem::exists(tpath))    // does path actually exist?
	{
		ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::testPath path %s found\n",  tpath.c_str()  ));	
		return true;
	}
	else{
		ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::testPath path %s NOT found\n",  tpath.c_str()  ));	
	}

}

int DirMoniter::Collect()
{
	ACE_TRACE("DirMoniter::Collect");

  try
  {
		//boost::filesystem::path cp =  boost::filesystem::initial_path();
		//if(cp.is_complete())
		ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::Collect current path %s \n",  CONFIG::instance()->_CurrentDir.c_str()  ));	
		
	  if (boost::filesystem::exists(path))    // does p actually exist?
	  {
		if (boost::filesystem::is_regular_file(path))        // is p a regular file?   
		{
			ACE_ERROR ((LM_ERROR, "(%t) Error DirMoniter::Collect path %s is a file  \n",  path.c_str() ));	
			return -1;
		}
		else if (boost::filesystem::is_directory(path))      // is p a directory?
		{
			ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::Collect path %s is a directory containing: \n",  path.c_str() ));	
			std::copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), back_inserter_dirignore(_pathvec));
			//std::sort(_pathvec.begin(), _pathvec.end());    // sort, since directory iteration
			// is not ordered on some file systems
			//if (boost::filesystem::is_directory(   ))      // is p a directory?
			for (PATHVEC::const_iterator it (_pathvec.begin()); it != _pathvec.end(); ++it)
			{
				ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::Collect		%s\n",  it->string().c_str() ));	
				//std::cout << "   " << *it << '\n';
			}
		}
		else
		{
			ACE_ERROR ((LM_ERROR, "(%t) Error DirMoniter::Collect %s	exists, but is neither a regular file nor a directory	\n",  path.c_str() ));	
			return -1;
		}
       //std::cout << path << " exists, but is neither a regular file nor a directory\n";
    }
    else
	{
		ACE_ERROR ((LM_ERROR, "(%t) Error DirMoniter::Collect %s does not exist\n",  path.c_str() ));	
		return -1;
	}
  }
  catch (const boost::filesystem::filesystem_error& ex)
  {
	ACE_ERROR ((LM_ERROR, "(%t) Error DirMoniter::Collect %s \n",  ex.what() ));	
    std::cout << ex.what() << '\n';
	return -1;
  }

  return 0;

}


DirMoniter::DirMoniter(std::string p)
{
	ACE_TRACE("DirMoniter::DirMoniter");
	path=p;
}


DirMoniter::~DirMoniter(void)
{
}

void DirMoniter::rename(std::string from, std::string to)
{
	ACE_TRACE("DirMoniter::rename");
	try
	{
		boost::filesystem3::path frmpath(from);
		boost::filesystem3::path topath(to);

		boost::filesystem3::rename(frmpath,topath);
		ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::rename %s %s \n",  from.c_str(), to.c_str() ));
	}
	catch(const boost::filesystem::filesystem_error& ex)
	{
		ACE_ERROR ((LM_ERROR, "(%t) Error DirMoniter::rename (%s) %s %s \n",  ex.what(), from.c_str(), to.c_str() ));	
		//std::cout << ex.what() << '\n';
	}
}

void DirMoniter::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%tDirMoniter::dump Path = %s\n" , this->path.c_str() ));	
    for (PATHVEC::const_iterator it (_pathvec.begin()); it != _pathvec.end(); ++it)
    {
		ACE_DEBUG ((LM_DEBUG, "(%t) DirMoniter::dump  file = %s \n",  it->string().c_str() ));	
    }
}
