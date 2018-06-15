#ifndef CPGF_GMETACONVERTER_H
#define CPGF_GMETACONVERTER_H

#include "cpgf/gapiutil.h"

#include <type_traits>

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

} // namespace cpgf


namespace cpgf {

inline bool isMetaConverterCanRead(uint32_t flag)
{
	return flag == metaConverterCanRead || flag == metaConverterCanReadWrite;
}

inline bool isMetaConverterCanWrite(uint32_t flag)
{
	return flag == metaConverterCanWrite || flag == metaConverterCanReadWrite;
}


template <typename T, typename Enabled = void>
struct GMetaTraitsCreateConverter
{
	static IMetaConverter * createConverter(const GMetaTraitsParam &) {
		return nullptr;
	}
};

template <typename T>
IMetaConverter * createConverterFromMetaTraits(const GMetaTraitsParam & param, T *)
{
	return GMetaTraitsCreateConverter<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::createConverter(param);
}

} // namespace cpgf


#endif
