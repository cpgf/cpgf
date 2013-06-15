#include "cppparam.h"

#include "util.h"

CppParam::CppParam()
	: name(), type(NULL)
{
}


CppParamList::CppParamList()
{
}

CppParamList::~CppParamList()
{
	clearPointerContainer(this->paramList);
}

CppParam * CppParamList::add()
{
	CppParam * param = new CppParam;
	this->paramList.push_back(param);
	return param;
}

int CppParamList::getCount() const
{
	return (int)(this->paramList.size());
}

CppParam * CppParamList::get(int i) const
{
	return this->paramList[i];
}

