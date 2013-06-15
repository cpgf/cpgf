#include "cppcontainer.h"
#include "cppnamespace.h"
#include "cppclass.h"
#include "cppfield.h"
#include "cppmethod.h"
#include "cppenum.h"
#include "cppoperator.h"

#include "util.h"

#include "cpgf/gassert.h"


CppContainer::CppContainer()
	: super()
{
}

CppContainer::~CppContainer()
{
}

void CppContainer::addItem(CppItem * item)
{
	this->itemList.push_back(item);
	this->doAddItem(item);
	item->setParent(this);
}

void CppContainer::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icNamespace:
			this->namespaceList.push_back(static_cast<CppNamespace *>(item));
			break;

		case icClass:
			this->classList.push_back(static_cast<CppClass *>(item));
			break;

		case icField:
			this->fieldList.push_back(static_cast<CppField *>(item));
			break;
			
		case icMethod:
			this->methodList.push_back(static_cast<CppMethod *>(item));
			break;

		case icEnum:
			this->enumList.push_back(static_cast<CppEnum *>(item));
			break;

		case icOperator:
			this->operatorList.push_back(static_cast<CppOperator *>(item));
			break;

		default:
			GASSERT(false);
			break;
	}
}

template <typename T>
void dumpList(T & itemList, std::ostream & os, int level)
{
	for(typename T::iterator it = itemList.begin(); it != itemList.end(); ++it) {
		(*it)->dump(os, level);
	}
}

void CppContainer::dump(std::ostream & os, int level)
{
	int newLevel = level + 1;

	dumpList(this->namespaceList, os, newLevel);
	dumpList(this->classList, os, newLevel);
	dumpList(this->fieldList, os, newLevel);
	dumpList(this->methodList, os, newLevel);
	dumpList(this->enumList, os, newLevel);
	dumpList(this->operatorList, os, newLevel);
}

