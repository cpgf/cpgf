#ifndef __GMETASERIALIZER_ARRAY_H
#define __GMETASERIALIZER_ARRAY_H

#include "cpgf/metatraits/gmetaserializer.h"
#include "cpgf/metatraits/gmetaserializer_trapall.h"


namespace cpgf {

namespace metatraits_internal {

IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, unsigned int elementSize, unsigned int elementCount);

template <typename T>
IMetaSerializer * metaTraitsCreateSerializerForArray(const T & a)
{
	IMetaSerializer * serializer = metaTraitsCreateSerializer(a);
	
	if(serializer == NULL) {
		serializer = createTrapAllSerializer<T>();
	}

	return serializer;
}

template <typename T, unsigned N>
IMetaSerializer * metaTraitsCreateSerializerForArray(const T (&)[N])
{
	T * p = 0;
	GScopedInterface<IMetaSerializer> elementSerializer(metaTraitsCreateSerializerForArray(*p));
	return metatraits_internal::createArraySerializer(elementSerializer.get(), sizeof(T), N);
}

} // namespace metatraits_internal

template <typename T, unsigned N>
IMetaSerializer * metaTraitsCreateSerializer(const T (&a)[N])
{
	return metatraits_internal::metaTraitsCreateSerializerForArray(a);
}



} // namespace cpgf


#endif
