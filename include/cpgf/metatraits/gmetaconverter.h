#ifndef __GMETACONVERTER_H
#define __GMETACONVERTER_H

#include "cpgf/gapiutil.h"


namespace cpgf {

struct IMetaConverter : public IExtendObject
{
	virtual gapi_bool G_API_CC canToCString() = 0;
	virtual const char * G_API_CC toCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator) = 0;
};


class GMetaConverterDefault : public IMetaConverter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual gapi_bool G_API_CC canToCString() {
		return false;
	}
	virtual const char * G_API_CC toCString(const void * /*instance*/, gapi_bool * needFree, IMemoryAllocator * /*allocator*/) {
		*needFree = false;

		return NULL;
	}
};

template <typename T>
struct GMetaTraitsCreateConverter
{
	static IMetaConverter * createConverter() {
		return NULL;
	}
};

template <typename T>
inline IMetaConverter * metaTraitsCreateConverter(const T &)
{
	return GMetaTraitsCreateConverter<T>::createConverter();
}


} // namespace cpgf




#endif
