#ifndef __GSTATICUNINITIALIZERORDERS_H
#define __GSTATICUNINITIALIZERORDERS_H

#include "cpgf/glifecycle.h"

namespace cpgf {

const GStaticUninitializationOrderType suo_ScriptDataWrapperPool = 0x5fffffff;
const GStaticUninitializationOrderType suo_MetaClass = 0x6fffffff;
const GStaticUninitializationOrderType suo_MemoryPool = 0x7fffffff;

template <typename T>
class GUninitializerDeleter
{
public:
	explicit GUninitializerDeleter(T ** p) : p(p) {}

	void operator() () {
		if(*p != NULL) {
			delete *p;
			*p = NULL;
		}
	}

private:
	T ** p;
};

template <typename T>
GUninitializerDeleter<T> makeUninitializerDeleter(T ** p)
{
	return GUninitializerDeleter<T>(p);
}


} // namespace cpgf


#endif
