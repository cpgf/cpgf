#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf {

template <>
struct GMetaTraitsCreateConverter <std::string>
{
	static IMetaConverter * createConverter();
};




} // namespace cpgf



#endif
