#include "builderfilewriter.h"
#include "builderitem.h"
#include "buildersection.h"
#include "builderutil.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "config.h"
#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

#include <string>

using namespace std;

namespace metagen {


const string CodeBlockName_ReflectionArea("reflection");
const string CodeBlockName_FunctionHeader("fheader");
const string CodeBlockName_FunctionBody("fbody");
const string CodeBlockName_ClassBody("cbody");
const string CodeBlockName_Customize("customize");

string getTextOfVisibility(ItemVisibility visibility)
{
	switch(visibility) {
		case ivPrivate:
			return "private";

		case ivProtected:
			return "protected";

		default:
			return "public";
	}
}

BuilderFileWriter::BuilderFileWriter(const Config * config, CppWriter * headerWriter)
	:	config(config),
		headerWriter(headerWriter),
		sourceWriter(new CppWriter()),
		nextFile(NULL),
		sectionList(NULL)
{
}

BuilderFileWriter::~BuilderFileWriter()
{
}

void BuilderFileWriter::prepare()
{
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
			s = Poco::format("template<%s >", cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName));
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
				s = Poco::format("%s<%s >(meta);", n->creationFunctionName, cppClass->getTextOfChainedTemplateParamList(itoWithArgName));
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
	printf("===========Source");
	printf("%s\n\n", codeWriter.getText().c_str());
}

std::string BuilderFileWriter::getCreationFunctionName(const CppContainer * cppContainer)
{
	return normalizeSymbolName(this->getConfig()->getCreationFunctionPrefix() + "_" + getContainertName(cppContainer));
}

std::string BuilderFileWriter::getCreationFunctionPrototype(const CppContainer * cppContainer)
{
	string creationName = this->getCreationFunctionName(cppContainer);
	return Poco::format("cpgf::GDefineMetaInfo %s()", creationName);
}

CodeBlock * BuilderFileWriter::getCodeBlock(FileType fileType)
{
	return (fileType == ftHeader ? this->getHeaderWriter() : this->getSourceWriter())->getCodeBlock();
}

std::string BuilderFileWriter::getReflectionAction(const std::string & name)
{
	return "_d.CPGF_MD_TEMPLATE " + name;
}

BuilderSection * getSection(const Config * config, BuilderSectionList * sectionList,
	std::multimap<const CppContainer *, BuilderSection *> * sectionMap,
	const CppContainer * cppContainer, const CppItem * payloadItem, bool * isNewSection)
{
	*isNewSection = false;
	size_t payload = getCppItemPayload(payloadItem);
	BuilderSection * section = NULL;
	std::multimap<const CppContainer *, BuilderSection *>::iterator it = sectionMap->find(cppContainer);
	int existingSectionCount = 0;
	if(it != sectionMap->end()) {
		while(it != sectionMap->end() && it->first == cppContainer) {
			++existingSectionCount;
			if(config->shouldSplitFile()
				&& it->second->getTotalPayload() + payload > config->getMaxItemCountPerFile()) {
				++it;
				continue;
			}
			section = it->second;
			break;
		}
	}
	if(section == NULL) {
		*isNewSection = true;
		section = sectionList->addSection(bstReflectionFunction, cppContainer);
		section->setIndex(existingSectionCount);
		sectionMap->insert(make_pair(cppContainer, section));
	}
	
	section->addPayload(payload);

	return section;
}

BuilderSection * BuilderFileWriter::getReflectionContainerSection(const CppContainer * cppContainer, const CppItem * payloadItem)
{
	bool isNewSection;
	BuilderSection * section = getSection(this->getConfig(), this->sectionList,
		&this->reflectionSectionMap, cppContainer, payloadItem, &isNewSection);
	if(isNewSection) {
		this->initializeReflectionFunctionOutline(section->getCodeBlock(), cppContainer, section->getIndex());
		this->createPartialCreationFunction(cppContainer, section->getIndex());
	}
	
	return section;
}

CodeBlock * BuilderFileWriter::getReflectionBodyBlock(CodeBlock * codeBlock)
{
	return codeBlock->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent)->getNamedBlock(CodeBlockName_Customize);
}

void BuilderFileWriter::initializeReflectionFunctionOutline(CodeBlock * codeBlock, const CppContainer * cppContainer,
															int sectionIndex)
{
	this->initializeReflectionFunctionOutline(codeBlock, cppContainer,
		getReflectionFunctionName(this->getConfig(), cppContainer, sectionIndex));
}

void BuilderFileWriter::initializeReflectionFunctionOutline(CodeBlock * codeBlock, const CppContainer * cppContainer,
															const std::string & functionName)
{
	const std::string & D = this->getConfig()->getMetaDefineParamName();
	CodeBlock * headerBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionHeader);
	const CppClass * cppClass = cppContainer->isClass() ? static_cast<const CppClass *>(cppContainer) : NULL;

	string s = "template <typename " + D;
	if(cppClass != NULL && cppClass->isChainedTemplate()) {
		s.append(", ");
		s.append(cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue));
	}
	s.append(" >");
	headerBlock->appendLine(s);

	s = Poco::format("void %s(%s & _d)", functionName, D);
	headerBlock->appendLine(s);
	
	CodeBlock * bodyBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionBody);
	bodyBlock->appendLine("using namespace cpgf;");
	bodyBlock->appendBlankLine();
	
	// force the block order for each kind of items
	for(ItemCategory ic = icFirst; ic < icCount; ic = ItemCategory(int(ic) + 1)) {
		bodyBlock->getNamedBlock(ItemNames[ic]);
	}
}

