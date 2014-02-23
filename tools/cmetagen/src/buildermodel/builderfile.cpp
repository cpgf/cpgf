#include "builderfile.h"
#include "builderwriter.h"
#include "buildersection.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppfile.h"
#include "project.h"
#include "util.h"

namespace metagen {


BuilderFile::BuilderFile(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icFile);
}

BuilderFile::~BuilderFile()
{
}

const CppFile * BuilderFile::getCppFile() const
{
	return static_cast<const CppFile *>(this->getCppItem());
}
	
void BuilderFile::doWriteMetaData(BuilderWriter * /*writer*/)
{
}


} // namespace metagen
