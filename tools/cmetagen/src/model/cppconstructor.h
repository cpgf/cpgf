#ifndef CPGF_CPPCONSTRUCTOR_H
#define CPGF_CPPCONSTRUCTOR_H

#include "cppitem.h"
#include "cppinvokable.h"

namespace metagen {


class CppConstructor : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppConstructor(const clang::Decl * decl);

	bool isDefaultConstructor() const;
	bool isCopyConstructor() const;
	bool isImplicitTypeConverter() const;

protected:
	virtual ItemCategory getCategory() const { return icConstructor; }
};


} // namespace metagen


#endif
