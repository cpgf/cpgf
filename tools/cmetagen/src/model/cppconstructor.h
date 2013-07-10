#ifndef __CPPCONSTRUCTOR_H
#define __CPPCONSTRUCTOR_H

#include "cppitem.h"
#include "cppinvokable.h"

namespace metagen {


class CppConstructor : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppConstructor(const clang::Decl * decl);

	bool isCopyConstructor() const;
	bool isImplicitTypeConverter() const;

protected:
	virtual ItemCategory getCategory() const { return icConstructor; }
};


} // namespace metagen


#endif
