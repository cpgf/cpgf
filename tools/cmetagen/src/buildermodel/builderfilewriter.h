#ifndef __BUILDERFILEWRITER_H
#define __BUILDERFILEWRITER_H

#include "cpgf/gscopedptr.h"

#include <string>
#include <vector>
#include <set>
#include <map>

namespace metagen {


class CppWriter;
class CppItem;
class CppContainer;
class CodeBlock;
class BuilderItem;
class BuilderSection;
class BuilderSectionList;
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
	
	typedef std::map<const CppContainer *, BuilderSection *> ContainerSectionMapType;

public:
	BuilderFileWriter(int fileIndex, const Config * config, CppWriter * headerWriter);
	~BuilderFileWriter();
	
	CppWriter * getHeaderWriter() const { return this->headerWriter; }
	CppWriter * getSourceWriter() const { return this->sourceWriter.get(); }

	int getFileIndex() const { return this->fileIndex; }
	const Config * getConfig() const { return this->config; }

	ItemListType * getItemList() { return &this->itemList; }
	void setNextFile(BuilderFileWriter * nextFile) { this->nextFile = nextFile; }

	void prepare();
//	void prepareMaster();
	void writeFile();
	void generateCode(BuilderSectionList * sectionList);

public:
	CodeBlock * createOperatorWrapperCodeBlock(const CppItem * cppItem);
	CodeBlock * createBitFieldWrapperCodeBlock(const CppItem * cppItem);
	CodeBlock * getParentReflectionCodeBlock(const CppItem * cppItem);
	CodeBlock * getContainerReflectionCodeBlock(const CppContainer * cppContainer);

public: // auxiliary functions used by BuilderItem's
	CodeBlock * getDeclarationCodeBlock(FileType fileType);
	CodeBlock * getImplementationCodeBlock(FileType fileType);
	
	std::string getReflectionAction(const std::string & name);
	
	std::string getCreationFunctionName(const CppContainer * cppContainer);
	std::string getCreationFunctionPrototype(const CppContainer * cppContainer);
	
private: // new
	BuilderSection * getContainerSection(const CppContainer * cppContainer);
	CodeBlock * getReflectionBodyBlock(CodeBlock * codeBlock);
	void initializeReflectionFunctionOutline(CodeBlock * codeBlock, const CppContainer * cppContainer);
	void createPartialCreationFunction(const CppContainer * cppContainer);
	void initializePartialCreationFunction(CodeBlock * codeBlock, const CppContainer * cppContainer);

private:
	std::string getReflectionFunctionName(const CppContainer * cppContainer);
	std::string getPartialCreationFunctionName(const CppContainer * cppContainer);
	std::string getPartialCreationFunctionPrototype(const CppContainer * cppContainer);

	CodeBlock * getCodeBlock(FileType fileType);
	std::string getContainertName(const CppContainer * cppContainer);
	void doPrepareItemConainer(const CppItem * cppItem);

	void doWriteHeader();
	void doWriteSource();
	void setupFileCodeBlockStructure();

	void doWriteReflectionFunction(const CppContainer * cppContainer);

private:
	int fileIndex;
	const Config * config;
	CppWriter * headerWriter;
	cpgf::GScopedPointer<CppWriter> sourceWriter;
	ItemListType itemList;
	BuilderFileWriter * nextFile;

	StringSetType generatedFunctionItemNames;
	ContainerNameMapType containerNameMap;
	
private: // new
	BuilderSectionList * sectionList;
	ContainerSectionMapType containerSectionMap;
};



} // namespace metagen


#endif
