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
	
private:
	void doWriterReflection(BuilderWriter * writer);
	void doWriterReflectionCode(BuilderWriter * writer, CodeBlock * codeBlock, const std::string & methodName);
	void doWriterDefaultParameterReflection(CodeBlock * codeBlock);

	void doWriterClassWrapper(BuilderWriter * writer);
	void doWriterClassWrapperMethodBody(CodeBlock * codeBlock);
	void doWriterClassWrapperCallSuperMethod(CodeBlock * codeBlock);
	void doWriterClassWrapperReflection(BuilderWriter * writer);
};


} // namespace metagen


#endif
