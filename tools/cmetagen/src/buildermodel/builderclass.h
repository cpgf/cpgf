#ifndef __BUILDERCLASS_H
#define __BUILDERCLASS_H

#include "builderitem.h"


class BuilderClass : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderClass(const CppItem * cppItem);
	virtual ~BuilderClass();
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer);
};

#endif
