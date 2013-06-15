#include "builderitem.h"
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

CodeBlock * BuilderItem::getContainerCodeBlock(CodeBlock * codeBlock)
{
	string blockName;
	const CppItem * cppItem = this->getCppItem();

	if(cppItem->getParent() == NULL || cppItem->isContainer()) {
		blockName = cppItem->getQualifiedName();
	}
	else {
		blockName = cppItem->getParent()->getQualifiedName();
	}
	return codeBlock->getNamedBlock(blockName);
}

CodeBlock * BuilderItem::getHeaderCodeBlock(CodeBlock * codeBlock)
{
	this->getBodyCodeBlock(codeBlock);
	return this->getContainerCodeBlock(codeBlock)->getNamedBlock("body");
}

CodeBlock * BuilderItem::getBodyCodeBlock(CodeBlock * codeBlock)
{
	this->getFooterCodeBlock(codeBlock);
	return this->getContainerCodeBlock(codeBlock)->getNamedBlock("header");
}

CodeBlock * BuilderItem::getFooterCodeBlock(CodeBlock * codeBlock)
{
	return this->getContainerCodeBlock(codeBlock)->getNamedBlock("footer");
}

CodeBlock * BuilderItem::getMetaDataCodeBlock(CodeBlock * codeBlock)
{
	return this->getBodyCodeBlock(codeBlock)->getNamedBlock(ItemNames[cppItem->getCategory()]);
}

void BuilderItem::writeMetaData(CppPairWriter * writer)
{
	this->doWriteMetaData(writer);
}

