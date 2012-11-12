#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H

#include "cpgf/metatraits/gmetaconverter.h"

#include <string>


namespace cpgf {

namespace metatraits_internal {

	IMetaConverter * doCreateConverterForStdString();

} // namespace metatraits_internal


template <>
struct GMetaTraitsCreateConverter <std::string, void>
{
	static IMetaConverter * createConverter(const GMetaTraitsParam &) {
		return metatraits_internal::doCreateConverterForStdString();
	}
};

inline IMetaConverter * createConverterFromMetaTraits(const GMetaTraitsParam &, std::string *)
{
	return metatraits_internal::doCreateConverterForStdString();
}


} // namespace cpgf



#endif
