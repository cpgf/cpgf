#ifndef __BUILDERITEM_H
#define __BUILDERITEM_H

#include "model/cppitem.h"

struct CppPairWriter;

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

private:
	const CppItem * cppItem;
};



#endif