void BuilderFileWriter::createPartialCreationFunction(const CppContainer * cppContainer, int sectionIndex)
{
	BuilderSection * section = this->sectionList->addSection(bstPartialCreationFunction, cppContainer);
	this->initializePartialCreationFunction(section->getCodeBlock(), cppContainer, sectionIndex);
}

void BuilderFileWriter::initializePartialCreationFunction(CodeBlock * codeBlock,
														  const CppContainer * cppContainer, int sectionIndex)
{
	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	string prototype = getPartialCreationFunctionPrototype(this->getConfig(), cppContainer, sectionIndex);

	string s;

	if(cppClass != NULL && cppClass->isTemplate()) {
		s = Poco::format("template <%s >", cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName));
		codeBlock->appendLine(s);
	}
	codeBlock->appendLine(prototype);

	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);
	string metaType;
	if(cppClass != NULL) {
		if(cppClass->isTemplate()) {
			metaType = Poco::format("cpgf::GDefineMetaClass<%s<%s > >", cppClass->getOutputName(), cppClass->getTextOfChainedTemplateParamList(itoWithArgName));
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
	s = Poco::format("%s(meta);",
		getReflectionFunctionName(this->getConfig(), cppContainer, sectionIndex));
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
	BuilderSection * section = this->getReflectionContainerSection(cppItem->getParent(), cppItem);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getContainerReflectionCodeBlock(const CppContainer * cppContainer)
{
	BuilderSection * section = this->getReflectionContainerSection(cppContainer, cppContainer);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppContainer->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderFileWriter::getClassWrapperCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getClassWrapperSection(cppItem->getParent());
	return section->getCodeBlock()->getNamedBlock(CodeBlockName_ClassBody, cbsBracket)
		->getNamedBlock(getTextOfVisibility(cppItem->getVisibility()))
		->getNamedBlock(CodeBlockName_Customize, cbsIndent | cbsTailEmptyLine)
	;
}

BuilderSection * BuilderFileWriter::getClassWrapperSection(const CppContainer * cppContainer)
{
	BuilderSection * section;
	ContainerSectionMapType::iterator it = this->wrapperClassSectionMap.find(cppContainer);
	if(it == this->wrapperClassSectionMap.end()) {
		section = this->sectionList->addSection(bstClassWrapper, cppContainer);
		this->wrapperClassSectionMap.insert(make_pair(cppContainer, section));

		this->initializeClassWrapperOutline(section->getCodeBlock(), cppContainer);
		this->createPartialClassWrapperCreationFunction(cppContainer, section->getIndex());
	}
	else {
		section = it->second;
	}
	return section;
}

BuilderSection * BuilderFileWriter::getClassWrapperReflectionSection(const CppContainer * cppContainer,
																	 const CppItem * payloadItem)
{
	bool isNewSection;
	BuilderSection * section = getSection(this->getConfig(), this->sectionList,
		&this->wrapperClassReflectionSectionMap, cppContainer, payloadItem, &isNewSection);
	if(isNewSection) {
		this->initializeClassWrapperReflectionOutline(section->getCodeBlock(), cppContainer, section->getIndex());
	}
	
	return section;
}

void BuilderFileWriter::initializeClassWrapperOutline(CodeBlock * codeBlock, const CppContainer * cppContainer)
{
	if(! cppContainer->isClass()) {
		GASSERT(false);
		return;
	}

	const CppClass * cppClass = static_cast<const CppClass *>(cppContainer);
	string s;
	
	if(cppClass->isTemplate()) {
		s = Poco::format("template <%s >", cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName));
		codeBlock->appendLine(s);
	}

	s = Poco::format("class %s%s : public %s, public cpgf::GScriptWrapper",
		cppClass->getName(), this->getConfig()->getClassWrapperPostfix(),
		cppClass->getOutputName()
	);
	codeBlock->appendLine(s);

	CodeBlock * bodyBlock = codeBlock->getNamedBlock(CodeBlockName_ClassBody, cbsBracket);
	bodyBlock->appendLine("private:");
	CodeBlock * superBlock = bodyBlock->appendBlock(cbsIndent | cbsTailEmptyLine);
	s = Poco::format("typedef %s super;", cppClass->getOutputName());
	superBlock->appendLine(s);
	for(int i = ivFirst; i < ivCount; ++i) {
		s = getTextOfVisibility(ItemVisibility(i));
		CodeBlock * block = bodyBlock->getNamedBlock(s, cbsTailEmptyLine);
		block->appendLine(s + ":");
		block->getNamedBlock(CodeBlockName_Customize, cbsIndent | cbsTailEmptyLine);
	}
}

void BuilderFileWriter::initializeClassWrapperReflectionOutline(CodeBlock * codeBlock,
																const CppContainer * cppContainer, int sectionIndex)
{
	this->initializeReflectionFunctionOutline(codeBlock, cppContainer,
		getReflectionFunctionName(this->getConfig(), cppContainer, sectionIndex, this->getConfig()->getClassWrapperPostfix())
		);
}

CodeBlock * BuilderFileWriter::getClassWrapperParentReflectionCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getClassWrapperReflectionSection(cppItem->getParent(), cppItem);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

void BuilderFileWriter::createPartialClassWrapperCreationFunction(const CppContainer * cppContainer, int sectionIndex)
{
	BuilderSection * section = this->sectionList->addSection(bstClassWrapperPartialCreationFunction, cppContainer);
	this->initializePartialCreationFunction(section->getCodeBlock(), cppContainer, sectionIndex);
}


} // namespace metagen
