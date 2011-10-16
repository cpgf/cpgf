#ifndef __GMETACONVERTER_H
#define __GMETACONVERTER_H


#include "cpgf/gmetacommon.h"

#include <string>


namespace cpgf {

class GMetaConverterStdString : public GMetaConverterDefault
{
public:
	virtual bool canToCString();
	virtual const char * toCString(const void * instance, int * needFree, IMemoryAllocator * allocator);
};

} // namespace cpgf


template <>
struct GMetaConverterTraits <std::string>
{
	static cpgf::GMetaConverter * createConverter()
	{
		return new cpgf::GMetaConverterStdString;
	}
};

template <>
struct GMetaConverterTraits <std::string &> : public GMetaConverterTraits<std::string>
{
};

template <>
struct GMetaConverterTraits <const std::string &> : public GMetaConverterTraits<std::string>
{
};


#endif
