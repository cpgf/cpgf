#ifndef __GMETACONVERTER_WIDESTRING_H
#define __GMETACONVERTER_WIDESTRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf {

IMetaConverter * metaTraitsCreateConverter(const GMetaTraitsParam &, const volatile std::wstring *);



} // namespace cpgf



#endif
