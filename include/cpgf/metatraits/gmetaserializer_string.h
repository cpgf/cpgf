#ifndef CPGF_GMETASERIALIZER_STRING_H
#define CPGF_GMETASERIALIZER_STRING_H

#include "cpgf/metatraits/gmetaserializer.h"

#include <string>


namespace cpgf {

namespace metatraits_internal {

	IMetaSerializer * doCreateSerializerForStdString();

} // namespace metatraits_internal

template <>
struct GMetaTraitsCreateSerializer <std::string, void>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return metatraits_internal::doCreateSerializerForStdString();
	}
};

} // namespace cpgf


#endif
