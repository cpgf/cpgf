#ifndef __FILEINFO_H
#define __FILEINFO_H

#include "cpgf/gscopedptr.h"

#include "boost/filesystem.hpp"

class CppNamespace;

class FileInfo
{
public:
	explicit FileInfo(const char * fullFileName);
	~FileInfo();
	
	CppNamespace * getGlobalNamespace() const { return this->globalNamespace.get(); }

private:
	boost::filesystem::path fullFileName;
	cpgf::GScopedPointer<CppNamespace> globalNamespace;
};


#endif
