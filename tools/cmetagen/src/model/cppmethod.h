#ifndef CPGF_CPPMETHOD_H
#define CPGF_CPPMETHOD_H

#include "cppitem.h"
#include "cppinvokable.h"

namespace metagen {


class CppMethod : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppMethod(const clang::Decl * decl);
	
	bool isArityUnique() const;
	
protected:
	virtual ItemCategory getCategory() const { return icMethod; }
};


} // namespace metagen


#endif
