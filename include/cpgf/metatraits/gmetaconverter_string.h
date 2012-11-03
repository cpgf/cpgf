#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf_metatraits {

cpgf::IMetaConverter * metaTraitsCreateConverter(const cpgf::GMetaTraitsParam &, const volatile std::string *);


} // namespace cpgf_metatraits



#endif
