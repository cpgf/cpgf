#include "builderitem.h"
#include "builderfilewriter.h"
#include "model/cppcontainer.h"
#include "codewriter/codeblock.h"

#include "cpgf/gassert.h"

#include <string>

using namespace std;


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

void BuilderItem::writeMetaData(BuilderFileWriter * writer)
{
	writer->requireItemConainerFunction(this->getCppItem());

	this->doWriteMetaData(writer);
}

