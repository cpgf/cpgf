#ifndef __GMETAARCHIVEWRITER_H
#define __GMETAARCHIVEWRITER_H

#include "cpgf/serialization/gmetaarchivecommon.h"

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


namespace serialization_internal {

template <typename T>
struct MetaArchiveWriteValueParam
{
	static const void * param(const T & object) {
		return &object;
	}
};

template <typename T>
struct MetaArchiveWriteValueParam <T *>
{
	static const void * param(T * object) {
		return static_cast<void *>(object);
	}
};

} // namespace serialization_internal


class GMetaModule;


#pragma pack(push, 1)
#pragma pack(1)

struct GMetaArchiveWriterParam
{
	const char * name;
	uint32_t archiveID;
	const void * instance;
	const GMetaTypeData * metaType;
	IMetaClass * metaClass;
	IMetaSerializer * serializer;
	uint32_t classTypeID;
	uint32_t pointers;
	GMetaArchiveConfigData config;
};

#pragma pack(pop)

class GMetaArchiveConfig;

struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
	virtual void G_API_CC writeNullPointer(const char * name) = 0;

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID) = 0;
	virtual void G_API_CC writeClassType(uint32_t classTypeID, IMetaClass * metaClass) = 0;

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) = 0;
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length) = 0;
};

struct IMetaArchiveWriter : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;
	virtual IMetaWriter * G_API_CC getMetaWriter() = 0;
	
	virtual void G_API_CC writeData(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;
	
	virtual void G_API_CC trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers) = 0;
};

// used only by serializer
struct IMetaSerializerWriter : public IExtendObject
{
	virtual void G_API_CC writeMember(GMetaArchiveWriterParam * param, IMetaAccessible * accessible) = 0;
};


IMetaArchiveWriter * createMetaArchiveWriter(IMetaService * service, IMetaWriter * writer);

void serializeWriteObjectValue(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);
void serializeWriteObjectPointer(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);

template <typename T>
void serializeWriteValue(IMetaArchiveWriter * archiveWriter, const char * name, const T & object, const GMetaModule * module)
{
	GMetaType metaType = createMetaType<T>();
	fixupMetaType(&metaType, module);
	GMetaTypeData metaTypeData = metaType.getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>(GExtendTypeCreateFlag_Serializer, module).getSerializer());
	archiveWriter->writeData(name, serialization_internal::MetaArchiveWriteValueParam<T>::param(object), &metaTypeData, serializer.get());
}

template <typename T>
void serializeWriteValue(IMetaArchiveWriter * archiveWriter, const char * name, const T & object) 
{
	serializeWriteValue(archiveWriter, name, object, NULL);
}

void metaSerializerWriteObjectMembers(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param);


} // namespace cpgf


#endif
