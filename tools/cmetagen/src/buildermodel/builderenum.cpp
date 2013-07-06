#include "builderenum.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppenum.h"


BuilderEnum::BuilderEnum(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icEnum);
}

BuilderEnum::~BuilderEnum()
{
}

const CppEnum * BuilderEnum::getCppEnum() const
{
	return static_cast<const CppEnum *>(this->getCppItem());
}

void BuilderEnum::doWriteMetaData(BuilderFileWriter * writer)
{
}

