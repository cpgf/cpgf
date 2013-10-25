#ifndef CPGF_CPPCONTAINER_H
#define CPGF_CPPCONTAINER_H

#include "cppitem.h"

#include <string>
#include <vector>
#include <map>


namespace metagen {


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
	typedef std::map<std::string, int> ItemCountMap;
	
public:
	typedef std::vector<const CppItem *> ItemListType;
	typedef std::vector<const CppNamespace *> NamespaceListType;
	typedef std::vector<const CppClass *> ClassListType;
	typedef std::vector<const CppField *> FieldListType;
	typedef std::vector<const CppMethod *> MethodListType;
	typedef std::vector<const CppEnum *> EnumListType;
	typedef std::vector<const CppOperator *> OperatorListType;

public:
	explicit CppContainer(const clang::Decl * decl);
	virtual ~CppContainer();

	const ItemListType * getItemList() const { return &this->itemList; }
	const NamespaceListType * getNamespaceList() const { return &this->namespaceList; }
	const ClassListType * getClassList() const { return &this->classList; }
	const FieldListType * getFieldList() const { return &this->fieldList; }
	const MethodListType * getMethodList() const { return &this->methodList; }
	const EnumListType * getEnumList() const { return &this->enumList; }
	const OperatorListType * getOperatorList() const { return &this->operatorList; }
	
	int getSameNamedItemCount(const CppNamedItem * item) const;

	virtual bool isContainer() const { return true; }
	
	virtual void dump(std::ostream & os, int level) const;

protected:
	void addItem(CppItem * item);

	virtual void doAddItem(CppItem * item);
	
	template <typename T>
	void pushItem(std::vector<T> & itemList, CppItem * item) {
		item->setIndexInCategory((int)itemList.size());
		itemList.push_back(static_cast<T>(const_cast<const CppItem *>(item)));
	}

private:
	ItemListType itemList;
	NamespaceListType namespaceList;
	ClassListType classList;
	FieldListType fieldList;
	MethodListType methodList;
	EnumListType enumList;
	OperatorListType operatorList;
	ItemCountMap itemCount;
	
private:
	friend class ClangParserImplement;
};

std::string getCppContainerInstantiationName(const CppContainer * cppContainer);


} // namespace metagen


#endif
