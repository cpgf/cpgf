#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>

namespace metagen {

class CppContainer;
class CodeBlock;
class Project;
class BuilderContext;
class BuilderSection;
class BuilderTemplateInstantiation;

enum CodeNameType {
	cntNormal, cntClassWrapper
};

const std::string CodeBlockName_ReflectionArea("reflection");
const std::string CodeBlockName_FunctionHeader("fheader");
const std::string CodeBlockName_FunctionBody("fbody");
const std::string CodeBlockName_ClassBody("cbody");
const std::string CodeBlockName_Customize("customize");

std::string getTextOfVisibility(ItemVisibility visibility);

std::string getReflectionClassName(const Project * project);
std::string getReflectionScope(const CppItem * item);
bool isVisibilityAllowed(ItemVisibility visibility, const Project * project);

// A payload is how many meta data the item contribute
size_t getCppItemPayload(const CppItem * item);

std::string getSectionIndexName(int sectionIndex);
std::string getContainerQualifiedName(const BuilderContext * builderContext, const CppContainer * cppContainer);
std::string getClassWrapperClassQualifiedName(const BuilderContext * builderContext, const CppContainer * cppContainer);
std::string getClassWrapperClassName(const BuilderContext * builderContext, const CppContainer * cppContainer);
std::string getContainerOrClassWrapperClassName(CodeNameType nameType, const BuilderContext * builderContext, const CppContainer * cppContainer);
std::string getContainerOrClassWrapperQualifiedName(const BuilderContext * builderContext, BuilderSection * section);

std::string getPartialCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section);
std::string getPartialCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section);

std::string getCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section);
std::string getCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section);

std::string getReflectionFunctionName(const BuilderContext * builderContext, BuilderSection * section);

CodeNameType getNameTypeFromBuilderSection(BuilderSection * section);

void initializeReflectionFunctionOutline(const BuilderContext * builderContext, BuilderSection * section);

void initializePartialCreationFunction(const BuilderContext * builderContext, BuilderSection * section,
		BuilderTemplateInstantiation * templateInstantiation);

void initializeClassWrapperOutline(const BuilderContext * builderContext, BuilderSection * section);


} // namespace metagen


#endif
