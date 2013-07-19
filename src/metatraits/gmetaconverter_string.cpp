#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/gapiutil.h"

#include "../pinclude/gmetaconverter_default.h"

#include <string>


namespace cpgf {

namespace {

class GMetaConverterStdString : public GMetaConverterDefault
{
public:
	virtual ~GMetaConverterStdString() {}

	virtual uint32_t G_API_CC capabilityForCString() {
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

} // unnamed namespace

namespace metatraits_internal {

	IMetaConverter * doCreateConverterForStdString()
	{
		return new GMetaConverterStdString();
	}

} // namespace metatraits_internal


} // namespace cpgf


