#include "builderdestructor.h"
#include "codewriter/cppwriter.h"


BuilderDestructor::BuilderDestructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icDestructor);
}

BuilderDestructor::~BuilderDestructor()
{
}

void BuilderDestructor::doWriteMetaData(CppPairWriter * writer)
{
}

