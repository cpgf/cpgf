#ifndef __BUILDERFILEWRITER_H
#define __BUILDERFILEWRITER_H

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <set>


class CppWriter;
class CppItem;
class CppContainer;
class CodeBlock;
class BuilderItem;
class Config;

class BuilderFileWriter
{
public:
	typedef std::vector<BuilderItem *> ItemListType;
	
	enum FileType {
		ftHeader, ftSource
	};
	
private:
	typedef std::set<std::string> StringSetType;

public:
	BuilderFileWriter(int fileIndex, const Config * config);
	~BuilderFileWriter();
	
	CppWriter * getHeaderWriter() const { return this->headerWriter.get(); }
	CppWriter * getSourceWriter() const { return this->sourceWriter.get(); }

	int getFileIndex() const { return this->fileIndex; }
	const Config * getConfig() const { return this->config; }

	ItemListType * getItemList() { return &this->itemList; }
	void setNextFile(BuilderFileWriter * nextFile) { this->nextFile = nextFile; }

	void prepare();
	void writeFile();
	
public: // auxiliary functions used by BuilderItem's
	std::string getReflectionFunctionName(const CppContainer * cppContainer);
	std::string getCreationFunctionName(const CppContainer * cppContainer);
	void requireItemConainerFunction(const CppItem * cppItem);

	CodeBlock * getFunctionContainerCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getFunctionHeaderCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getFunctionBodyCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getMetaDataCodeBlock(const CppItem * cppItem, FileType fileType);
	
	std::string getReflectionAction(const std::string & name);
	
private:
	CodeBlock * getCodeBlock(FileType fileType);

private:
	void doWriteHeader();
	void doWriteSource();
	void setupFileCodeBlockStructure();

private:
	int fileIndex;
	const Config * config;
	cpgf::GScopedPointer<CppWriter> headerWriter;
	cpgf::GScopedPointer<CppWriter> sourceWriter;
	ItemListType itemList;
	BuilderFileWriter * nextFile;

	StringSetType generatedFunctionItemNames;
};


#endif
