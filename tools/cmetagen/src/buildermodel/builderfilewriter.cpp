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
const string CodeBlockName_ImplementionArea("implementation");
const string CodeBlockName_FunctionHeader("header");
const string CodeBlockName_FunctionBody("body");


BuilderFileWriter::BuilderFileWriter(int fileIndex, const Config * config, CppWriter * headerWriter)
	:	fileIndex(fileIndex),
		config(config),
		headerWriter(headerWriter),
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
	typedef vector<ContainerName> ContainerNameListType;
	typedef map<const CppContainer *, ContainerNameListType> ContainerMapType;

	ContainerMapType containerMap;

	BuilderFileWriter * currentWriter = this;
	while(currentWriter != NULL) {
		for(ContainerNameMapType::iterator it = currentWriter->containerNameMap.begin();
			it != currentWriter->containerNameMap.end();
			++it) {
			containerMap[it->first].push_back(it->second);
		}
		currentWriter = currentWriter->nextFile;
	}

	for(ContainerMapType::iterator it = containerMap.begin(); it != containerMap.end(); ++it) {
		CodeBlock * headerCodeBlock = this->getDeclarationCodeBlock(ftHeader);
		CodeBlock * sourceCodeBlock;
		string s;

		const CppClass * cppClass = NULL;
		if(it->first->isClass()) {
			cppClass = static_cast<const CppClass *>(it->first);
		}
		if(cppClass != NULL && cppClass->isTemplate()) {
			sourceCodeBlock = this->getImplementationCodeBlock(ftHeader);
		}
		else {
			sourceCodeBlock = this->getImplementationCodeBlock(ftSource);
		}

		const ContainerNameListType & nameList = it->second;
		//for(ContainerNameListType::const_iterator n = nameList.begin(); n != nameList.end(); ++n) {
		//	sourceCodeBlock->appendLine(n->creationPrototype + ";");
		//}

		string prototype = this->getCreationFunctionPrototype(it->first);

		if(cppClass != NULL && cppClass->isTemplate()) {
			s = Poco::format("template<%s >", cppClass->getTextOfChainedTemplateParamList(itoWithType | itoWithName));
			headerCodeBlock->appendLine(s);
			sourceCodeBlock->appendLine(s);
		}
		headerCodeBlock->appendLine(prototype + ";");
		sourceCodeBlock->appendLine(prototype);

		CodeBlock * bodyBlock = sourceCodeBlock->appendBlock(cbsBracketAndIndent);
		bodyBlock->appendLine("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();");
		bodyBlock->appendLine("cpgf::GDefineMetaInfo meta = _d.getMetaInfo();");
		bodyBlock->appendBlankLine();

		for(ContainerNameListType::const_iterator n = nameList.begin(); n != nameList.end(); ++n) {
			if(cppClass != NULL && cppClass->isTemplate()) {
				s = Poco::format("%s<%s >(meta);", n->creationFunctionName, cppClass->getTextOfChainedTemplateParamList(itoWithName));
			}
			else {
				s = Poco::format("%s(meta);", n->creationFunctionName);
			}
			bodyBlock->appendLine(s);
		}

		bodyBlock->appendBlankLine();
		bodyBlock->appendLine("return meta;");
	}
}

void BuilderFileWriter::writeFile()
{
	this->doWriteHeader();
	this->doWriteSource();
}

void BuilderFileWriter::generateCode()
{
	for(ItemListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->writeMetaData(this);
	}
}

