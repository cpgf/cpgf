#ifndef CPGF_BUILDERUTIL_H
#define CPGF_BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>
#include <set>

namespace metagen {

class CppContainer;
class CppClass;
class CodeBlock;
class Project;
class BuilderContext;
class BuilderSection;

enum CodeNameType {
	cntNormal, cntClassWrapper
};

const std::string CodeBlockName_ReflectionArea("reflection");
const std::string CodeBlockName_FunctionHeader("fheader");
const std::string CodeBlockName_FunctionBody("fbody");
const std::string CodeBlockName_ClassBody("cbody");
const std::string CodeBlockName_Customize("customize");

std::string getTextOfVisibility(ItemVisibility visibility);

std::string getReflectionClassName(const Project * project, bool asType);
std::string getReflectionScope(const CppItem * item, bool asType);

// A payload is how many meta data the item contribute
size_t getCppItemPayload(const CppItem * item);

std::string getClassWrapperClassName(const BuilderContext * builderContext, const CppContainer * cppContainer);

std::string getPartialCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section);
std::string getPartialCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section);

std::string getCppClassNormalizedSymboName(const CppClass * cppClass);
std::string getCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section);
std::string getCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section);
std::string getCreationFunctionPrototype(const std::string & functionName);

std::string getReflectionFunctionName(const BuilderContext * builderContext, BuilderSection * section);

CodeNameType getNameTypeFromBuilderSection(BuilderSection * section);

std::string getMetaTypeTypedef(const BuilderContext * builderContext,
	 BuilderSection * section, std::string * outReflectionTemplateParams);

void initializeReflectionFunctionOutline(const BuilderContext * builderContext, BuilderSection * section);
void initializePartialCreationFunction(const BuilderContext * builderContext, BuilderSection * section);
void initializeClassWrapperOutline(const BuilderContext * builderContext, BuilderSection * section);

bool shouldGenerateCreationFunction(const CppItem * cppItem);

void generateMainRegisterFiles(const std::set<std::string> & creationFunctionNameList, const Project * project);


} // namespace metagen


#endif
