#include "buildersection.h"
#include "codewriter/codeblock.h"
#include "codewriter/codewriter.h"
#include "util.h"

#include <algorithm>


namespace metagen {


BuilderSection::BuilderSection(BuilderSectionType type, const CppItem * cppItem)
	: type(type), cppItem(cppItem), totalPayload(0), index(0), codeBlock(new CodeBlock), relateSection(NULL)
{
}

CodeBlock * BuilderSection::getCodeBlock() const
{
	return this->codeBlock.get();
}

BuilderSectionType BuilderSection::getType() const
{
	return this->type;
}

void BuilderSection::addPayload(size_t payload)
{
	this->totalPayload += payload;
}

size_t BuilderSection::getTotalPayload() const
{
	if(this->totalPayload == 0) {
		if(this->relateSection != NULL) {
			return this->relateSection->getTotalPayload();
		}
	}
	
	return this->totalPayload;
}

void BuilderSection::setIndex(size_t index)
{
	this->index = index;
}

size_t BuilderSection::getIndex() const
{
	return this->index;
}

bool BuilderSection::isClassWrapper() const
{
	return this->getType() >= bstClassWrapperBegin && this->getType() <= bstClassWrapperEnd;
}

bool BuilderSection::isPartialCreationFunction() const
{
	return this->getType() == bstPartialCreationFunction
		|| this->getType() == bstClassWrapperPartialCreationFunction;
}

const CppItem * BuilderSection::getCppItem() const
{
	return this->cppItem;
}

const BuilderTemplateInstantiation * BuilderSection::getTemplateInstantiation() const
{
	return this->templateInstantiation;
}

void BuilderSection::setTemplateInstantiation(const BuilderTemplateInstantiation * templateInstantiation)
{
	this->templateInstantiation = templateInstantiation;
}

BuilderSection * BuilderSection::getRelateSection() const
{
	return this->relateSection;
}

void BuilderSection::setRelateSection(BuilderSection * relateSection)
{
	this->relateSection = relateSection;
}


BuilderSectionList::BuilderSectionList()
{
}

BuilderSectionList::~BuilderSectionList()
{
	clearPointerContainer(this->sectionList);
}

BuilderSection * BuilderSectionList::addSection(BuilderSectionType type, const CppItem * cppItem)
{
	BuilderSection * section = new BuilderSection(type, cppItem);
	this->sectionList.push_back(section);
	return section;
}

bool sortSectionComparer(BuilderSection * a, BuilderSection * b)
{
	if(a->getType() < b->getType()) {
		return true;
	}
	if(a->getType() > b->getType()) {
		return false;
	}
	if(a->getCppItem() != b->getCppItem()) {
		return a->getCppItem() < b->getCppItem();
	}
	return a->getIndex() < b->getIndex();
}

void BuilderSectionList::sort()
{
	std::sort(this->sectionList.begin(), this->sectionList.end(), &sortSectionComparer);
}

void BuilderSectionList::dump()
{
	printf("Section list begin \n");
	for(BuilderSectionList::iterator it = this->begin(); it != this->end(); ++it) {
		CodeWriter codeWriter;
		(*it)->getCodeBlock()->write(&codeWriter);
		printf("%s", codeWriter.getText().c_str());
	}
	printf("Section list end \n");
}


} // namespace metagen


