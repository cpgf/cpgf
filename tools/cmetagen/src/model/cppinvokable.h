#ifndef __CPPINVOKABLE_H
#define __CPPINVOKABLE_H

#include "cppitem.h"


class CppInvokable : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppInvokable(const clang::Decl * decl);
	
	bool isStatic() const;
	bool isConst() const;
	bool isVariadic() const;
	bool isTemplate() const;

	size_t getArity() const;

	std::string getPointeredType() const;

};


#endif
