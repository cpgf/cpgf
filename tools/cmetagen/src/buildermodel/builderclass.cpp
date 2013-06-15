#include "builderclass.h"
#include "codewriter/cppwriter.h"


BuilderClass::BuilderClass(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icClass);
}

BuilderClass::~BuilderClass()
{
}

void BuilderClass::doWriteMetaData(CppPairWriter * writer)
{
}

