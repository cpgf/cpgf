#ifndef __CPPCONTEXT_H
#define __CPPCONTEXT_H

#include "cppitem.h"

#include <string>
#include <vector>


class CppNamespace;
class CppClass;
class CppField;
class CppMethod;
class CppEnum;
class CppOperator;

// A context, from C++ side, is a namespace, class, or function, that can contain other nested stuff.
// A context in metagen, is only namespace or class, because the nested stuff in a function can't be accessed from outside.
class CppContext : public CppItem
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
	CppContext();
	virtual ~CppContext();

	void addItem(CppItem * item);

	NamespaceListType & getNamespaceList() { return this->namespaceList; }
	ClassListType & getClassList() { return this->classList; }
	FieldListType & getFieldList() { return this->fieldList; }
	MethodListType & getMethodList() { return this->methodList; }
	EnumListType & getEnumList() { return this->enumList; }
	OperatorListType & getOperatorList() { return this->operatorList; }

	virtual void dump(std::ostream & os, int level);

protected:
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
