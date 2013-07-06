#ifndef __BUILDERMETHOD_H
#define __BUILDERMETHOD_H

#include "builderinvokable.h"


class CppMethod;

class BuilderMethod : public BuilderInvokable
{
private:
	typedef BuilderInvokable super;
	
public:
	explicit BuilderMethod(const CppItem * cppItem);
	virtual ~BuilderMethod();
	
	const CppMethod * getCppMethod() const;
	
protected:
	virtual void doWriteMetaData(BuilderFileWriter * writer);
};

#endif
