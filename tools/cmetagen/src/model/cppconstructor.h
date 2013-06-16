#ifndef __CPPCONSTRUCTOR_H
#define __CPPCONSTRUCTOR_H

#include "cppitem.h"
#include "cppinvokable.h"
#include "cpptemplateitem.h"


class CppConstructor : public CppInvokable, public CppTemplateItem
{
public:
	virtual bool isTemplate() const { return this->getTemplateParamList()->getCount() > 0; }

protected:
	virtual ItemCategory getCategory() const { return icConstructor; }
};


#endif
