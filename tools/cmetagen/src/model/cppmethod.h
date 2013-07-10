#ifndef __CPPMETHOD_H
#define __CPPMETHOD_H

#include "cppitem.h"
#include "cppinvokable.h"

namespace metagen {


class CppMethod : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppMethod(const clang::Decl * decl);
	
protected:
	virtual ItemCategory getCategory() const { return icMethod; }
};


} // namespace metagen


#endif
