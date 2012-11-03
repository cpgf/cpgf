#ifndef __GMETACONVERTER_H
#define __GMETACONVERTER_H

#include "cpgf/gapiutil.h"


namespace cpgf {

struct GMetaTraitsParam;

const uint32_t metaConverterCanNothing = 0;
const uint32_t metaConverterCanRead = 1;
const uint32_t metaConverterCanWrite = 2;
const uint32_t metaConverterCanReadWrite = 3;

struct IMetaConverter : public IExtendObject
{
	virtual uint32_t G_API_CC capabilityForCString() = 0;
	virtual const char * G_API_CC readCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator) = 0;
	virtual void G_API_CC writeCString(void * instance, const char * str) = 0;

	virtual uint32_t G_API_CC capabilityForCWideString() = 0;
	virtual const wchar_t * G_API_CC readCWideString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator) = 0;
	virtual void G_API_CC writeCWideString(void * instance, const wchar_t * str) = 0;
};


inline bool isMetaConverterCanRead(uint32_t flag)
{
	return flag == metaConverterCanRead || flag == metaConverterCanReadWrite;
}

inline bool isMetaConverterCanWrite(uint32_t flag)
{
	return flag == metaConverterCanWrite || flag == metaConverterCanReadWrite;
}


} // namespace cpgf


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateConverter
{
	static cpgf::IMetaConverter * createConverter(const cpgf::GMetaTraitsParam &) {
		return NULL;
	}
};

inline cpgf::IMetaConverter * metaTraitsCreateConverter(const cpgf::GMetaTraitsParam &, ...)
{
	return NULL;
}

template <typename T>
cpgf::IMetaConverter * createConverterFromMetaTraits(const cpgf::GMetaTraitsParam & param, T * p)
{
	cpgf::IMetaConverter * converter = metaTraitsCreateConverter(param, p);
	if(converter == NULL) {
		converter = GMetaTraitsCreateConverter<T>::createConverter(param);
	}
	return converter;
}

} // namespace cpgf_metatraits


#endif
