#ifndef __CPPOPERATOR_H
#define __CPPOPERATOR_H

#include "cppitem.h"
#include "cppinvokable.h"
#include "cpptemplateitem.h"


class CppOperator : public CppInvokable, public CppTemplateItem
{
public:
	virtual bool isTemplate() const { return this->getTemplateParamList()->getCount() > 0; }

protected:
	virtual ItemCategory getCategory() const { return icOperator; }
};


#endif
