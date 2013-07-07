#ifndef __BUILDERFILEWRITER_H
#define __BUILDERFILEWRITER_H

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <set>
#include <map>


class CppWriter;
class CppItem;
class CppContainer;
class CodeBlock;
class BuilderItem;
class Config;

enum FileType {
	ftHeader, ftSource
};
	
class BuilderFileWriter
{
public:
	typedef std::vector<BuilderItem *> ItemListType;
	
private:
	struct ContainerName {
		ContainerName(const std::string & creationFunctionName,
			const std::string & creationPrototype)
			: creationFunctionName(creationFunctionName),
				creationPrototype(creationPrototype)
		{}

		std::string creationFunctionName;
		std::string creationPrototype;
	};

	typedef std::set<std::string> StringSetType;
	typedef std::map<const CppContainer *, ContainerName> ContainerNameMapType;

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
	void prepareMaster();
	void writeFile();
	
public: // auxiliary functions used by BuilderItem's
	CodeBlock * getFunctionContainerCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getFunctionHeaderCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getFunctionBodyCodeBlock(const CppItem * cppItem, FileType fileType);
	
	CodeBlock * getReflectionCodeBlock(const CppItem * cppItem);
	
	CodeBlock * getWrapperCodeBlock(const CppItem * cppItem, FileType fileType);
	CodeBlock * getDeclarationCodeBlock(FileType fileType);
	CodeBlock * getImplementationCodeBlock(FileType fileType);
	
	std::string getReflectionAction(const std::string & name);
	
	std::string getCreationFunctionName(const CppContainer * cppContainer);
	std::string getCreationFunctionPrototype(const CppContainer * cppContainer);

private:
	std::string getReflectionFunctionName(const CppContainer * cppContainer);
	std::string getSplittedCreationFunctionName(const CppContainer * cppContainer);
	std::string getSplittedCreationFunctionPrototype(const CppContainer * cppContainer);

	CodeBlock * getCodeBlock(FileType fileType);
	std::string getContainertName(const CppContainer * cppContainer);
	void doPrepareItemConainer(const CppItem * cppItem);

	void doWriteHeader();
	void doWriteSource();
	void setupFileCodeBlockStructure();

	void doWriteReflectionFunction(const CppContainer * cppContainer);
	void doWriteCreationFunction(const CppContainer * cppContainer);

private:
	int fileIndex;
	const Config * config;
	cpgf::GScopedPointer<CppWriter> headerWriter;
	cpgf::GScopedPointer<CppWriter> sourceWriter;
	ItemListType itemList;
	BuilderFileWriter * nextFile;

	StringSetType generatedFunctionItemNames;
	ContainerNameMapType containerNameMap;
};


#endif
