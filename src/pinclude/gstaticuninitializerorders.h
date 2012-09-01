#ifndef __GSTATICUNINITIALIZERORDERS_H
#define __GSTATICUNINITIALIZERORDERS_H

#include "cpgf/glifecycle.h"

namespace cpgf {

const GStaticUninitializationOrderType suo_ScriptDataWrapperPool = 0x500000;
const GStaticUninitializationOrderType suo_GlobalMetaClass = 0x600000;
const GStaticUninitializationOrderType suo_MetaModuleForGlobalMetaclass = 0x600001;
const GStaticUninitializationOrderType suo_MemoryPool = 0x700000;

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
