#ifndef __CPPNAMESPACE_H
#define __CPPNAMESPACE_H

#include "cppcontainer.h"


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


#endif

