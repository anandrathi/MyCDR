#pragma once

#ifndef AR_DIRMONITER_H
#define AR_DIRMONITER_H

#include <string>
#include <vector>
#include <set>
#include "MyCDRBASE.h"
#include <boost/filesystem/path.hpp>

class DirMoniter : public MyCDRBASE
{
	std::string path;
public:
    typedef std::set<boost::filesystem::path> PATHSET;             // store paths,
    typedef std::vector<boost::filesystem::path> PATHVEC;             // store paths,
	//PATHSET _pathset;                                // so we can sort them later
	PATHVEC _pathvec;                                // so we can sort them later
	DirMoniter(std::string );
	~DirMoniter(void);
	int Collect();
	void dump(void);
	static void rename(std::string , std::string );
	static bool testPath(std::string& );
};


#endif //AR_DIRMONITER_H