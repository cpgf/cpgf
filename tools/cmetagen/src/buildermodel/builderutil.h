#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>

namespace metagen {

class CppContainer;
class Project;
class BuilderContext;

std::string getReflectionClassName(const Project * project);
std::string getReflectionScope(const CppItem * item);
bool isVisibilityAllowed(ItemVisibility visibility, const Project * project);

// A payload is how many meta data the item contribute
size_t getCppItemPayload(const CppItem * item);

std::string getSectionIndexName(int sectionIndex);
std::string getContainertName(const BuilderContext * builderContext, const CppContainer * cppContainer);
std::string getPartialCreationFunctionName(const BuilderContext * builderContext, const CppContainer * cppContainer, int index);
std::string getPartialCreationFunctionPrototype(const BuilderContext * builderContext, const CppContainer * cppContainer, int index);
std::string getReflectionFunctionName(const BuilderContext * builderContext,
					const CppContainer * cppContainer, int index);
std::string getClassWrapperReflectionFunctionName(const BuilderContext * builderContext,
					const CppContainer * cppContainer, int index);
std::string getClassWrapperClassName(const BuilderContext * builderContext, const CppContainer * cppContainer);


} // namespace metagen


#endif
