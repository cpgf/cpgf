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
std::string getContainerOrClassWrapperQualifiedName(CodeNameType nameType, const BuilderContext * builderContext, const CppContainer * cppContainer);

std::string getPartialCreationFunctionName(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index);
std::string getPartialCreationFunctionPrototype(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index);

std::string getCreationFunctionName(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer);
std::string getCreationFunctionPrototype(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer);

std::string getReflectionFunctionName(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index);

CodeNameType getNameTypeFromBuilderSection(BuilderSection * section);

void initializeReflectionFunctionOutline(CodeBlock * codeBlock, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int sectionIndex);
void initializeReflectionFunctionOutline(CodeBlock * codeBlock, const BuilderContext * builderContext,
		const CppContainer * cppContainer, const std::string & functionName);

void initializePartialCreationFunction(CodeNameType nameType, const BuilderContext * builderContext,
		CodeBlock * codeBlock, const CppContainer * cppContainer, int sectionIndex,
		BuilderTemplateInstantiation * templateInstantiation);

void initializeClassWrapperOutline(CodeBlock * codeBlock, const BuilderContext * builderContext,
		const CppContainer * cppContainer);
void initializeClassWrapperReflectionOutline(CodeBlock * codeBlock, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int sectionIndex);


} // namespace metagen


#endif
