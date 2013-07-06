#ifndef __BUILDEROPERATOR_H
#define __BUILDEROPERATOR_H

#include "builderinvokable.h"


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
	virtual void doWriteMetaData(BuilderFileWriter * writer);
	
private:
	size_t calculateReflectionParamCount() const;
	void doWriteReflection(BuilderFileWriter * writer);
	void doWriteWrapper(BuilderFileWriter * writer);
};

#endif
