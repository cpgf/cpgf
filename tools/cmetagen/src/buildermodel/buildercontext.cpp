#include "buildercontext.h"
#include "builderclass.h"
#include "builderconstructor.h"
#include "buildercontainer.h"
#include "builderdestructor.h"
#include "builderenum.h"
#include "builderfield.h"
#include "builderfile.h"
#include "builderitem.h"
#include "buildermethod.h"
#include "buildernamespace.h"
#include "builderoperator.h"
#include "builderutil.h"
#include "buildersection.h"
#include "builderwriter.h"

#include "model/cppfile.h"
#include "model/cppclass.h"
#include "model/cppcontext.h"
#include "project.h"
#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

// test
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"

#include <algorithm>

using namespace std;


namespace metagen {


BuilderItem * createBuilderItem(const CppItem * cppItem)
{
	switch(cppItem->getCategory()) {
		case icFile:
			return new BuilderFile(cppItem);

		case icNamespace:
			return new BuilderNamespace(cppItem);

		case icClass:
			return new BuilderClass(cppItem);

		case icField:
			return new BuilderField(cppItem);

		case icMethod:
			return new BuilderMethod(cppItem);

		case icEnum:
			return new BuilderEnum(cppItem);

		case icOperator:
			return new BuilderOperator(cppItem);

		case icConstructor:
			return new BuilderConstructor(cppItem);

		case icDestructor:
			return new BuilderDestructor(cppItem);

		default:
			GASSERT(false);
			break;
	}
	
	return NULL;
}


BuilderContext::BuilderContext(const Project * project, const std::string & sourceFileName)
	:	project(project),
		sourceFileName(normalizePath(sourceFileName)),
		sectionList(new BuilderSectionList())
{
	this->sourceBaseFileName = this->sourceFileName.getBaseName();
}

BuilderContext::~BuilderContext()
{
	clearPointerContainer(this->itemList);
}

BuilderItem * BuilderContext::createItem(const CppItem * cppItem)
{
	BuilderItem * builderItem = createBuilderItem(cppItem);
	builderItem->setProject(this->project);
	return builderItem;
}

void BuilderContext::process(const CppContext * cppContext)
{
	this->doProcessFile(cppContext->getCppFile());
}

void BuilderContext::doProcessFile(const CppFile * cppFile)
{
	BuilderFile * file = static_cast<BuilderFile *>(this->createItem(cppFile));
	this->itemList.push_back(file);
	file->setProject(this->project);

	this->flatten(file);

	this->generateCodeSections();
	this->generateCreationFunctionSections();
	this->generateFilePartitions();

	this->getSectionList()->sort();
	this->getSectionList()->dump();
}

void BuilderContext::generateCodeSections()
{
	BuilderWriter builderWriter(this);
	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		(*it)->writeMetaData(&builderWriter);
	}
}

void BuilderContext::generateCreationFunctionSections()
{
	TempBuilderSectionListType partialCreationSections;
	
	this->doCollectPartialCreationFunctions(&partialCreationSections);

	typedef pair<const CppItem *, BuilderSectionType> PairType;
	set<PairType> generatedItemSet;
	for(TempBuilderSectionListType::iterator it = partialCreationSections.begin();
		it != partialCreationSections.end();
		++it) {
		BuilderSection * section = *it;
		const CppItem * cppItem = section->getCppItem();
		PairType p = make_pair(cppItem, section->getType());
		if(generatedItemSet.find(p) == generatedItemSet.end()) {
			generatedItemSet.insert(p);
			this->doGenerateCreateFunctionSection(section, it, partialCreationSections.end());
		}
	}
}

void BuilderContext::doGenerateCreateFunctionSection(BuilderSection * sampleSection,
		TempBuilderSectionListType::iterator begin,
		TempBuilderSectionListType::iterator end)
{
	const CppContainer * sampleContainer = static_cast<const CppContainer *>(sampleSection->getCppItem());

	BuilderSection * declarationSection = this->sectionList->addSection(bstCreationFunctionDeclaration, sampleContainer);
	declarationSection->getCodeBlock()->appendLine(
		Poco::format("%s;", getCreationFunctionPrototype(this, sampleSection))
	);

	BuilderSection * definitionSection = this->sectionList->addSection(bstCreationFunctionDefinition, sampleContainer);
	CodeBlock * forwardDeclarationBlock = definitionSection->getCodeBlock()->appendBlock();
	CodeBlock * creationBlock = definitionSection->getCodeBlock()->appendBlock();
	CodeBlock * headerBlock = creationBlock->getNamedBlock(CodeBlockName_FunctionHeader);
	CodeBlock * bodyBlock = creationBlock->getNamedBlock(CodeBlockName_FunctionBody, cbsBracketAndIndent);

	headerBlock->appendLine(getCreationFunctionPrototype(this, sampleSection));

	bodyBlock->appendLine("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();");
	bodyBlock->appendLine("cpgf::GDefineMetaInfo meta = _d.getMetaInfo();");
	bodyBlock->appendBlankLine();

	for(TempBuilderSectionListType::iterator it = begin; it != end; ++it) {
		BuilderSection * currentSection = *it;
		const CppContainer * currentContainer = static_cast<const CppContainer *>(currentSection->getCppItem());
		if(currentContainer == sampleContainer && currentSection->getType() == sampleSection->getType()) {
			forwardDeclarationBlock->appendLine(
				Poco::format("%s;", getPartialCreationFunctionPrototype(this, currentSection))
			);
			bodyBlock->appendLine(
				Poco::format("%s(meta);", getPartialCreationFunctionName(this, currentSection))
			);
		}
	}

	bodyBlock->appendBlankLine();
	bodyBlock->appendLine("return meta;");
}

