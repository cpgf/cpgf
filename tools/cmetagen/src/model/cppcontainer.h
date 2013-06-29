#ifndef __CPPCONTAINER_H
#define __CPPCONTAINER_H

#include "cppitem.h"

#include <string>
#include <vector>


class CppNamespace;
class CppClass;
class CppField;
class CppMethod;
class CppEnum;
class CppOperator;

// A container, from C++ side, is a namespace, class, or function, that can contain other nested stuff.
// A container in metagen, is only namespace or class, because the nested stuff in a function can't be accessed from outside.
class CppContainer : public CppNamedItem
{
private:
	typedef CppNamedItem super;
	
public:
	typedef std::vector<CppItem *> ItemListType;
	typedef std::vector<CppNamespace *> NamespaceListType;
	typedef std::vector<CppClass *> ClassListType;
	typedef std::vector<CppField *> FieldListType;
	typedef std::vector<CppMethod *> MethodListType;
	typedef std::vector<CppEnum *> EnumListType;
	typedef std::vector<CppOperator *> OperatorListType;

public:
	explicit CppContainer(clang::Decl * decl);
	virtual ~CppContainer();

	void addItem(CppItem * item);

	const ItemListType * getItemList() const { return &this->itemList; }
	const NamespaceListType * getNamespaceList() const { return &this->namespaceList; }
	const ClassListType * getClassList() const { return &this->classList; }
	const FieldListType * getFieldList() const { return &this->fieldList; }
	const MethodListType * getMethodList() const { return &this->methodList; }
	const EnumListType * getEnumList() const { return &this->enumList; }
	const OperatorListType * getOperatorList() const { return &this->operatorList; }

	virtual void dump(std::ostream & os, int level);

protected:
	virtual void doAddItem(CppItem * item);

private:
	ItemListType itemList;
	NamespaceListType namespaceList;
	ClassListType classList;
	FieldListType fieldList;
	MethodListType methodList;
	EnumListType enumList;
	OperatorListType operatorList;
};



#endif
