#ifndef __BUILDERCLASS_H
#define __BUILDERCLASS_H

#include "buildercontainer.h"


class CppClass;

class BuilderClass : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	
public:
	explicit BuilderClass(const CppItem * cppItem);
	virtual ~BuilderClass();
	
	const CppClass * getCppClass() const;
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
	
private:	
	void doWriteBaseClasses(BuilderFileWriter * writer);
	void doWriteAsNestedClass(BuilderFileWriter * writer);
};

#endif
