#ifndef __CPPINVOKABLE_H
#define __CPPINVOKABLE_H

#include "cppparam.h"
#include "cpptype.h"
#include "cppitem.h"


class CppInvokable : public CppItem
{
private:
	typedef CppItem super;

public:
	CppInvokable() : resultType(NULL) {}
	
	CppType * getResultType() const { return this->resultType; }
	void setResultType(CppType * resultType) { this->resultType = resultType; }
	bool hasResultType() const { return this->resultType != NULL; }
	
	CppParam * addParam() { return this->paramList.add(); }
	const CppParamList * getParamList() const { return &this->paramList; }
	
	bool isConst() const { return this->getFlags().has(ifConst); }
	void setConst(bool value) { this->getFlags().setByBool(ifConst, value); }
	
private:
	CppParamList paramList;
	CppType * resultType;
};


#endif
