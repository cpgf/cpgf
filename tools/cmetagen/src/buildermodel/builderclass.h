#ifndef __BUILDERCLASS_H
#define __BUILDERCLASS_H

#include "builderitem.h"


class CppClass;

class BuilderClass : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderClass(const CppItem * cppItem);
	virtual ~BuilderClass();
	
	const CppClass * getCppClass() const;
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
