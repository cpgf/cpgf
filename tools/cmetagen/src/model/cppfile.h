#ifndef __CPPFILE_H
#define __CPPFILE_H

#include "cppcontext.h"

#include "Poco/Path.h"


class CppFile : public CppContext
{
private:
	typedef CppContext super;

public:
	explicit CppFile(const char * fullFileName);
	~CppFile();

protected:	
	virtual ItemCategory getCategory() const { return icFile; }

private:
	Poco::Path fullFileName;
};


#endif
