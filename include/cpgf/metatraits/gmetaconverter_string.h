#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf {

namespace metatraits_internal {

IMetaConverter * createConverterForString();

} // namespace metatraits_internal

template <>
struct GMetaTraitsCreateConverter <std::string>
{
	static IMetaConverter * createConverter() {
		return metatraits_internal::createConverterForString();
	}
};




} // namespace cpgf



#endif
