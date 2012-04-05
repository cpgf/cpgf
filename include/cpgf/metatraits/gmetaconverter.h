#ifndef __GMETACONVERTER_H
#define __GMETACONVERTER_H

#include "cpgf/gapiutil.h"


namespace cpgf {

const uint32_t metaConverterCanNothing = 0;
const uint32_t metaConverterCanRead = 1;
const uint32_t metaConverterCanWrite = 2;
const uint32_t metaConverterCanReadWrite = 3;

struct IMetaConverter : public IExtendObject
{
	virtual uint32_t G_API_CC capabilityForCString() = 0;
	virtual const char * G_API_CC readCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator) = 0;
	virtual void G_API_CC writeCString(void * instance, const char * str) = 0;
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

inline bool isMetaConverterCanRead(uint32_t flag)
{
	return flag == metaConverterCanRead || flag == metaConverterCanReadWrite;
}

inline bool isMetaConverterCanWrite(uint32_t flag)
{
	return flag == metaConverterCanWrite || flag == metaConverterCanReadWrite;
}


} // namespace cpgf




#endif
