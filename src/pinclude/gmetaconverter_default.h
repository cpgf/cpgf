#ifndef CPGF_GMETACONVERTER_DEFAULT_H
#define CPGF_GMETACONVERTER_DEFAULT_H


#include "cpgf/metatraits/gmetaconverter.h"


namespace cpgf {

class GMetaConverterDefault : public IMetaConverter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual ~GMetaConverterDefault() {}

	virtual uint32_t G_API_CC capabilityForCString() {
		return metaConverterCanNothing;
	}
	
	virtual const char * G_API_CC readCString(const void * /*instance*/, gapi_bool * /*needFree*/, IMemoryAllocator * /*allocator*/) {
		return nullptr;
	}
	
	virtual void G_API_CC writeCString(void * /*instance*/, const char * /*str*/) {
	}

	virtual uint32_t G_API_CC capabilityForCWideString() {
		return metaConverterCanNothing;
	}
	
	virtual const wchar_t * G_API_CC readCWideString(const void * /*instance*/, gapi_bool * /*needFree*/, IMemoryAllocator * /*allocator*/) {
		return nullptr;
	}
	
	virtual void G_API_CC writeCWideString(void * /*instance*/, const wchar_t * /*str*/) {
	}
};


} // namespace cpgf



#endif
