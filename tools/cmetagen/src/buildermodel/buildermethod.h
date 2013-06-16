#ifndef __BUILDERMETHOD_H
#define __BUILDERMETHOD_H

#include "builderitem.h"


class BuilderMethod : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderMethod(const CppItem * cppItem);
	virtual ~BuilderMethod();
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
