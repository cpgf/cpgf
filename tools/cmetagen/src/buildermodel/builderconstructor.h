#ifndef __BUILDERCONSTRUCTOR_H
#define __BUILDERCONSTRUCTOR_H

#include "builderitem.h"


class BuilderConstructor : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderConstructor(const CppItem * cppItem);
	virtual ~BuilderConstructor();
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer);
};

#endif
