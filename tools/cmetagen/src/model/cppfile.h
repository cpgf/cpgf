#ifndef __CPPFILE_H
#define __CPPFILE_H

#include "cppcontainer.h"

namespace metagen {


class CppFile : public CppContainer
{
private:
	typedef CppContainer super;

public:
	CppFile(const clang::Decl * decl);
	~CppFile();

protected:	
	virtual ItemCategory getCategory() const { return icFile; }
};


} // namespace metagen


#endif
