#ifndef __CPPFILE_H
#define __CPPFILE_H

#include "cppcontainer.h"

#include "Poco/Path.h"

namespace metagen {


class CppFile : public CppContainer
{
private:
	typedef CppContainer super;

public:
	CppFile(const char * fullFileName, const clang::Decl * decl);
	~CppFile();

	void prepare() const;
	
	const Poco::Path & getFullFileName() const { return this->fullFileName; }

protected:	
	virtual ItemCategory getCategory() const { return icFile; }

private:
	Poco::Path fullFileName;
};


} // namespace metagen


#endif
