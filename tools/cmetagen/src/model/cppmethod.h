#ifndef __CPPMETHOD_H
#define __CPPMETHOD_H

#include "cppitem.h"
#include "cppinvokable.h"
#include "cpptemplateitem.h"


class CppMethod : public CppInvokable, public CppTemplateItem
{
protected:
	virtual ItemCategory getCategory() const { return icMethod; }
};


#endif
