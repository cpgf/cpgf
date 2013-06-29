#ifndef __CPPCONSTRUCTOR_H
#define __CPPCONSTRUCTOR_H

#include "cppitem.h"
#include "cppinvokable.h"


class CppConstructor : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppConstructor(const clang::Decl * decl) : super(decl) {}

protected:
	virtual ItemCategory getCategory() const { return icConstructor; }
};


#endif
