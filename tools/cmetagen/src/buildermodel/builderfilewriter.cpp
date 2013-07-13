#include "builderfilewriter.h"
#include "builderitem.h"
#include "buildersection.h"
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

namespace metagen {


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
		nextFile(NULL),
		sectionList(NULL)
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

/*
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
*/

void BuilderFileWriter::writeFile()
{
	this->doWriteHeader();
	this->doWriteSource();
}

void BuilderFileWriter::generateCode(BuilderSectionList * sectionList)
{
	this->sectionList = sectionList;
	for(ItemListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->writeMetaData(this);
	}
	this->sectionList = NULL;
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

std::string BuilderFileWriter::getPartialCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName("partial_" + this->config->getCreationFunctionPrefix() + "_" + this->getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

std::string BuilderFileWriter::getPartialCreationFunctionPrototype(const CppContainer * cppContainer)
{
	string creationName = this->getPartialCreationFunctionName(cppContainer);
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
		this->getPartialCreationFunctionName(cppContainer),
		this->getPartialCreationFunctionPrototype(cppContainer)
		)
	));

//	this->doWriteSplittedCreationFunction(cppContainer);
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

BuilderSection * BuilderFileWriter::getContainerSection(const CppContainer * cppContainer)
{
	BuilderSection * section;
	ContainerSectionMapType::iterator it = this->containerSectionMap.find(cppContainer);
	if(it == this->containerSectionMap.end()) {
		section = this->sectionList->addSection(bstReflectionFunction, cppContainer);
		this->containerSectionMap.insert(make_pair(cppContainer, section));

		this->initializeReflectionFunctionOutline(section->getCodeBlock(), cppContainer);
		this->createPartialCreationFunction(cppContainer);
	}
	else {
		section = it->second;
	}
	return section;
}

CodeBlock * BuilderFileWriter::getReflectionBodyBlock(CodeBlock * codeBlock)
{
	return codeBlock->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent)->getNamedBlock("freeUse");
}

void BuilderFileWriter::initializeReflectionFunctionOutline(CodeBlock * codeBlock, const CppContainer * cppContainer)
{
	const std::string & D = this->config->getMetaDefineParamName();
	CodeBlock * headerBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionHeader);
	const CppClass * cppClass = cppContainer->isClass() ? static_cast<const CppClass *>(cppContainer) : NULL;

	string s = "template <typename " + D;
	if(cppClass != NULL && cppClass->isChainedTemplate()) {
		s.append(", ");
		s.append(cppClass->getTextOfChainedTemplateParamList(itoWithType | itoWithName | itoWithDefaultValue));
	}
	s.append(" >");
	headerBlock->appendLine(s);

	s = Poco::format("void %s(%s & _d)", this->getReflectionFunctionName(cppContainer), D);
	headerBlock->appendLine(s);
	
	CodeBlock * bodyBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionBody);
	bodyBlock->appendLine("using namespace cpgf;");
	bodyBlock->appendBlankLine();
	
	// force the block order for each kind of items
	for(ItemCategory ic = icFirst; ic < icCount; ic = ItemCategory(int(ic) + 1)) {
		bodyBlock->getNamedBlock(ItemNames[ic]);
	}
}

void BuilderFileWriter::createPartialCreationFunction(const CppContainer * cppContainer)
{
	BuilderSection * section = this->sectionList->addSection(bstPartialCreationFunction, cppContainer);
	this->initializePartialCreationFunction(section->getCodeBlock(), cppContainer);
}

void BuilderFileWriter::initializePartialCreationFunction(CodeBlock * codeBlock, const CppContainer * cppContainer)
{
	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	string prototype = this->getPartialCreationFunctionPrototype(cppContainer);
//	this->getDeclarationCodeBlock(ftHeader)->appendLine(prototype + ";");

	string s;

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

CodeBlock * BuilderFileWriter::createOperatorWrapperCodeBlock(const CppItem * cppItem)
{
	return this->sectionList->addSection(bstOperatorWrapperFunction, cppItem)->getCodeBlock();
}

CodeBlock * BuilderFileWriter::createBitFieldWrapperCodeBlock(const CppItem * cppItem)
{
	return this->sectionList->addSection(bstBitFieldWrapperFunction, cppItem)->getCodeBlock();
}

CodeBlock * BuilderFileWriter::getParentReflectionCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getContainerSection(cppItem->getParent());
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getContainerReflectionCodeBlock(const CppContainer * cppContainer)
{
	BuilderSection * section = this->getContainerSection(cppContainer);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppContainer->getCategory()], cbsTailEmptyLine);
}

} // namespace metagen
