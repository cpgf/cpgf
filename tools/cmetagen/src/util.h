#ifndef __UTIL_H
#define __UTIL_H


template <typename T>
void clearPointerContainer(T & container)
{
	for(typename T::iterator it = container.begin(); it != container.end(); ++it) {
		delete *it;
	}
}


#endif
