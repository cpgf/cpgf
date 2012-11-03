#ifndef __GMETASERIALIZER_STRING_H
#define __GMETASERIALIZER_STRING_H

#include "cpgf/metatraits/gmetaserializer.h"

#include <string>


namespace cpgf_metatraits {

cpgf::IMetaSerializer * metaTraitsCreateSerializer(const cpgf::GMetaTraitsParam &, const volatile std::string *);


} // namespace cpgf_metatraits


#endif
