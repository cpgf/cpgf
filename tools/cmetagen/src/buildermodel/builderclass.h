#ifndef __BUILDERCLASS_H
#define __BUILDERCLASS_H

#include "buildercontainer.h"


namespace metagen {


class CppClass;

class BuilderClass : public BuilderContainer
{
private:
	typedef BuilderContainer super;
	
public:
	explicit BuilderClass(const CppItem * cppItem);
	virtual ~BuilderClass();
	
	const CppClass * getCppClass() const;
	
	bool shouldWrapClass() const { return true; }
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);
	
private:	
	void doWriteBaseClasses(BuilderWriter * writer);
	void doWriteAsNestedClass(BuilderWriter * writer);
	void doWriteInheritedOverridableMethods(BuilderWriter * writer);
};


} // namespace metagen


#endif
