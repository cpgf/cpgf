#ifndef __UTIL_H
#define __UTIL_H

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


#endif
