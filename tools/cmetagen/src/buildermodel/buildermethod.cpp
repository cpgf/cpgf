#include "buildermethod.h"
#include "codewriter/cppwriter.h"


BuilderMethod::BuilderMethod(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icMethod);
}

BuilderMethod::~BuilderMethod()
{
}

void BuilderMethod::doWriteMetaData(CppPairWriter * writer)
{
}

