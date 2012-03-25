#ifndef __GMETASERIALIZER_ARRAY_H
#define __GMETASERIALIZER_ARRAY_H

#include "cpgf/metatraits/gmetaserializer.h"
#include "cpgf/metatraits/gmetaserializer_fundamental.h"


namespace cpgf {

namespace metatraits_internal {

IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, unsigned int elementSize, unsigned int elementCount);

} // namespace metatraits_internal

template <typename T, unsigned N>
IMetaSerializer * metaTraitsCreateSerializer(const T (&)[N])
{
	T * p = 0;
	GScopedInterface<IMetaSerializer> elementSerializer(metaTraitsCreateSerializer(*p));
	return metatraits_internal::createArraySerializer(elementSerializer.get(), sizeof(T), N);
}



} // namespace cpgf


#endif
