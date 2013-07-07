#include "builderfilewriter.h"
#include "builderitem.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "config.h"
#include "util.h"

#include "Poco/Format.h"

#include <string>

using namespace std;

const string CodeBlockName_WrapperArea("wrapper");
const string CodeBlockName_DeclarationArea("declaration");
const string CodeBlockName_ReflectionArea("reflection");
const string CodeBlockName_FunctionHeader("header");
const string CodeBlockName_FunctionBody("body");


BuilderFileWriter::BuilderFileWriter(int fileIndex, const Config * config)
	:	fileIndex(fileIndex),
		config(config),
		headerWriter(new CppWriter()),
		sourceWriter(new CppWriter()),
		nextFile(NULL)
{
	this->setupFileCodeBlockStructure();
}

BuilderFileWriter::~BuilderFileWriter()
{
}

void BuilderFileWriter::prepare()
{
	for(ItemListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		this->doPrepareItemConainer((*it)->getCppItem());
	}
}

void BuilderFileWriter::prepareMaster()
{
	struct ContainerName
	{
		ContainerName(const string & creationFunctionName, const string & creationPrototype)
			: creationFunctionName(creationFunctionName), creationPrototype(creationPrototype)
		{}

		string creationFunctionName;
		string creationPrototype;
	};

	typedef vector<ContainerName> ContainerNameListType;
	typedef map<const CppContainer *, ContainerNameListType> ContainerMapType;

	ContainerMapType containerMap;

	BuilderFileWriter * currentWriter = this;
	while(currentWriter != NULL) {
		for(ContainerListType::iterator it = currentWriter->containerList.begin();
			it != currentWriter->containerList.end();
			++it) {
			containerMap[*it].push_back(ContainerName(
				currentWriter->getCreationFunctionName(*it),
				currentWriter->getCreationFunctionPrototype(*it)
				));
		}
		currentWriter = currentWriter->nextFile;
	}

	for(ContainerMapType::iterator it = containerMap.begin(); it != containerMap.end(); ++it) {
		CodeBlock * codeBlock = this->getDeclarationCodeBlock(ftSource);

		const ContainerNameListType & nameList = it->second;
		for(ContainerNameListType::const_iterator n = nameList.begin(); n != nameList.end(); ++n) {
			codeBlock->addLine(n->creationPrototype + ";");
		}

		string creationName = this->getMasterCreationFunctionName(it->first);
		string prototype = Poco::format("cpgf::GDefineMetaInfo %s()", creationName);

		this->getDeclarationCodeBlock(ftHeader)->addLine(prototype + ";");

		codeBlock->addLine(prototype);

		CodeBlock * bodyBlock = codeBlock->addBlock(cbsBracketAndIndent);
		string s;
		bodyBlock->addLine("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();");
		bodyBlock->addLine("cpgf::GDefineMetaInfo meta = _d.getMetaInfo();");
		bodyBlock->addBlankLine();

		for(ContainerNameListType::const_iterator n = nameList.begin(); n != nameList.end(); ++n) {
			s = Poco::format("%s(meta);", n->creationFunctionName);
			bodyBlock->addLine(s);
		}

		bodyBlock->addBlankLine();
		bodyBlock->addLine("return meta;");
	}
}

void BuilderFileWriter::writeFile()
{
	for(ItemListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->writeMetaData(this);
	}

	this->doWriteHeader();
	this->doWriteSource();
}

void BuilderFileWriter::doWriteHeader()
{
	CodeWriter codeWriter;
	this->getHeaderWriter()->write(&codeWriter);
	printf("===========Header %d\n", this->fileIndex);
	printf("%s\n\n", codeWriter.getText().c_str());
}

void BuilderFileWriter::doWriteSource()
{
	CodeWriter codeWriter;
	this->getSourceWriter()->write(&codeWriter);
	printf("===========Source %d\n", this->fileIndex);
	printf("%s\n\n", codeWriter.getText().c_str());
}

void BuilderFileWriter::setupFileCodeBlockStructure()
{
	const string * areaNames[] = {
		&CodeBlockName_WrapperArea,
		&CodeBlockName_DeclarationArea,
		&CodeBlockName_ReflectionArea
	};
	for(int i = 0; i < sizeof(areaNames) / sizeof(areaNames[0]); ++i) {
		this->getCodeBlock(ftHeader)->getNamedBlock(*areaNames[i]);
		this->getCodeBlock(ftHeader)->getNamedBlock(*areaNames[i]);
	}
}

string getFileIndexName(int fileIndex)
{
	string result;

	if(fileIndex > 0) {
		Poco::format(result, "_%d", fileIndex);
	}

	return result;
}

string BuilderFileWriter::getContainertName(const CppContainer * cppContainer)
{
	string result;

	if(cppContainer->isClass()) {
		result = normalizeSymbolName(cppContainer->getQualifiedName());
	}
	else {
		result = "Global";
	}

	return result;
}

std::string BuilderFileWriter::getReflectionFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->config->getReflectionFunctionPrefix() + "_" + this->getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

std::string BuilderFileWriter::getCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName("internal_" + this->config->getCreationFunctionPrefix() + "_" + this->getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

std::string BuilderFileWriter::getCreationFunctionPrototype(const CppContainer * cppContainer)
{
	string creationName = this->getCreationFunctionName(cppContainer);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string BuilderFileWriter::getMasterCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->config->getCreationFunctionPrefix() + "_" + this->getContainertName(cppContainer));
}

