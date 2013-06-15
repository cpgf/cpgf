#ifndef __CPPFILE_H
#define __CPPFILE_H

#include "cppcontainer.h"

#include "Poco/Path.h"


class CppFile : public CppContainer
{
private:
	typedef CppContainer super;

public:
	explicit CppFile(const char * fullFileName);
	~CppFile();

protected:	
	virtual ItemCategory getCategory() const { return icFile; }

private:
	Poco::Path fullFileName;
};


#endif
