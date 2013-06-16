#ifndef __BUILDERITEM_H
#define __BUILDERITEM_H

#include "model/cppitem.h"


class BuilderFileWriter;
class CodeBlock;

class BuilderItem
{
public:
	explicit BuilderItem(const CppItem * cppItem);
	virtual ~BuilderItem();

	const CppItem * getCppItem() const { return this->cppItem; }

	void writeMetaData(BuilderFileWriter * writer);
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer) = 0;
	
	void checkBuilderItemCategory(ItemCategory category);
	
private:
	const CppItem * cppItem;
};



#endif
