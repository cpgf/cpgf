#include "builderoperator.h"
#include "codewriter/cppwriter.h"


BuilderOperator::BuilderOperator(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icOperator);
}

BuilderOperator::~BuilderOperator()
{
}

void BuilderOperator::doWriteMetaData(CppPairWriter * writer)
{
}

