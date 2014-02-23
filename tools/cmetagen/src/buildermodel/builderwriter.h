#ifndef CPGF_BUILDERWRITER_H
#define CPGF_BUILDERWRITER_H

#include "cpgf/gscopedptr.h"
#include "builderutil.h"

#include <string>
#include <vector>
#include <set>
#include <map>

namespace metagen {


class CppItem;
class CppContainer;
class CodeBlock;
class BuilderItem;
class BuilderSection;
class BuilderSectionList;
class BuilderContext;
class BuilderTemplateInstantiation;
class Project;


class BuilderWriter
{
private:
	typedef std::map<const CppContainer *, BuilderSection *> ContainerSectionMapType;
	typedef std::multimap<const CppContainer *, BuilderSection *> ReflectionSectionMultimapType;

public:
	explicit BuilderWriter(BuilderContext * builderContext);
	~BuilderWriter();
	
	const Project * getProject() const;
	BuilderContext * getBuilderContext();

public: // auxiliary functions used by BuilderItem's
	CodeBlock * createOperatorWrapperCodeBlock(const CppItem * cppItem);
	CodeBlock * createBitFieldWrapperCodeBlock(const CppItem * cppItem);
	CodeBlock * getParentReflectionCodeBlock(const CppItem * cppItem, BuilderSection ** outSection = NULL);
	CodeBlock * getContainerReflectionCodeBlock(const CppContainer * cppContainer);
	
	CodeBlock * getClassWrapperCodeBlock(const CppItem * cppItem, const CppContainer * container);
	CodeBlock * getClassWrapperParentReflectionCodeBlock(const CppItem * cppItem, const CppContainer * container);

	std::string getReflectionAction(const std::string & name);

private:
	BuilderSection * getReflectionContainerSection(const CppContainer * cppContainer, const CppItem * payloadItem);
	CodeBlock * getReflectionBodyBlock(CodeBlock * codeBlock);
	void createPartialCreationFunction(const CppContainer * cppContainer, BuilderSection * reflectionSection);
	void doCreatePartialCreationFunction(const CppContainer * cppContainer, BuilderSection * reflectionSection,
		BuilderTemplateInstantiation * templateInstantiation);

	BuilderSection * getClassWrapperSection(const CppContainer * cppContainer);
	BuilderSection * getClassWrapperReflectionSection(const CppContainer * cppContainer, const CppItem * payloadItem);

	void doWriteReflectionFunction(const CppContainer * cppContainer);

	BuilderSectionList * getSectionList();

private:
	BuilderContext * builderContext;
	ReflectionSectionMultimapType reflectionSectionMap;
	// section map for wrapper class declaration
	ContainerSectionMapType wrapperClassSectionMap;
	// section map for wrapper class reflection function
	ReflectionSectionMultimapType wrapperClassReflectionSectionMap;
};


} // namespace metagen


#endif
