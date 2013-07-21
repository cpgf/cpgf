#include "builderwriter.h"
#include "builderitem.h"
#include "buildersection.h"
#include "buildercontext.h"
#include "builderutil.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "project.h"
#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

#include <string>

using namespace std;

namespace metagen {


BuilderWriter::BuilderWriter(BuilderContext * builderContext)
	: builderContext(builderContext)
{
}

BuilderWriter::~BuilderWriter()
{
}

const Project * BuilderWriter::getProject() const
{
	return this->builderContext->getProject();
}

BuilderContext * BuilderWriter::getBuilderContext()
{
	return this->builderContext;
}

std::string BuilderWriter::getReflectionAction(const std::string & name)
{
	return "_d.CPGF_MD_TEMPLATE " + name;
}

BuilderSection * getSection(const Project * project, BuilderSectionList * sectionList,
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
			if(project->shouldSplitFile()
				&& it->second->getTotalPayload() + payload > project->getMaxItemCountPerFile()) {
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

BuilderSection * BuilderWriter::getReflectionContainerSection(const CppContainer * cppContainer, const CppItem * payloadItem)
{
	bool isNewSection;
	BuilderSection * section = getSection(this->getProject(), this->getSectionList(),
		&this->reflectionSectionMap, cppContainer, payloadItem, &isNewSection);
	if(isNewSection) {
		initializeReflectionFunctionOutline(section->getCodeBlock(), this->getBuilderContext(),
			cppContainer, section->getIndex());
		this->createPartialCreationFunction(cppContainer, section->getIndex());
	}
	
	return section;
}

CodeBlock * BuilderWriter::getReflectionBodyBlock(CodeBlock * codeBlock)
{
	return codeBlock->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent)->getNamedBlock(CodeBlockName_Customize);
}

void BuilderWriter::createPartialCreationFunction(const CppContainer * cppContainer, int sectionIndex)
{
	BuilderSection * section = this->getSectionList()->addSection(bstPartialCreationFunction, cppContainer);
	section->setIndex(sectionIndex);
	initializePartialCreationFunction(cntNormal, this->getBuilderContext(), section->getCodeBlock(),
		cppContainer, sectionIndex, NULL);
}

CodeBlock * BuilderWriter::createOperatorWrapperCodeBlock(const CppItem * cppItem)
{
	return this->getSectionList()->addSection(bstOperatorWrapperFunction, cppItem)->getCodeBlock();
}

CodeBlock * BuilderWriter::createBitFieldWrapperCodeBlock(const CppItem * cppItem)
{
	return this->getSectionList()->addSection(bstBitFieldWrapperFunction, cppItem)->getCodeBlock();
}

CodeBlock * BuilderWriter::getParentReflectionCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getReflectionContainerSection(cppItem->getParent(), cppItem);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderWriter::getContainerReflectionCodeBlock(const CppContainer * cppContainer)
{
	BuilderSection * section = this->getReflectionContainerSection(cppContainer, cppContainer);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppContainer->getCategory()], cbsTailEmptyLine);
}

CodeBlock * BuilderWriter::getClassWrapperCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getClassWrapperSection(cppItem->getParent());
	return section->getCodeBlock()->getNamedBlock(CodeBlockName_ClassBody, cbsBracket)
		->getNamedBlock(getTextOfVisibility(cppItem->getVisibility()))
		->getNamedBlock(CodeBlockName_Customize, cbsIndent | cbsTailEmptyLine)
	;
}

BuilderSection * BuilderWriter::getClassWrapperSection(const CppContainer * cppContainer)
{
	BuilderSection * section;
	ContainerSectionMapType::iterator it = this->wrapperClassSectionMap.find(cppContainer);
	if(it == this->wrapperClassSectionMap.end()) {
		section = this->getSectionList()->addSection(bstClassWrapper, cppContainer);
		this->wrapperClassSectionMap.insert(make_pair(cppContainer, section));

		initializeClassWrapperOutline(section->getCodeBlock(), this->getBuilderContext(), cppContainer);
		this->createPartialClassWrapperCreationFunction(cppContainer, section->getIndex());
	}
	else {
		section = it->second;
	}
	return section;
}

BuilderSection * BuilderWriter::getClassWrapperReflectionSection(const CppContainer * cppContainer,
		const CppItem * payloadItem)
{
	bool isNewSection;
	BuilderSection * section = getSection(this->getProject(), this->getSectionList(),
		&this->wrapperClassReflectionSectionMap, cppContainer, payloadItem, &isNewSection);
	if(isNewSection) {
		initializeClassWrapperReflectionOutline(section->getCodeBlock(), this->getBuilderContext(),
			cppContainer, section->getIndex());
	}

	return section;
}

CodeBlock * BuilderWriter::getClassWrapperParentReflectionCodeBlock(const CppItem * cppItem)
{
	BuilderSection * section = this->getClassWrapperReflectionSection(cppItem->getParent(), cppItem);
	return this->getReflectionBodyBlock(section->getCodeBlock())->getNamedBlock(ItemNames[cppItem->getCategory()], cbsTailEmptyLine);
}

void BuilderWriter::createPartialClassWrapperCreationFunction(const CppContainer * cppContainer, int sectionIndex)
{
	BuilderSection * section = this->getSectionList()->addSection(bstClassWrapperPartialCreationFunction, cppContainer);
	initializePartialCreationFunction(cntClassWrapper, this->getBuilderContext(), section->getCodeBlock(),
		cppContainer, sectionIndex, NULL);
}

BuilderSectionList * BuilderWriter::getSectionList()
{
	return this->builderContext->getSectionList();
}


} // namespace metagen
