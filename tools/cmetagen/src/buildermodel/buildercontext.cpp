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
#include "builderfilewriter.h"

#include "model/cppfile.h"
#include "model/cppclass.h"
#include "model/cppcontext.h"
#include "model/cpppolicy.h"
#include "model/cppsourcefile.h"

#include "project.h"
#include "constants.h"
#include "util.h"
#include "logger.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"
#include "Poco/Path.h"

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


BuilderContext::BuilderContext(const Project * project, const CppSourceFile & sourceFile, bool overwriteEvenIfNoChange)
	:	project(project),
		sourceFile(sourceFile),
		overwriteEvenIfNoChange(overwriteEvenIfNoChange),
		sectionList(new BuilderSectionList())
{
}

BuilderContext::~BuilderContext()
{
	clearPointerContainer(this->itemList);
	clearPointerContainer(this->fileWriterList);
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

const Project * BuilderContext::getProject() const
{
	return this->project;
}

bool BuilderContext::shouldOverwriteEvenIfNoChange() const
{
	return this->overwriteEvenIfNoChange;
}

std::string BuilderContext::getSourceFileName() const
{
	return this->sourceFile.getFileName();
}

std::string BuilderContext::getSourceBaseFileName() const
{
	return this->sourceFile.getBaseFileName();
}

BuilderContext::ItemListType * BuilderContext::getItemList()
{
	return &this->itemList;
}

BuilderSectionList * BuilderContext::getSectionList()
{
	return this->sectionList.get();
}

void BuilderContext::doProcessFile(const CppFile * cppFile)
{
	BuilderFile * file = static_cast<BuilderFile *>(this->createItem(cppFile));
	this->itemList.push_back(file);
	file->setProject(this->project);

	this->flatten(file);
	this->doPreocessByScript();

	this->generateCodeSections();
	if(this->getSectionList()->isEmpty()) {
		getLogger().warn(Poco::format("\nThere is no data to be reflected in file %s\n"
			"maybe the file contains syntax errors or the file includes itself?\n",
			string(this->sourceFile.getFileName())));
		return;
	}
	this->generateCreationFunctionSections();
	
	this->createHeaderFileWriter();
	this->createSourceFileWriters();

	for(BuilderFileWriterListType::iterator it = this->fileWriterList.begin();
		it != this->fileWriterList.end();
		++it) {
		(*it)->output();
	}
}

void BuilderContext::doPreocessByScript()
{
	for(ItemListType::iterator it = this->getItemList()->begin(); it != this->getItemList()->end(); ++it) {
		this->getProject()->processBuilderItemByScript(*it);
	}
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
	BuilderSectionListType partialCreationSections;
	
	this->doCollectPartialCreationFunctions(&partialCreationSections);

	typedef pair<const CppItem *, BuilderSectionType> PairType;
	set<PairType> generatedItemSet;
	for(BuilderSectionListType::iterator it = partialCreationSections.begin();
		it != partialCreationSections.end();
		++it) {
		BuilderSection * section = *it;
		const CppItem * cppItem = section->getCppItem();
		PairType p = make_pair(cppItem, section->getType());
		if(generatedItemSet.find(p) == generatedItemSet.end()) {
			generatedItemSet.insert(p);
			this->doGenerateCreateFunctionSection(section, it, partialCreationSections.end());
			if(shouldGenerateCreationFunction(section->getCppItem())) {
				if(! this->creationFunctionNameCode.empty()) {
					this->creationFunctionNameCode.append("\n");
				}
				string creationFunctionName = getCreationFunctionName(this, section);
				this->creationFunctionNameCode.append(creationFunctionName);
			}
		}
	}
}

void BuilderContext::doGenerateCreateFunctionSection(BuilderSection * sampleSection,
		BuilderSectionListType::iterator begin,
		BuilderSectionListType::iterator end)
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

	bodyBlock->appendLine(getMetaTypeTypedef(this, sampleSection, NULL));
	bodyBlock->appendBlankLine();

	if(sampleContainer->isClass()) {
		const CppClass * cppClass = static_cast<const CppClass *>(sampleContainer);
		string className;
		if(sampleSection->isClassWrapper()) {
			className = getClassWrapperClassName(this, cppClass);
		}
		else {
			className = cppClass->getName();
		}
		
		CppPolicy cppPolicy;
		cppClass->getPolicy(&cppPolicy);

		if(cppPolicy.hasRule()) {
			bodyBlock->appendLine(Poco::format("%s _d = %s::Policy<%s >::declare(\"%s\");",
				metaTypeTypeDefName,
				metaTypeTypeDefName,
				cppPolicy.getTextOfMakePolicy(false),
				className
				)
			);
		}
		else {
			bodyBlock->appendLine(Poco::format("%s _d = %s::declare(\"%s\");",
				metaTypeTypeDefName,
				metaTypeTypeDefName,
				className
				)
			);
		}
	}
	else {
		bodyBlock->appendLine("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();");
	}

	bodyBlock->appendLine("cpgf::GDefineMetaInfo meta = _d.getMetaInfo();");
	bodyBlock->appendBlankLine();

	for(BuilderSectionListType::iterator it = begin; it != end; ++it) {
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

void BuilderContext::createHeaderFileWriter()
{
	BuilderFileWriter * fileWriter = BuilderFileWriter::createHeaderFile(this->sourceFile, this);
	this->fileWriterList.push_back(fileWriter);

	for(BuilderSectionList::iterator it = this->getSectionList()->begin();
		it != this->getSectionList()->end();
		++it) {
		BuilderSection * section = *it;
		if(! section->shouldBeInSourceFile()) {
			fileWriter->addSection(section);
		}
	}
	fileWriter->setCreationFunctionNameCode(this->creationFunctionNameCode);
}

void BuilderContext::createSourceFileWriters()
{
	BuilderSectionListType partialCreationSections;
	
	this->doCollectPartialCreationFunctions(&partialCreationSections);

	std::sort(partialCreationSections.begin(), partialCreationSections.end(), &partialCreationSectionComparer);

	this->doCreateSourceFileWriters(&partialCreationSections);
}

void BuilderContext::doCollectPartialCreationFunctions(BuilderSectionListType * partialCreationSections)
{
	for(BuilderSectionList::iterator it = this->getSectionList()->begin();
		it != this->getSectionList()->end();
		++it) {
		BuilderSection * section = *it;
		if(section->isPartialCreationFunction()) {
			const CppItem * cppItem = section->getCppItem();
			if(cppItem->isClass()
				&& static_cast<const CppClass *>(cppItem)->isChainedTemplate()
				&& section->getTemplateInstantiation() == NULL) {
				continue;
			}
			partialCreationSections->push_back(section);
			GASSERT(section->getCppItem()->isContainer());
		}
	}
}

void BuilderContext::doCreateSourceFileWriters(BuilderSectionListType * partialCreationSections)
{
	int fileIndex = 0;
	while(! partialCreationSections->empty()) {
		BuilderSectionListType sectionsInOneFile;
		this->doExtractPartialCreationFunctions(partialCreationSections, &sectionsInOneFile);
		BuilderFileWriter * fileWriter = BuilderFileWriter::createSourceFile(this->sourceFile, this, fileIndex);
		this->fileWriterList.push_back(fileWriter);
		if(fileIndex == 0) {
			for(BuilderSectionList::iterator it = this->sectionList->begin();
				it != this->sectionList->end();
				++it) {
				if((*it)->isCreationFunctionDefinition()) {
					fileWriter->addSection(*it);
				}
			}
		}
		++fileIndex;
		for(BuilderSectionListType::iterator it = sectionsInOneFile.begin();
			it != sectionsInOneFile.end();
			++it) {
			fileWriter->addSection(*it);
		}
	}
}

void BuilderContext::doExtractPartialCreationFunctions(BuilderSectionListType * partialCreationSections,
	BuilderSectionListType * outputSections)
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
		for(BuilderSectionListType::iterator it = partialCreationSections->begin();
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

void BuilderContext::flatten(BuilderContainer * builderContainer)
{
	for(CppContainer::ItemListType::const_iterator it = builderContainer->getCppContainer()->getItemList()->begin();
		it != builderContainer->getCppContainer()->getItemList()->end(); ++it) {
		if((*it)->isInMainFile() || (*it)->isNamespace()) {
			cpgf::GScopedPointer<BuilderItem> item(this->createItem(*it));

			BuilderItem * itemPointer = item.get();
			this->itemList.push_back(item.take());
			builderContainer->addItem(itemPointer);
			if((*it)->isContainer()) {
				this->flatten(static_cast<BuilderContainer *>(itemPointer));
			}
		}
	}
}


} // namespace metagen

