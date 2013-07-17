#include "builderitem.h"
#include "builderwriter.h"
#include "model/cppcontainer.h"
#include "codewriter/codeblock.h"

#include "cpgf/gassert.h"

#include <string>

using namespace std;

namespace metagen {


BuilderItem::BuilderItem(const CppItem * cppItem)
	: cppItem(cppItem), project(project)
{
}

BuilderItem::~BuilderItem()
{
}

const CppItem * BuilderItem::getCppItem() const
{
	return this->cppItem;
}

void BuilderItem::setSkipBind(bool skip)
{
	this->flags.setByBool(bfSkipBind, skip);
}

bool BuilderItem::shouldSkipBind() const
{
	return this->flags.has(bfSkipBind);
}

bool BuilderItem::canBind() const
{
	return true;
}

void BuilderItem::checkBuilderItemCategory(ItemCategory category)
{
	GASSERT(this->cppItem->getCategory() == category);
}

std::string BuilderItem::getPolicyText() const
{
	return this->doGetPolicyText();
}

std::string BuilderItem::doGetPolicyText() const
{
	return "";
}

void BuilderItem::writeMetaData(BuilderWriter * writer)
{
	this->doWriteMetaData(writer);
}


} // namespace metagen
