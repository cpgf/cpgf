#ifndef __CPPINVOKABLE_H
#define __CPPINVOKABLE_H

#include "cppparam.h"
#include "cpptype.h"
#include "cppitem.h"


class CppInvokable : virtual public CppItem
{
public:
	CppInvokable() : resultType(NULL) {}
	
	CppType * getResultType() const { return this->resultType; }
	void setResultType(CppType * resultType) { this->resultType = resultType; }
	bool hasResultType() const { return this->resultType != NULL; }
	
	CppParam * addParam() { return this->paramList.add(); }
	int getParamCount() const { return this->paramList.getCount(); }
	CppParam * getParam(int i) const { return this->paramList.get(i); }
	
	bool isConst() const { return this->getFlags().has(ifConst); }
	void setConst(bool value) { this->getFlags().setByBool(ifConst, value); }
	
private:
	CppParamList paramList;
	CppType * resultType;
};


#endif
