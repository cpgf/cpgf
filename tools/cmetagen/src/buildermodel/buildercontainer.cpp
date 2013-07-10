#include "buildercontainer.h"
#include "model/cppcontainer.h"
#include "codewriter/cppwriter.h"

#include <algorithm>

namespace metagen {


BuilderContainer::BuilderContainer(const CppItem * cppItem)
	: super(cppItem)
{
}

BuilderContainer::~BuilderContainer()
{
}

const CppContainer * BuilderContainer::getCppContainer() const
{
	return static_cast<const CppContainer *>(this->getCppItem());
}

void BuilderContainer::addItem(BuilderItem * item)
{
//	this->itemList.push_back(item);
	item->setParent(this);
}

bool compareItem(BuilderItem * itemA, BuilderItem * itemB)
{
	const CppItem * cppItemA = itemA->getCppItem();
	const CppItem * cppItemB = itemB->getCppItem();

	if(cppItemA->getParent() != cppItemB->getParent()) {
		return cppItemA->getParent()->getQualifiedName() < cppItemB->getParent()->getQualifiedName();
	}

	return cppItemA->getCategory() < cppItemB->getCategory();
}

void BuilderContainer::sortItems()
{
	std::sort(this->itemList.begin(), this->itemList.end(), &compareItem);
}


} // namespace metagen
