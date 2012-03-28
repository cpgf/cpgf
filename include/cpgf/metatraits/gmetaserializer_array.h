#ifndef __GMETASERIALIZER_ARRAY_H
#define __GMETASERIALIZER_ARRAY_H

#include "cpgf/metatraits/gmetaserializer.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

// forward declare the function in gmetaserializer_trapall.h
// gmetaserializer_trapall.h must be inlcuded after gmetaserializer_array.h
// otherwise, because gmetaserializer_trapall.h includes gmetaextendtype.h, it will cause array overloading doesn't work on GCC.
template <typename T>
IMetaSerializer * createTrapAllSerializer();


namespace metatraits_internal {

IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, const GMetaType & metaType, unsigned int elementSize, unsigned int elementCount);

template <typename T>
GMetaType createMetaTypeForArray(const T &)
{
	return createMetaType<T>();
}

template <typename T, int N>
GMetaType createMetaTypeForArray(const T (&)[N])
{
	return createMetaTypeForArray<T>();
}

template <typename T>
IMetaSerializer * metaTraitsCreateSerializerForArray(const T & a)
{
	IMetaSerializer * serializer = metaTraitsCreateSerializer(a);
	
	if(serializer == NULL) {
		serializer = createTrapAllSerializer<T>();
	}

	return serializer;
}

template <typename T, int N>
IMetaSerializer * metaTraitsCreateSerializerForArray(const T (&)[N])
{
	T * p = 0;
	GScopedInterface<IMetaSerializer> elementSerializer(metaTraitsCreateSerializerForArray(*p));
	return metatraits_internal::createArraySerializer(elementSerializer.get(), createMetaTypeForArray(*p), sizeof(T), N);
}

} // namespace metatraits_internal

template <typename T, int N>
IMetaSerializer * metaTraitsCreateSerializer(const T (&a)[N])
{
	return metatraits_internal::metaTraitsCreateSerializerForArray(a);
}



} // namespace cpgf


#include "cpgf/metatraits/gmetaserializer_trapall.h"


#endif
