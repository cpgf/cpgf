#include "buildermethod.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"


BuilderMethod::BuilderMethod(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icMethod);
}

BuilderMethod::~BuilderMethod()
{
}

void BuilderMethod::doWriteMetaData(BuilderFileWriter * writer)
{
}

