#include "util.h"

#include <algorithm>


std::string normalizePath(const std::string & path)
{
	std::string result(path);
	std::replace(result.begin(), result.end(), '\\', '/');
	return result;
}
