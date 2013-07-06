#ifndef __BUILDERENUM_H
#define __BUILDERENUM_H

#include "builderitem.h"


class CppEnum;

class BuilderEnum : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderEnum(const CppItem * cppItem);
	virtual ~BuilderEnum();
	
	const CppEnum * getCppEnum() const;
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
