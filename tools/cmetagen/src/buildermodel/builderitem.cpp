#include "builderitem.h"
#include "buildercontainer.h"
#include "builderwriter.h"
#include "model/cppcontainer.h"
#include "codewriter/codeblock.h"

#include "cpgf/gassert.h"

#include <string>

using namespace std;

namespace metagen {


BuilderItem::BuilderItem(const CppItem * cppItem)
	: cppItem(cppItem), project(project), parent(NULL)
{
}

BuilderItem::~BuilderItem()
{
}

const CppItem * BuilderItem::getCppItem() const
{
	return this->cppItem;
}

void BuilderItem::checkBuilderItemCategory(ItemCategory category)
{
	(void)category;
	GASSERT(this->cppItem->getCategory() == category);
}

void BuilderItem::writeMetaData(BuilderWriter * writer)
{
	if(this->shouldSkipBind()) {
		return;
	}
	
	if(! this->canBind()) {
		return;
	}
	
	this->doWriteMetaData(writer);
}

void BuilderItem::setSkipBind(bool skip)
{
	this->skipBind.set(skip);
}

bool BuilderItem::shouldSkipBind() const
{
	return this->skipBind.get() || (this->getParent() != NULL && this->getParent()->shouldSkipBind());
}

bool BuilderItem::canBind() const
{
	return isVisibilityAllowed(this->getCppItem()->getVisibility(), this->getProject())
		&& this->doCanBind();
}

bool BuilderItem::doCanBind() const
{
	return true;
}

void BuilderItem::setWrapClass(bool wrap)
{
	this->wrapClass.set(wrap);
}

bool BuilderItem::shouldWrapClass() const
{
	if(this->getParent() != NULL
		&& this->getParent()->getCppItem()->isClass()
		&& ! this->getParent()->shouldSkipBind()) {
		return false;
	}

	// For class, the default is false as the user must enable it explicitly.
	// For method, the default is true as the user must disable it explicitly.
	if(this->getCppItem()->isClass()) {
		return this->wrapClass.get(false);
	}
	else {
		return this->wrapClass.get(true);
	}
}


} // namespace metagen
