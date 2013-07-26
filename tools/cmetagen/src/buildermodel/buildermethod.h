#ifndef __BUILDERMETHOD_H
#define __BUILDERMETHOD_H

#include "builderinvokable.h"

namespace metagen {


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
	virtual void doWriteMetaData(BuilderWriter * writer);
};

void writeMethodClassWrapper(const CppMethod * cppMethod, BuilderWriter * writer, const CppContainer * container);


} // namespace metagen


#endif
