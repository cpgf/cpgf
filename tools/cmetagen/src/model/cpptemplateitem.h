#ifndef __CPPTEMPLATEITEM_H
#define __CPPTEMPLATEITEM_H

#include "cppitem.h"
#include "cppparam.h"


class CppTemplateItem
{
public:
	CppParam * addTemplateParam() { return this->paramList.add(); }
	const CppParamList * getTemplateParamList() const { return &this->paramList; }
	
private:
	CppParamList paramList;	
};


#endif
