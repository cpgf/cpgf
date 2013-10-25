#ifndef CPGF_BUILDEROPERATOR_H
#define CPGF_BUILDEROPERATOR_H

#include "builderinvokable.h"

namespace metagen {


class CppOperator;

class BuilderOperator : public BuilderInvokable
{
private:
	typedef BuilderInvokable super;
	
public:
	explicit BuilderOperator(const CppItem * cppItem);
	virtual ~BuilderOperator();
	
	const CppOperator * getCppOperator() const;
	
protected:
	virtual void doWriteMetaData(BuilderWriter * writer);
	
private:
	size_t calculateReflectionParamCount() const;
	void doWriteReflection(BuilderWriter * writer);
	void doWriteWrapper(BuilderWriter * writer);
	bool shouldGenerateWrapper() const;
};


} // namespace metagen


#endif
