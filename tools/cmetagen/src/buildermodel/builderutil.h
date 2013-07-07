#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include "model/cppitem.h"

#include <string>


class Config;

std::string getReflectionClassName(const Config * config);
std::string getReflectionScope(const CppItem * item);
bool isVisibilityAllowed(ItemVisibility visibility, const Config * config);

#endif
