#ifndef __CPPMETHOD_H
#define __CPPMETHOD_H

#include "cppitem.h"
#include "cppinvokable.h"


class CppMethod : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppMethod(clang::Decl * decl) : super(decl) {}

protected:
	virtual ItemCategory getCategory() const { return icMethod; }
};


#endif
