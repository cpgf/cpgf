#ifndef __BUILDERUTIL_H
#define __BUILDERUTIL_H

#include <string>


class Config;
class CppItem;

std::string getReflectionClassName(const Config * config);
std::string getReflectionScope(const CppItem * item);


#endif
