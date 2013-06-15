#include "builderenum.h"
#include "codewriter/cppwriter.h"


BuilderEnum::BuilderEnum(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icEnum);
}

BuilderEnum::~BuilderEnum()
{
}

void BuilderEnum::doWriteMetaData(CppPairWriter * writer)
{
}

