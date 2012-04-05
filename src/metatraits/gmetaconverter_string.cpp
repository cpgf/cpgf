#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/gapiutil.h"

#include <string>


namespace cpgf {

class GMetaConverterStdString : public IMetaConverter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual uint32 G_API_CC capabilityForCString() {
		return metaConverterCanReadWrite;
	}
	
	virtual const char * G_API_CC readCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * /*allocator*/) {
		*needFree = false;
		
		return static_cast<const std::string *>(instance)->c_str();
	}

	virtual void G_API_CC writeCString(void * instance, const char * str) {
		*static_cast<std::string *>(instance) = str;
	}

};

namespace metatraits_internal {

IMetaConverter * createConverterForString()
{
	return new GMetaConverterStdString;
}

} // namespace metatraits_internal



} // namespace cpgf

