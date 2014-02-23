#include "buildernamespace.h"
#include "builderwriter.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppnamespace.h"

namespace metagen {


BuilderNamespace::BuilderNamespace(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icNamespace);
}

BuilderNamespace::~BuilderNamespace()
{
}

const CppNamespace * BuilderNamespace::getCppNamespace() const
{
	return static_cast<const CppNamespace *>(this->getCppItem());
}

void BuilderNamespace::doWriteMetaData(BuilderWriter * /*writer*/)
{
}


} // namespace metagen
