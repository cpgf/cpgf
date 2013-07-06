#include "builderconstructor.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppconstructor.h"


BuilderConstructor::BuilderConstructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icConstructor);
}

BuilderConstructor::~BuilderConstructor()
{
}

const CppConstructor * BuilderConstructor::getCppConstructor() const
{
	return static_cast<const CppConstructor *>(this->getCppItem());
}

void BuilderConstructor::doWriteMetaData(BuilderFileWriter * writer)
{
}

