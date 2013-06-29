#ifndef __CPPOPERATOR_H
#define __CPPOPERATOR_H

#include "cppitem.h"
#include "cppinvokable.h"


class CppOperator : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppOperator(const clang::Decl * decl) : super(decl) {}

protected:
	virtual ItemCategory getCategory() const { return icOperator; }
};


#endif
