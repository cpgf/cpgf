#ifndef CPGF_GMETASERIALIZER_TRAPALL_H
#define CPGF_GMETASERIALIZER_TRAPALL_H

#include "cpgf/gmetatype.h"
#include "cpgf/gmetaextendtype.h"
#include "cpgf/gscopedinterface.h"


namespace cpgf {

struct IMetaSerializer;
class GMetaType;

namespace metatraits_internal {

cpgf::IMetaSerializer * doCreateTrapAllSerializer(const cpgf::GMetaType & metaType, cpgf::IMetaSerializer * serializer);

} // namespace metatraits_internal

template <typename T>
cpgf::IMetaSerializer * createTrapAllSerializer(const cpgf::GMetaTraitsParam & param)
{
	cpgf::GScopedInterface<cpgf::IMetaSerializer> serializer(cpgf::createMetaExtendType<T>(cpgf::GExtendTypeCreateFlag_Serializer).getSerializer());
	cpgf::GMetaType metaType(cpgf::createMetaType<T>());
	cpgf::fixupMetaType(&metaType, param.module);
	return cpgf::metatraits_internal::doCreateTrapAllSerializer(metaType, serializer.get());
}



} // namespace cpgf


#endif
