#ifndef __BUILDERFIELD_H
#define __BUILDERFIELD_H

#include "builderitem.h"


class BuilderField : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderField(const CppItem * cppItem);
	virtual ~BuilderField();
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
