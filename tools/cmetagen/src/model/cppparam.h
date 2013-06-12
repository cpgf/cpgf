#ifndef __CPPPARAM_H
#define __CPPPARAM_H

#include "cpptype.h"

#include <string>
#include <vector>

class CppParam
{
public:
	CppParam();

	const std::string & getName() const { return this->name; }
	void setName(const std::string & name) { this->name = name; }
	
	CppType * getType() const { return this->type; }
	void setType(CppType * type) { this->type = type; }

	const std::string & getDefaultValue() const { return this->defaultValue; }
	void setDefaultValue(const std::string & defaultValue) { this->defaultValue = defaultValue; }
	bool hasDefaultValue() const { return ! this->defaultValue.empty(); }
	
private:
	std::string name;
	CppType * type;
	std::string defaultValue;
};


class CppParamList
{
private:
	typedef std::vector<CppParam *> ParamListType;
	
public:
	CppParamList();
	~CppParamList();
	
	CppParam * add();
	int getCount() const;
	CppParam * get(int i) const;

private:
	ParamListType paramList;
};



#endif
