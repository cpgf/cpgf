#include "builderfield.h"
#include "codewriter/cppwriter.h"


BuilderField::BuilderField(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icField);
}

BuilderField::~BuilderField()
{
}

void BuilderField::doWriteMetaData(CppPairWriter * writer)
{
}

