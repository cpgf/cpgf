#ifndef __CPPNAMESPACE_H
#define __CPPNAMESPACE_H

#include "cppcontainer.h"


class CppNamespace : public CppContainer
{
private:
	typedef CppContainer super;
	typedef std::vector<CppItem *> ItemListType;

public:

protected:
	virtual ItemCategory getCategory() const { return icNamespace; }

};


#endif

