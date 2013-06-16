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
	if(this->nextFile != NULL) {
		this->nextFile->prepare();
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
	printf("===========Header\n");
	printf("%s\n\n", codeWriter.getText().c_str());
}

void BuilderFileWriter::doWriteSource()
{
	CodeWriter codeWriter;
	this->getSourceWriter()->write(&codeWriter);
	printf("===========Source\n");
	printf("%s\n\n", codeWriter.getText().c_str());
}

void BuilderFileWriter::setupFileCodeBlockStructure()
{
	this->getCodeBlock(ftHeader)->getNamedBlock(CodeBlockName_WrapperArea, cbbWithoutBracket, cbiWithoutIndent);
	this->getCodeBlock(ftHeader)->getNamedBlock(CodeBlockName_ReflectionArea, cbbWithoutBracket, cbiWithoutIndent);
}

string getContainertName(const CppContainer * cppContainer)
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

string getFileIndexName(int fileIndex)
{
	string result;

	if(fileIndex > 0) {
		Poco::format(result, "_%d", fileIndex);
	}

	return result;
}

std::string BuilderFileWriter::getReflectionFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->config->getReflectionFunctionPrefix() + "_" + getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

std::string BuilderFileWriter::getCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->config->getCreationFunctionPrefix() + "_" + getContainertName(cppContainer) + getFileIndexName(this->fileIndex));
}

void BuilderFileWriter::requireItemConainerFunction(const CppItem * cppItem)
{
	const CppContainer * cppContainer;

	if(cppItem->isContainer()) {
		cppContainer = static_cast<const CppContainer *>(cppItem);
	}
	else {
		cppContainer = cppItem->getParent();
	}

	string reflectionName = this->getReflectionFunctionName(cppContainer);
	if(this->generatedFunctionItemNames.find(reflectionName) != this->generatedFunctionItemNames.end()) {
		return;
	}

	CodeBlock * codeBlock = this->getFunctionHeaderCodeBlock(cppContainer, ftHeader);
	const CppClass * cppClass = cppContainer->isClass() ? static_cast<const CppClass *>(cppContainer) : NULL;

	string s = "template <typename D";
	if(cppClass != NULL && cppClass->isTemplate()) {
		s.append(", ");
		s.append(paramListToString(cppClass->getTemplateParamList(), poWithType | poWithName | poWithDefaultValue));
	}
	s.append(">");
	codeBlock->addLine(s);

	s = "void ";
	s.append(this->getReflectionFunctionName(cppContainer));
	s.append("(D & _d)");
	codeBlock->addLine(s);

	string creationName = this->getCreationFunctionName(cppContainer);
}

CodeBlock * BuilderFileWriter::getFunctionContainerCodeBlock(const CppItem * cppItem, FileType fileType)
{
	CodeBlock * codeBlock = this->getCodeBlock(fileType)->getNamedBlock(CodeBlockName_ReflectionArea, cbbWithoutBracket, cbiWithoutIndent);

	string blockName;

	if(cppItem->getParent() == NULL || cppItem->isContainer()) {
		blockName = cppItem->getQualifiedName();
	}
	else {
		blockName = cppItem->getParent()->getQualifiedName();
	}
	return codeBlock->getNamedBlock(blockName, cbbWithoutBracket, cbiWithoutIndent);
}

CodeBlock * BuilderFileWriter::getFunctionHeaderCodeBlock(const CppItem * cppItem, FileType fileType)
{
	CodeBlock * block = this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionHeader, cbbWithoutBracket, cbiWithoutIndent);
	this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionBody);
	return block;
}

CodeBlock * BuilderFileWriter::getFunctionBodyCodeBlock(const CppItem * cppItem, FileType fileType)
{
	this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionHeader, cbbWithoutBracket, cbiWithoutIndent);
	CodeBlock * block = this->getFunctionContainerCodeBlock(cppItem, fileType)->getNamedBlock(CodeBlockName_FunctionBody);
	return block;
}

CodeBlock * BuilderFileWriter::getMetaDataCodeBlock(const CppItem * cppItem, FileType fileType)
{
	return this->getFunctionBodyCodeBlock(cppItem, fileType)->getNamedBlock(ItemNames[cppItem->getCategory()], cbbWithoutBracket, cbiWithoutIndent);
}

CodeBlock * BuilderFileWriter::getCodeBlock(FileType fileType)
{
	return (fileType == ftHeader ? this->getHeaderWriter() : this->getSourceWriter())->getCodeBlock();
}

