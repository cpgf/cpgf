#include "builderconstructor.h"
#include "codewriter/cppwriter.h"


BuilderConstructor::BuilderConstructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icConstructor);
}

BuilderConstructor::~BuilderConstructor()
{
}

void BuilderConstructor::doWriteMetaData(CppPairWriter * writer)
{
}

