#ifndef __GMETASERIALIZER_ARRAY_H
#define __GMETASERIALIZER_ARRAY_H

#include "cpgf/metatraits/gmetaserializer.h"
#include "cpgf/metatraits/gmetaserializer_trapall.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


namespace metatraits_internal {

IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, const GMetaType & metaType, unsigned int elementSize, unsigned int elementCount);

template <typename T>
IMetaSerializer * metaTraitsCreateSerializerForArray(const GMetaTraitsParam & param, const T & a)
{
	IMetaSerializer * serializer = createSerializerFromMetaTraits(param, &a);
	
	if(serializer == NULL) {
		serializer = createTrapAllSerializer<T>(param);
	}

	return serializer;
}

template <typename T, int N>
IMetaSerializer * metaTraitsCreateSerializerForArray(const GMetaModule * module, const T (&)[N])
{
	T * p = 0;
	GScopedInterface<IMetaSerializer> elementSerializer(metaTraitsCreateSerializerForArray(module, *p));
	return createArraySerializer(elementSerializer.get(), createMetaType<T>(), sizeof(T), N);
}

} // namespace metatraits_internal

template <typename T, int N>
struct GMetaTraitsCreateSerializer <T[N]>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam & param) {
		T * p = 0;
		GScopedInterface<IMetaSerializer> elementSerializer(metatraits_internal::metaTraitsCreateSerializerForArray(param, *p));
		return metatraits_internal::createArraySerializer(elementSerializer.get(), createMetaType<T>(), sizeof(T), N);
	}
};




} // namespace cpgf



#endif