void BuilderFileWriter::doPrepareItemConainer(const CppItem * cppItem)
{
	const CppContainer * cppContainer;

	if(cppItem->isContainer()) {
		cppContainer = static_cast<const CppContainer *>(cppItem);
	}
	else {
		cppContainer = cppItem->getNamedParent();
	}

	string reflectionName = this->getReflectionFunctionName(cppContainer);
	if(this->generatedFunctionItemNames.find(reflectionName) != this->generatedFunctionItemNames.end()) {
		return;
	}
	this->generatedFunctionItemNames.insert(reflectionName);
	this->containerList.push_back(cppContainer);

	this->doWriteReflectionFunction(cppContainer);
	this->doWriteCreationFunction(cppContainer);
}

void BuilderFileWriter::doWriteReflectionFunction(const CppContainer * cppContainer)
{
	const std::string & D = this->config->getMetaDefineParamName();
	CodeBlock * codeBlock = this->getFunctionHeaderCodeBlock(cppContainer, ftHeader);
	const CppClass * cppClass = cppContainer->isClass() ? static_cast<const CppClass *>(cppContainer) : NULL;

	string s = "template <typename " + D;
	if(cppClass != NULL && cppClass->isChainedTemplate()) {
		s.append(", ");
		s.append(cppClass->getTextOfChainedTemplateParamList(itoWithType | itoWithName | itoWithDefaultValue));
	}
	s.append(" >");
	codeBlock->addLine(s);

	s = Poco::format("void %s(%s & _d)", this->getReflectionFunctionName(cppContainer), D);
	codeBlock->addLine(s);
	
	CodeBlock * bodyBlock = this->getFunctionBodyCodeBlock(cppContainer, ftHeader);
	bodyBlock->addLine("using namespace cpgf;");
	bodyBlock->addBlankLine();
	
	// force the block order for each kind of items
	for(ItemCategory ic = icFirst; ic < icCount; ic = ItemCategory(int(ic) + 1)) {
		bodyBlock->getNamedBlock(ItemNames[ic]);
	}
}

void BuilderFileWriter::doWriteCreationFunction(const CppContainer * cppContainer)
{
	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	if(cppClass != NULL && cppClass->isTemplate()) {
		return;
	}

	string prototype = this->getCreationFunctionPrototype(cppContainer);
	this->getDeclarationCodeBlock(ftHeader)->addLine(prototype + ";");

	CodeBlock * codeBlock = this->getDeclarationCodeBlock(ftSource);
	codeBlock->addLine(prototype);

	CodeBlock * bodyBlock = codeBlock->addBlock(cbsBracketAndIndent);
	string s;
	string metaType;
	if(cppClass != NULL) {
		metaType = Poco::format("cpgf::GDefineMetaClass<%s >", cppClass->getOutputName());
	}
	else {
		metaType = "GDefineMetaGlobal";
	}
	s = Poco::format("%s meta = %s::fromMetaClass(metaInfo.getMetaClass());", metaType, metaType);
	bodyBlock->addLine(s);
	s = Poco::format("%s(meta);", this->getReflectionFunctionName(cppContainer));
	bodyBlock->addLine(s);
}

CodeBlock * BuilderFileWriter::getFunctionContainerCodeBlock(const CppItem * cppItem, FileType fileType)
{
	CodeBlock * codeBlock = this->getCodeBlock(fileType)->getNamedBlock(CodeBlockName_ReflectionArea);

	string blockName;

	if(cppItem->isContainer()) {
		blockName = getContainertName(static_cast<const CppContainer *>(cppItem));
	}
	else {
		blockName = cppItem->getNamedParent()->getQualifiedName();
	}
	return codeBlock->getNamedBlock(blockName);
}

CodeBlock * BuilderFileWriter::getFunctionHeaderCodeBlock(const CppItem * cppItem, FileType fileType)
{
	CodeBlock * block = this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionHeader);
	this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent);
	return block;
}

CodeBlock * BuilderFileWriter::getFunctionBodyCodeBlock(const CppItem * cppItem, FileType fileType)
{
	this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionHeader);
	CodeBlock * block = this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent);
	return block;
}

CodeBlock * BuilderFileWriter::getReflectionCodeBlock(const CppItem * cppItem)
{
	return this->getFunctionBodyCodeBlock(cppItem->getNamedParent(), ftHeader)->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getWrapperCodeBlock(const CppItem * cppItem, FileType fileType)
{
	return this->getCodeBlock(fileType)->getNamedBlock(CodeBlockName_WrapperArea)
		->getNamedBlock(ItemNames[cppItem->getCategory()]);
}

CodeBlock * BuilderFileWriter::getDeclarationCodeBlock(FileType fileType)
{
	return this->getCodeBlock(fileType)->getNamedBlock(CodeBlockName_DeclarationArea, cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getCodeBlock(FileType fileType)
{
	return (fileType == ftHeader ? this->getHeaderWriter() : this->getSourceWriter())->getCodeBlock();
}

std::string BuilderFileWriter::getReflectionAction(const std::string & name)
{
	return "_d.CPGF_MD_TEMPLATE " + name;
}

