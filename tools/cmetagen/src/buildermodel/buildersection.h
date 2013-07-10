#ifndef __BUILDERSECTION_H
#define __BUILDERSECTION_H

#include "cpgf/gscopedptr.h"

#include <vector>


namespace metagen {

class CodeBlock;
class CppItem;

enum BuilderSectionType {
	bstOperatorWrapperFunction = 0,
	bstBitFieldWrapperFunction = 1,
};

class BuilderSection
{
public:
	explicit BuilderSection(BuilderSectionType type, const CppItem * cppItem);
	
	CodeBlock * getCodeBlock() const;

private:
	BuilderSectionType type;
	const CppItem * cppItem;
	cpgf::GScopedPointer<CodeBlock> codeBlock;
};

class BuilderSectionList
{
public:
	typedef std::vector<BuilderSection *> BuilderSectionListType;
	typedef BuilderSectionListType::iterator iterator;
	typedef BuilderSectionListType::const_iterator const_iterator;

public:
	BuilderSectionList();
	~BuilderSectionList();

	BuilderSection * addSection(BuilderSectionType type, const CppItem * cppItem);

public:
	iterator begin() { return this->sectionList.begin(); }
	const_iterator begin() const { return this->sectionList.begin(); }
	iterator end() { return this->sectionList.end(); }
	const_iterator end() const { return this->sectionList.end(); }

private:
	BuilderSectionListType sectionList;
};


} // namespace metagen


#endif
