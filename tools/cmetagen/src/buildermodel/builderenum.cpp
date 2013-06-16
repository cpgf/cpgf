#include "builderenum.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"


BuilderEnum::BuilderEnum(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icEnum);
}

BuilderEnum::~BuilderEnum()
{
}

void BuilderEnum::doWriteMetaData(BuilderFileWriter * writer)
{
}

