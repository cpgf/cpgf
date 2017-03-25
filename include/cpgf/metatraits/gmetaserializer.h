#ifndef CPGF_GMETASERIALIZER_H
#define CPGF_GMETASERIALIZER_H

#include "cpgf/gapiutil.h"

#include <type_traits>

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
	virtual void G_API_CC freeObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass, void * instance) = 0;
	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) = 0;
};

} // namespace cpgf


namespace cpgf {

template <typename T, typename Enabled = void>
struct GMetaTraitsCreateSerializer
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return nullptr;
	}
};

template <typename T>
IMetaSerializer * createSerializerFromMetaTraits(const GMetaTraitsParam & param, T *)
{
	return GMetaTraitsCreateSerializer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::createSerializer(param);
}

} // namespace cpgf


#endif
