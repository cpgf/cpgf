#include "builderfile.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
#include "application/config.h"
#include "util.h"


class SplittedFile
{
public:
	typedef std::vector<BuilderItem *> ItemListType;
	
public:
	explicit SplittedFile(int fileIndex, const Config * config) : fileIndex(fileIndex), config(config), nextFile(NULL) { }

	ItemListType * getItemList() { return &this->itemList; }
	void setNextFile(SplittedFile * nextFile) { this->nextFile = nextFile; }

	void prepare();
	void writeFile();

private:
	void doWriteHeader(CppPairWriter * writer);
	void doWriteSource(CppPairWriter * writer);

private:
	ItemListType itemList;
	int fileIndex;
	const Config * config;
	SplittedFile * nextFile;
};

void SplittedFile::prepare()
{
}

void SplittedFile::writeFile()
{
	CppWriter headerWriter("a.h");
	CppWriter sourceWriter("a.cpp");

	CppPairWriter pairWriter(&headerWriter, & sourceWriter);
	this->doWriteHeader(&pairWriter);
	this->doWriteSource(&pairWriter);
}

void SplittedFile::doWriteHeader(CppPairWriter * writer)
{
	for(ItemListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->writeMetaData(writer);
	}
}

void SplittedFile::doWriteSource(CppPairWriter * writer)
{
}


BuilderFile::BuilderFile(const CppItem * cppItem)
	: super(cppItem), config(NULL)
{
	this->checkBuilderItemCategory(icFile);
}

BuilderFile::~BuilderFile()
{
	clearPointerContainer(this->splittedFileList);
}

void BuilderFile::doWriteMetaData(CppPairWriter * writer)
{
}

void BuilderFile::prepare()
{
	this->sortItems();
	this->generateSplittedFiles();
	for(SplittedFileListType::iterator it = this->splittedFileList.begin(); it != this->splittedFileList.end(); ++it) {
		(*it)->prepare();
	}
}

void BuilderFile::generateSplittedFiles()
{
	SplittedFile * currentFile = new SplittedFile(0, this->config);
	this->splittedFileList.push_back(currentFile);

	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		currentFile->getItemList()->push_back(*it);
		if(this->config->getMaxItemCountPerFile() > 0 && currentFile->getItemList()->size() > this->config->getMaxItemCountPerFile()) {
			currentFile = new SplittedFile(this->splittedFileList.size(), this->config);
			this->splittedFileList[this->splittedFileList.size() - 1]->setNextFile(currentFile);
			this->splittedFileList.push_back(currentFile);
		}
	}
}

void BuilderFile::outputFiles()
{
	for(SplittedFileListType::iterator it = this->splittedFileList.begin(); it != this->splittedFileList.end(); ++it) {
		(*it)->writeFile();
	}
}

