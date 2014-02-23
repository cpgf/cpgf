#include "buildercontainer.h"
#include "model/cppcontainer.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"

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
	this->itemList.push_back(item);
	item->setParent(this);
}


} // namespace metagen
