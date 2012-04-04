#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/gapiutil.h"

#include <string>


namespace cpgf {

class GMetaConverterStdString : public GMetaConverterDefault
{
public:
	virtual gapi_bool G_API_CC canToCString() {
		return true;
	}
	
	virtual const char * G_API_CC toCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * /*allocator*/) {
		*needFree = false;
		
		return static_cast<const std::string *>(instance)->c_str();
	}
};

namespace metatraits_internal {

IMetaConverter * createConverterForString()
{
	return new GMetaConverterStdString;
}

} // namespace metatraits_internal



} // namespace cpgf

