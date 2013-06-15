#ifndef __BUILDERITEM_H
#define __BUILDERITEM_H

#include "model/cppitem.h"


struct CppPairWriter;
class CodeBlock;

class BuilderItem
{
public:
	explicit BuilderItem(const CppItem * cppItem);
	virtual ~BuilderItem();

	const CppItem * getCppItem() const { return this->cppItem; }

	void writeMetaData(CppPairWriter * writer);
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer) = 0;
	
	void checkBuilderItemCategory(ItemCategory category);
	
	CodeBlock * getContainerCodeBlock(CodeBlock * codeBlock);
	CodeBlock * getHeaderCodeBlock(CodeBlock * codeBlock);
	CodeBlock * getBodyCodeBlock(CodeBlock * codeBlock);
	CodeBlock * getFooterCodeBlock(CodeBlock * codeBlock);
	CodeBlock * getMetaDataCodeBlock(CodeBlock * codeBlock);

private:
	const CppItem * cppItem;
};



#endif
