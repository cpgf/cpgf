#ifndef __GMETACONVERTER_WIDESTRING_H
#define __GMETACONVERTER_WIDESTRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf_metatraits {

cpgf::IMetaConverter * metaTraitsCreateConverter(const cpgf::GMetaTraitsParam &, const volatile std::wstring *);


} // namespace cpgf_metatraits



#endif