void BuilderFileWriter::doWriteHeader()
{
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
		&CodeBlockName_ReflectionArea,
		&CodeBlockName_ImplementionArea
	};
	for(int i = 0; i < sizeof(areaNames) / sizeof(areaNames[0]); ++i) {
		this->getCodeBlock(ftHeader)->getNamedBlock(*areaNames[i]);
		this->getCodeBlock(ftSource)->getNamedBlock(*areaNames[i]);
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

std::string BuilderFileWriter::getSplittedCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName("splitted_" + this->config->getCreationFunctionPrefix() + "_" + this->getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

std::string BuilderFileWriter::getSplittedCreationFunctionPrototype(const CppContainer * cppContainer)
{
	string creationName = this->getSplittedCreationFunctionName(cppContainer);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string BuilderFileWriter::getCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->config->getCreationFunctionPrefix() + "_" + this->getContainertName(cppContainer));
}

std::string BuilderFileWriter::getCreationFunctionPrototype(const CppContainer * cppContainer)
{
	string creationName = this->getCreationFunctionName(cppContainer);
	return Poco::format("cpgf::GDefineMetaInfo %s()", creationName);
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
	this->containerNameMap.insert(make_pair(cppContainer, ContainerName(
		this->getSplittedCreationFunctionName(cppContainer),
		this->getSplittedCreationFunctionPrototype(cppContainer)
		)
	));

	this->doWriteReflectionFunction(cppContainer);
	this->doWriteSplittedCreationFunction(cppContainer);
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
	codeBlock->appendLine(s);

	s = Poco::format("void %s(%s & _d)", this->getReflectionFunctionName(cppContainer), D);
	codeBlock->appendLine(s);
	
	CodeBlock * bodyBlock = this->getFunctionBodyCodeBlock(cppContainer, ftHeader);
	bodyBlock->appendLine("using namespace cpgf;");
	bodyBlock->appendBlankLine();
	
	// force the block order for each kind of items
	for(ItemCategory ic = icFirst; ic < icCount; ic = ItemCategory(int(ic) + 1)) {
		bodyBlock->getNamedBlock(ItemNames[ic]);
	}
}

void BuilderFileWriter::doWriteSplittedCreationFunction(const CppContainer * cppContainer)
{
	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	string prototype = this->getSplittedCreationFunctionPrototype(cppContainer);
//	this->getDeclarationCodeBlock(ftHeader)->appendLine(prototype + ";");

	string s;
	CodeBlock * codeBlock;

	if(cppClass != NULL && cppClass->isTemplate()) {
		codeBlock = this->getImplementationCodeBlock(ftHeader);
	}
	else {
		codeBlock = this->getImplementationCodeBlock(ftSource);
	}

	if(cppClass != NULL && cppClass->isTemplate()) {
		s = Poco::format("template <%s >", cppClass->getTextOfChainedTemplateParamList(itoWithType | itoWithName));
		codeBlock->appendLine(s);
	}
	codeBlock->appendLine(prototype);

	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);
	string metaType;
	if(cppClass != NULL) {
		if(cppClass->isTemplate()) {
			metaType = Poco::format("cpgf::GDefineMetaClass<%s<%s > >", cppClass->getOutputName(), cppClass->getTextOfChainedTemplateParamList(itoWithName));
		}
		else {
			metaType = Poco::format("cpgf::GDefineMetaClass<%s >", cppClass->getOutputName());
		}
	}
	else {
		metaType = "GDefineMetaGlobal";
	}
	s = Poco::format("%s meta = %s::fromMetaClass(metaInfo.getMetaClass());", metaType, metaType);
	bodyBlock->appendLine(s);
	s = Poco::format("%s(meta);", this->getReflectionFunctionName(cppContainer));
	bodyBlock->appendLine(s);
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
	Poco::format(blockName, "%s%d", blockName, this->fileIndex);
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

CodeBlock * BuilderFileWriter::getImplementationCodeBlock(FileType fileType)
{
	return this->getCodeBlock(fileType)->getNamedBlock(CodeBlockName_ImplementionArea, cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getCodeBlock(FileType fileType)
{
	return (fileType == ftHeader ? this->getHeaderWriter() : this->getSourceWriter())->getCodeBlock();
}

std::string BuilderFileWriter::getReflectionAction(const std::string & name)
{
	return "_d.CPGF_MD_TEMPLATE " + name;
}

