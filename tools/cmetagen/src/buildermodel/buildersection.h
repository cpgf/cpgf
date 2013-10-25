#ifndef CPGF_BUILDERSECTION_H
#define CPGF_BUILDERSECTION_H

#include "cpgf/gscopedptr.h"

#include <deque>


namespace metagen {

class CodeBlock;
class CppItem;
class BuilderTemplateInstantiation;

enum BuilderSectionType {
	bstOperatorWrapperFunction = 0,
	bstBitFieldWrapperFunction,
	bstReflectionFunction,
	bstPartialCreationFunction,

	bstClassWrapperBegin,
	bstClassWrapper = bstClassWrapperBegin,
	bstClassWrapperReflectionFunction,
	bstClassWrapperPartialCreationFunction,
	bstClassWrapperEnd = bstClassWrapperPartialCreationFunction,

	bstCreationFunctionDeclaration, // in header
	bstCreationFunctionDefinition // in source
};

class BuilderSection
{
public:
	explicit BuilderSection(BuilderSectionType type, const CppItem * cppItem);
	
	CodeBlock * getCodeBlock() const;
	BuilderSectionType getType() const;

	void addPayload(size_t payload);	
	size_t getTotalPayload() const;

	void setIndex(size_t index);	
	size_t getIndex() const;

	bool isClassWrapper() const;
	bool isPartialCreationFunction() const;
	bool isCreationFunctionDefinition() const;
	bool shouldBeInSourceFile() const;

	const CppItem * getCppItem() const;

	const BuilderTemplateInstantiation * getTemplateInstantiation() const;
	void setTemplateInstantiation(const BuilderTemplateInstantiation * templateInstantiation);
	
	BuilderSection * getRelateSection() const;
	void setRelateSection(BuilderSection * relateSection);
	
private:
	BuilderSectionType type;
	const CppItem * cppItem;
	size_t totalPayload;
	size_t index;
	cpgf::GScopedPointer<CodeBlock> codeBlock;
	const BuilderTemplateInstantiation * templateInstantiation;
	BuilderSection * relateSection;
};

typedef std::deque<BuilderSection *> BuilderSectionListType;

class BuilderSectionList
{
public:
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
	bool isEmpty() const { return this->sectionList.empty(); }

private:
	BuilderSectionListType sectionList;
};

void sortSectionList(BuilderSectionListType * sectionList);


} // namespace metagen


#endif
