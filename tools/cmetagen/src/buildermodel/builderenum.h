#ifndef __BUILDERENUM_H
#define __BUILDERENUM_H

#include "builderitem.h"


class BuilderEnum : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderEnum(const CppItem * cppItem);
	virtual ~BuilderEnum();
	
protected:
	virtual void doWriteMetaData(CppPairWriter * writer);
};

#endif
