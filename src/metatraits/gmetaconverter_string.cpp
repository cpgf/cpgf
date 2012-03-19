
#include "cpgf/metatraits/gmetaconverter.h"
#include "cpgf/gapiutil.h"

#include <string>


namespace cpgf {

class GMetaConverterStdString : public GMetaConverterDefault
{
public:
	virtual gapi_bool G_API_CC canToCString() {
		return true;
	}
	
	virtual const char * G_API_CC toCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator) {
		(void)allocator;

		*needFree = false;
		
		return static_cast<const std::string *>(instance)->c_str();
	}
};

//cpgf::IMetaConverter * metaTraitsCreateConverter(std::string &)
//{
//	return new cpgf::GMetaConverterStdString;
//}

cpgf::IMetaConverter * metaTraitsCreateConverter(const std::string &)
{
	return new cpgf::GMetaConverterStdString;
}



} // namespace cpgf

