#ifndef __CPPCONSTRUCTOR_H
#define __CPPCONSTRUCTOR_H

#include "cppitem.h"
#include "cppinvokable.h"
#include "cpptemplateitem.h"


class CppConstructor : virtual public CppItem, public CppInvokable, public CppTemplateItem
{
protected:
	virtual ItemCategory getCategory() const { return icConstructor; }
};


#endif
