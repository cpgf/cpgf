#ifndef __BUILDERNAMESPACE_H
#define __BUILDERNAMESPACE_H

#include "builderitem.h"


class BuilderNamespace : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderNamespace(const CppItem * cppItem);
	virtual ~BuilderNamespace();
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
