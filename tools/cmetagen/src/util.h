#ifndef __UTIL_H
#define __UTIL_H

#include "cpgf/gflags.h"

#include <string>

namespace metagen {


template <typename T>
void clearPointerContainer(T & container)
{
	for(typename T::iterator it = container.begin(); it != container.end(); ++it) {
		delete *it;
	}
}

std::string normalizeSymbolName(const std::string & name);

// Replace all '\' with '/'
std::string normalizePath(const std::string & path);
std::string makeRelativePath(const std::string & base, const std::string & path);
bool readStringFromFile(const std::string & fileName, std::string * outContent);
bool writeStringToFile(const std::string & fileName, const std::string & content);
bool shouldTargetFileBeUpdated(const std::string & sourceFileName, const std::string & targetFileName);


} // namespace metagen


#endif
