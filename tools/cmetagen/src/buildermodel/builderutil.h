#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>

namespace metagen {

class CppContainer;
class Project;
class BuilderContext;

enum CodeNameType {
	cntNormal, cntClassWrapper
};

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
std::string getReflectionFunctionName(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index);


} // namespace metagen


#endif
