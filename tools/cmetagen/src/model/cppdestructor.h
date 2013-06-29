#ifndef __CPPDESTRUCTOR_H
#define __CPPDESTRUCTOR_H

#include "cppitem.h"


class CppDestructor : public CppItem
{
private:
	typedef CppItem super;

public:
	explicit CppDestructor(clang::Decl * decl) : super(decl) {}

protected:
	virtual ItemCategory getCategory() const { return icDestructor; }
};


#endif
