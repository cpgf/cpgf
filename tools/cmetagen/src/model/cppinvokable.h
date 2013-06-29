#ifndef __CPPINVOKABLE_H
#define __CPPINVOKABLE_H

#include "cpptype.h"
#include "cppitem.h"


class CppInvokable : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	CppInvokable(clang::Decl * decl);
	
	bool isStatic() const;
	bool isConst() const;
	bool isVariadic() const;
	bool isTemplate() const;

	size_t getArity() const;
};


#endif
