#include "buildernamespace.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
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

void BuilderNamespace::doWriteMetaData(BuilderFileWriter * /*writer*/)
{
}


} // namespace metagen
