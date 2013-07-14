#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>

namespace metagen {

class CppContainer;
class Config;

std::string getReflectionClassName(const Config * config);
std::string getReflectionScope(const CppItem * item);
bool isVisibilityAllowed(ItemVisibility visibility, const Config * config);

// A payload is how many meta data the item contribute
size_t getCppItemPayload(const CppItem * item);

std::string getSectionIndexName(int sectionIndex);
std::string getContainertName(const CppContainer * cppContainer);
std::string getPartialCreationFunctionName(const Config * config, const CppContainer * cppContainer, int index);
std::string getPartialCreationFunctionPrototype(const Config * config, const CppContainer * cppContainer, int index);
std::string getReflectionFunctionName(const Config * config,
					const CppContainer * cppContainer, int index, const std::string & postfix = "");


} // namespace metagen


#endif
