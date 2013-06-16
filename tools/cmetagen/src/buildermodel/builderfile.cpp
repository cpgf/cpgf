#include "builderfile.h"
#include "builderfilewriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
#include "config.h"
#include "util.h"


BuilderFile::BuilderFile(const CppItem * cppItem)
	: super(cppItem), config(NULL)
{
	this->checkBuilderItemCategory(icFile);
}

BuilderFile::~BuilderFile()
{
	clearPointerContainer(this->builderFileWriterList);
}

void BuilderFile::doWriteMetaData(BuilderFileWriter * writer)
{
}

void BuilderFile::prepare()
{
	this->sortItems();
	this->createFileWriters();
	this->builderFileWriterList[0]->prepare();
}

void BuilderFile::createFileWriters()
{
	BuilderFileWriter * currentFile = new BuilderFileWriter(0, this->config);
	this->builderFileWriterList.push_back(currentFile);

	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		currentFile->getItemList()->push_back(*it);
		if(this->config->getMaxItemCountPerFile() > 0 && currentFile->getItemList()->size() >= this->config->getMaxItemCountPerFile()) {
			currentFile = new BuilderFileWriter(this->builderFileWriterList.size(), this->config);
			this->builderFileWriterList[this->builderFileWriterList.size() - 1]->setNextFile(currentFile);
			this->builderFileWriterList.push_back(currentFile);
		}
	}
}

void BuilderFile::outputFiles()
{
	for(BuilderFileWriterListType::iterator it = this->builderFileWriterList.begin(); it != this->builderFileWriterList.end(); ++it) {
		(*it)->writeFile();
	}
}

