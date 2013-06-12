#ifndef __CPPTEMPLATEITEM_H
#define __CPPTEMPLATEITEM_H

#include "cppitem.h"
#include "cppparam.h"


class CppTemplateItem
{
public:
	CppParam * addTemplateParam() { return this->paramList.add(); }
	int getTemplateParamCount() const { return this->paramList.getCount(); }
	CppParam * getTemplateParam(int i) const { return this->paramList.get(i); }
	
private:
	CppParamList paramList;	
};


#endif
