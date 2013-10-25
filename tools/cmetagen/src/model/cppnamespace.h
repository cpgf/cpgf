#ifndef CPGF_CPPNAMESPACE_H
#define CPGF_CPPNAMESPACE_H

#include "cppcontainer.h"

namespace metagen {


class CppNamespace : public CppContainer
{
private:
	typedef CppContainer super;
	typedef std::vector<CppItem *> ItemListType;

public:
	explicit CppNamespace(clang::Decl * decl) : super(decl) {}

	bool isAnonymous() const;

protected:
	virtual ItemCategory getCategory() const { return icNamespace; }

};


} // namespace metagen


#endif

