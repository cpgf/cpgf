#include "builderfile.h"
#include "builderfilewriter.h"
#include "buildersection.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
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
	
void BuilderFile::doWriteMetaData(BuilderFileWriter * /*writer*/)
{
}

void BuilderFile::outputFiles()
{
	BuilderSectionList sectionList;
	BuilderFileWriter currentFile(this->getConfig());
	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		currentFile.getItemList()->push_back(*it);
	}
	currentFile.generateCode(&sectionList);

	sectionList.dump();
}


} // namespace metagen
