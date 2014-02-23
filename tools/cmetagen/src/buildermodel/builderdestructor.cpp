#include "builderdestructor.h"
#include "builderwriter.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppdestructor.h"

namespace metagen {


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

bool BuilderDestructor::doCanBind() const
{
	return false;
}

void BuilderDestructor::doWriteMetaData(BuilderWriter * /*writer*/)
{
}


} // namespace metagen
