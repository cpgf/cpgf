#include "builderdestructor.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppdestructor.h"


BuilderDestructor::BuilderDestructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icDestructor);
}

BuilderDestructor::~BuilderDestructor()
{
}

const CppDestructor * BuilderDestructor::getCppDestructor() const
{
	return static_cast<const CppDestructor *>(this->getCppItem());
}

bool BuilderDestructor::canBind() const
{
	return false;
}

void BuilderDestructor::doWriteMetaData(BuilderFileWriter * /*writer*/)
{
}

