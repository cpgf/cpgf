#ifndef __GMETASERIALIZER_STRING_H
#define __GMETASERIALIZER_STRING_H

#include "cpgf/metatraits/gmetaserializer.h"

#include <string>


namespace cpgf {

template <>
struct GMetaTraitsCreateSerializer <std::string>
{
	static IMetaSerializer * createSerializer();
};


} // namespace cpgf


#endif
