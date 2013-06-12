#ifndef __CPPNAMESPACE_H
#define __CPPNAMESPACE_H

#include "cppitem.h"

#include <string>
#include <vector>


class CppNamespace;
class CppClass;
class CppField;
class CppMethod;
class CppEnum;
class CppOperator;

class CppNamespace : public CppItem
{
private:
	typedef CppItem super;
	typedef std::vector<CppItem *> ItemListType;

public:
	typedef std::vector<CppNamespace *> NamespaceListType;
	typedef std::vector<CppClass *> ClassListType;
	typedef std::vector<CppField *> FieldListType;
	typedef std::vector<CppMethod *> MethodListType;
	typedef std::vector<CppEnum *> EnumListType;
	typedef std::vector<CppOperator *> OperatorListType;

public:
	CppNamespace();
	virtual ~CppNamespace();

	void addItem(CppItem * item);

	NamespaceListType & getNamespaceList() { return this->namespaceList; }
	ClassListType & getClassList() { return this->classList; }
	FieldListType & getFieldList() { return this->fieldList; }
	MethodListType & getMethodList() { return this->methodList; }
	EnumListType & getEnumList() { return this->enumList; }
	OperatorListType & getOperatorList() { return this->operatorList; }

	virtual void dump(std::ostream & os, int level);

protected:
	virtual ItemCategory getCategory() const { return icNamespace; }

	virtual void doAddItem(CppItem * item);

private:
	NamespaceListType namespaceList;
	ClassListType classList;
	FieldListType fieldList;
	MethodListType methodList;
	EnumListType enumList;
	OperatorListType operatorList;
};


#endif

