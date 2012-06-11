#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/gapiutil.h"

#include "../pinclude/gmetaconverter_default.h"

#include <string>


namespace cpgf {

class GMetaConverterStdWideString : public GMetaConverterDefault
{
public:
	virtual uint32_t G_API_CC capabilityForCWideString() {
		return metaConverterCanReadWrite;
	}
	
	virtual const wchar_t * G_API_CC readCWideString(const void * instance, gapi_bool * needFree, IMemoryAllocator * /*allocator*/) {
		*needFree = false;
		
		return static_cast<const std::wstring *>(instance)->c_str();
	}

	virtual void G_API_CC writeCWideString(void * instance, const wchar_t * str) {
		*static_cast<std::wstring *>(instance) = str;
	}

};

namespace metatraits_internal {

IMetaConverter * createConverterForWideString()
{
	return new GMetaConverterStdWideString;
}

} // namespace metatraits_internal



} // namespace cpgf