bool partialCreationSectionComparer(BuilderSection * a, BuilderSection * b)
{
	return a->getTotalPayload() > b->getTotalPayload();
}

void BuilderContext::generateFilePartitions()
{
	TempBuilderSectionListType partialCreationSections;
	
	this->doCollectPartialCreationFunctions(&partialCreationSections);

	std::sort(partialCreationSections.begin(), partialCreationSections.end(), &partialCreationSectionComparer);

	this->doGenerateFilePartitions(&partialCreationSections);
}

void BuilderContext::doCollectPartialCreationFunctions(TempBuilderSectionListType * partialCreationSections)
{
	for(BuilderSectionList::iterator it = this->getSectionList()->begin();
		it != this->getSectionList()->end();
		++it) {
		BuilderSection * section = *it;
		if(section->isPartialCreationFunction()) {
			const CppItem * cppItem = section->getCppItem();
			if(cppItem->isClass()
				&& static_cast<const CppClass *>(cppItem)->isTemplate()
				&& section->getTemplateInstantiation() == NULL) {
				continue;
			}
			partialCreationSections->push_back(section);
			GASSERT(section->getCppItem()->isContainer());
		}
	}
}

void BuilderContext::doGenerateFilePartitions(TempBuilderSectionListType * partialCreationSections)
{
	while(! partialCreationSections->empty()) {
		TempBuilderSectionListType sectionsInOneFile;
		this->doExtractPartialCreationFunctions(partialCreationSections, &sectionsInOneFile);
		printf("TTTTTTTTTTTT \n");
		for(TempBuilderSectionListType::iterator it = sectionsInOneFile.begin();
			it != sectionsInOneFile.end();
			++it) {
		CodeWriter codeWriter;
		(*it)->getCodeBlock()->write(&codeWriter);
		printf("%s\n\n", codeWriter.getText().c_str());
		}
	}
}

void BuilderContext::doExtractPartialCreationFunctions(TempBuilderSectionListType * partialCreationSections,
	TempBuilderSectionListType * outputSections)
{
	if(partialCreationSections->empty()) {
		return;
	}
	
	const size_t maxItemCountPerFile = this->getProject()->getMaxItemCountPerFile();
	size_t totalPayload = partialCreationSections->front()->getTotalPayload();

	outputSections->push_back(partialCreationSections->front());
	partialCreationSections->pop_front();

	bool found = true;
	while(found) {
		found = false;
		for(TempBuilderSectionListType::iterator it = partialCreationSections->begin();
			it != partialCreationSections->end();
			) {
			if(maxItemCountPerFile == 0
				||  totalPayload + (*it)->getTotalPayload() <= maxItemCountPerFile) {
				found = true;
				totalPayload += (*it)->getTotalPayload();
				outputSections->push_back(*it);
				it = partialCreationSections->erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void BuilderContext::flatten(BuilderFile * file)
{
	this->doFlatten(file, file);
}

void BuilderContext::doFlatten(BuilderFile * file, BuilderContainer * builderContainer)
{
	for(CppContainer::ItemListType::const_iterator it = builderContainer->getCppContainer()->getItemList()->begin();
		it != builderContainer->getCppContainer()->getItemList()->end(); ++it) {
		if(! (*it)->isInMainFile()) {
			continue;
		}
		if(this->shouldSkipItem(*it)) {
			continue;
		}

		cpgf::GScopedPointer<BuilderItem> item(this->createItem(*it));
		if(! item->canBind()) {
			continue;
		}
		if(item->shouldSkipBind()) {
			continue;
		}

		BuilderItem * itemPointer = item.get();
		this->itemList.push_back(item.take());
		builderContainer->addItem(itemPointer);
		if((*it)->isContainer()) {
			this->doFlatten(file, static_cast<BuilderContainer *>(itemPointer));
		}
	}
}

bool BuilderContext::shouldSkipItem(const CppItem * cppItem)
{
	return ! isVisibilityAllowed(cppItem->getVisibility(), this->project);
}


} // namespace metagen

