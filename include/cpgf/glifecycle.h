#ifndef CPGF_GLIFECYCLE_H
#define CPGF_GLIFECYCLE_H

#include "cpgf/gcallback.h"


namespace cpgf {

void shutDownLibrary();
bool isLibraryLive();

struct LibraryLifeGuard
{
	~LibraryLifeGuard() {
		shutDownLibrary();
	}
};

#define G_GUARD_LIBRARY_LIFE namespace { LibraryLifeGuard _libraryGuard; }


typedef GCallback<void ()> GStaticUninitializerType;
typedef unsigned int GStaticUninitializationOrderType;

void addOrderedStaticUninitializer(GStaticUninitializationOrderType order, const GStaticUninitializerType & uninitializer);


} // namespace cpgf



#endif

