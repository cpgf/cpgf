#ifndef __CPPDESTRUCTOR_H
#define __CPPDESTRUCTOR_H

#include "cppitem.h"

namespace metagen {


class CppDestructor : public CppItem
{
private:
	typedef CppItem super;

public:
	explicit CppDestructor(const clang::Decl * decl) : super(decl) {}

protected:
	virtual ItemCategory getCategory() const { return icDestructor; }
};


} // namespace metagen


#endif
