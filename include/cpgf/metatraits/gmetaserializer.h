#ifndef __GMETASERIALIZER_H
#define __GMETASERIALIZER_H


#include "cpgf/gapiutil.h"


namespace cpgf {

struct IMetaArchiveWriter;
struct IMetaArchiveReader;
struct IMetaClass;
struct GMetaArchiveWriterParam;
struct GMetaArchiveReaderParam;
struct IMetaSerializerWriter;
struct IMetaSerializerReader;
struct GMetaTraitsParam;


struct IMetaSerializer : public IObject
{
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) = 0;
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) = 0;
	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) = 0;
};



} // namespace cpgf


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateSerializer
{
	static cpgf::IMetaSerializer * createSerializer(const cpgf::GMetaTraitsParam &) {
		return NULL;
	}
};

inline cpgf::IMetaSerializer * metaTraitsCreateSerializer(const cpgf::GMetaTraitsParam &, ...)
{
	return NULL;
}

template <typename T>
cpgf::IMetaSerializer * createSerializerFromMetaTraits(const cpgf::GMetaTraitsParam & param, T * p)
{
	cpgf::IMetaSerializer * serializer = metaTraitsCreateSerializer(param, p);
	if(serializer == NULL) {
		serializer = GMetaTraitsCreateSerializer<T>::createSerializer(param);
	}
	return serializer;
}

} // namespace cpgf_metatraits



#endif
