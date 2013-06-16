#include "builderconstructor.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"


BuilderConstructor::BuilderConstructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icConstructor);
}

BuilderConstructor::~BuilderConstructor()
{
}

void BuilderConstructor::doWriteMetaData(BuilderFileWriter * writer)
{
}

