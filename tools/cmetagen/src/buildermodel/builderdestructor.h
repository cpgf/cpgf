#ifndef __BUILDERDESTRUCTOR_H
#define __BUILDERDESTRUCTOR_H

#include "builderitem.h"


class BuilderDestructor : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderDestructor(const CppItem * cppItem);
	virtual ~BuilderDestructor();
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
