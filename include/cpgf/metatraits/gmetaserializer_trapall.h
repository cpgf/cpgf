#ifndef __GMETASERIALIZER_TRAPALL_H
#define __GMETASERIALIZER_TRAPALL_H

#include "cpgf/gmetatype.h"
#include "cpgf/gmetaextendtype.h"


namespace cpgf {

struct IMetaSerializer;
class GMetaType;

namespace metatraits_internal {

IMetaSerializer * doCreateTrapAllSerializer(const GMetaType & metaType, IMetaSerializer * serializer);

} // namespace metatraits_internal

template <typename T>
IMetaSerializer * createTrapAllSerializer()
{
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>(GExtendTypeCreateFlag_Serializer).getSerializer());
	return metatraits_internal::doCreateTrapAllSerializer(createMetaType<T>(), serializer.get());
}



} // namespace cpgf


#endif
