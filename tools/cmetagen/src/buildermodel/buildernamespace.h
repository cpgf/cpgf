#ifndef __BUILDERNAMESPACE_H
#define __BUILDERNAMESPACE_H

#include "builderitem.h"


class CppNamespace;

class BuilderNamespace : public BuilderItem
{
private:
	typedef BuilderItem super;
	
public:
	explicit BuilderNamespace(const CppItem * cppItem);
	virtual ~BuilderNamespace();

	const CppNamespace * getCppNamespace() const;
		
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
