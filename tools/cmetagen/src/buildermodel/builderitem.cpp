#include "builderitem.h"

#include "cpgf/gassert.h"


BuilderItem::BuilderItem(const CppItem * cppItem)
	: cppItem(cppItem)
{
}

BuilderItem::~BuilderItem()
{
}

void BuilderItem::checkBuilderItemCategory(ItemCategory category)
{
	GASSERT(this->cppItem->getCategory() == category);
}

void BuilderItem::writeMetaData(CppPairWriter * writer)
{
	this->doWriteMetaData(writer);
}

