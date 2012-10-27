#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf {

IMetaConverter * metaTraitsCreateConverter(const GMetaTraitsParam &, const volatile std::string *);


} // namespace cpgf



#endif
