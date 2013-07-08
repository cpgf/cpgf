#include "builderfile.h"
#include "builderfilewriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppfile.h"
#include "config.h"
#include "util.h"


BuilderFile::BuilderFile(const CppItem * cppItem)
	: super(cppItem), headerWriter(new CppWriter)
{
	this->checkBuilderItemCategory(icFile);
}

BuilderFile::~BuilderFile()
{
	clearPointerContainer(this->builderFileWriterList);
}

const CppFile * BuilderFile::getCppFile() const
{
	return static_cast<const CppFile *>(this->getCppItem());
}
	
void BuilderFile::doWriteMetaData(BuilderFileWriter * /*writer*/)
{
}

void BuilderFile::prepare()
{
	this->sortItems();
	this->createFileWriters();
	for(BuilderFileWriterListType::iterator it = this->builderFileWriterList.begin(); it != this->builderFileWriterList.end(); ++it) {
		(*it)->prepare();
	}
	this->builderFileWriterList[0]->prepareMaster();
}

void BuilderFile::createFileWriters()
{
	BuilderFileWriter * currentFile = new BuilderFileWriter(0, this->getConfig(), this->headerWriter.get());
	this->builderFileWriterList.push_back(currentFile);

	int itemCountInWriter = 0;
	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		currentFile->getItemList()->push_back(*it);
		if(this->getConfig()->getMaxItemCountPerFile() > 0) {
			const CppItem * item = (*it)->getCppItem();
			if(item->isFile() || item->isNamespace() || item->isClass()) {
			}
			else {
				++itemCountInWriter;
			}
			if(itemCountInWriter >= this->getConfig()->getMaxItemCountPerFile()) {
				currentFile = new BuilderFileWriter(this->builderFileWriterList.size(), this->getConfig(), this->headerWriter.get());
				this->builderFileWriterList[this->builderFileWriterList.size() - 1]->setNextFile(currentFile);
				this->builderFileWriterList.push_back(currentFile);
				itemCountInWriter = 0;
			}
		}
	}
}

void BuilderFile::outputFiles()
{
	for(BuilderFileWriterListType::iterator it = this->builderFileWriterList.begin(); it != this->builderFileWriterList.end(); ++it) {
		(*it)->generateCode();
	}

	CodeWriter codeWriter;
	this->headerWriter->write(&codeWriter);
	printf("===========Header\n");
	printf("%s\n\n", codeWriter.getText().c_str());

	for(BuilderFileWriterListType::iterator it = this->builderFileWriterList.begin(); it != this->builderFileWriterList.end(); ++it) {
		(*it)->writeFile();
	}
}

