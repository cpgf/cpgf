#ifndef CPGF_BUILDERCONSTRUCTOR_H
#define CPGF_BUILDERCONSTRUCTOR_H

#include "builderinvokable.h"

namespace metagen {


class CppConstructor;

class BuilderConstructor : public BuilderInvokable
{
private:
	typedef BuilderInvokable super;
	
public:
	explicit BuilderConstructor(const CppItem * cppItem);
	virtual ~BuilderConstructor();
	
	const CppConstructor * getCppConstructor() const;
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);

private:
	void doWriterReflection(BuilderWriter * writer);
	void doWriterReflectionCode(BuilderWriter * writer, CodeBlock * codeBlock);
	void doWriterClassWrapper(BuilderWriter * writer);
	void doWriterClassWrapperReflection(BuilderWriter * writer);
};


} // namespace metagen


#endif
