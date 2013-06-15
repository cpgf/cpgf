#include "buildernamespace.h"
#include "codewriter/cppwriter.h"


BuilderNamespace::BuilderNamespace(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icNamespace);
}

BuilderNamespace::~BuilderNamespace()
{
}

void BuilderNamespace::doWriteMetaData(CppPairWriter * writer)
{
}

