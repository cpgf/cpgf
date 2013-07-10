#include "buildersection.h"
#include "codewriter/codeblock.h"
#include "util.h"

namespace metagen {


BuilderSection::BuilderSection(BuilderSectionType type, const CppItem * cppItem)
	: type(type), cppItem(cppItem), codeBlock(new CodeBlock)
{
}

CodeBlock * BuilderSection::getCodeBlock() const
{
	return this->codeBlock.get();
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


} // namespace metagen


