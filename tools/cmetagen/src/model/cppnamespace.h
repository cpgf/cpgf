#ifndef __CPPNAMESPACE_H
#define __CPPNAMESPACE_H

#include "cppcontext.h"


class CppNamespace : public CppContext
{
private:
	typedef CppContext super;
	typedef std::vector<CppItem *> ItemListType;

public:

protected:
	virtual ItemCategory getCategory() const { return icNamespace; }

};


#endif

