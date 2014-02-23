#include "cppcontainer.h"
#include "cppnamespace.h"
#include "cppclass.h"
#include "cppfield.h"
#include "cppmethod.h"
#include "cppenum.h"
#include "cppoperator.h"

#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

namespace metagen {


CppContainer::CppContainer(const clang::Decl * decl)
	: super(decl)
{
}

CppContainer::~CppContainer()
{
}

void CppContainer::addItem(CppItem * item)
{
	this->itemList.push_back(item);
	if(item->isNamed()) {
		++this->itemCount[static_cast<CppNamedItem *>(item)->getQualifiedName()];
	}
	this->doAddItem(item);
	item->setParent(this);
}

void CppContainer::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icNamespace:
			this->pushItem(this->namespaceList, item);
			break;

		case icClass:
			this->pushItem(this->classList, item);
			break;

		case icField:
			this->pushItem(this->fieldList, item);
			break;
			
		case icMethod:
			this->pushItem(this->methodList, item);
			break;

		case icEnum:
			this->pushItem(this->enumList, item);
			break;

		case icOperator:
			this->pushItem(this->operatorList, item);
			break;

		default:
			GASSERT(false);
			break;
	}
}

int CppContainer::getSameNamedItemCount(const CppNamedItem * item) const
{
	ItemCountMap::const_iterator it = this->itemCount.find(item->getQualifiedName());
	if(it != this->itemCount.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

template <typename T>
void dumpList(T & itemList, std::ostream & os, int level)
{
	for(typename T::const_iterator it = itemList.begin(); it != itemList.end(); ++it) {
		(*it)->dump(os, level);
	}
}

void CppContainer::dump(std::ostream & os, int level) const
{
	int newLevel = level + 1;

	dumpList(this->namespaceList, os, newLevel);
	dumpList(this->classList, os, newLevel);
	dumpList(this->fieldList, os, newLevel);
	dumpList(this->methodList, os, newLevel);
	dumpList(this->enumList, os, newLevel);
	dumpList(this->operatorList, os, newLevel);
}

std::string getCppContainerInstantiationName(const CppContainer * cppContainer)
{
	if(cppContainer->isClass()) {
		const CppClass * cppClass = static_cast<const CppClass *>(cppContainer);
		return cppClass->getTextOfQualifedInstantiationName();
		//if(cppClass->isTemplate()) {
		//	return Poco::format("%s<%s >",
		//		cppClass->getQualifiedName(),
		//		cppClass->getTextOfChainedTemplateParamList(itoWithArgName)
		//	);
		//}
	}
	return cppContainer->getQualifiedName();
}


} // namespace metagen
