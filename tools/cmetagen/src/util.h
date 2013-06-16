#ifndef __UTIL_H
#define __UTIL_H

#include "cpgf/gflags.h"

#include <string>


template <typename T>
void clearPointerContainer(T & container)
{
	for(typename T::iterator it = container.begin(); it != container.end(); ++it) {
		delete *it;
	}
}

// Replace all '\' with '/'
std::string normalizePath(const std::string & path);

std::string removeLastToken(const std::string & s);
std::string removeAllAfterEqualSign(const std::string & s);

std::string normalizeSymbolName(const std::string & name);
std::string removeQualifications(const std::string & name);

class CppParamList;

enum ParamListToStringOption {
	poWithType = 1 << 0,
	poWithName = 1 << 1,
	poWithDefaultValue = 1 << 2
};

std::string paramListToString(const CppParamList * paramList, const cpgf::GFlags<ParamListToStringOption> options);

#endif
