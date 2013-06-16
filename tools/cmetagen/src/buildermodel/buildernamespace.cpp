#include "buildernamespace.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"


BuilderNamespace::BuilderNamespace(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icNamespace);
}

BuilderNamespace::~BuilderNamespace()
{
}

void BuilderNamespace::doWriteMetaData(BuilderFileWriter * writer)
{
}

