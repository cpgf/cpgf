#ifndef __GMETAARCHIVEWRITER_H
#define __GMETAARCHIVEWRITER_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


#pragma pack(push, 1)
#pragma pack(1)

struct GMetaArchiveWriterParam
{
	const char * name;
	uint32_t archiveID;
	const void * instance;
	IMetaClass * metaClass;
	IMetaSerializer * serializer;
	uint32_t classTypeID;
	uint32_t pointers;
};

#pragma pack(pop)


struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value) = 0;
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
	virtual void G_API_CC writeNullPointer(const char * name) = 0;

	virtual void G_API_CC beginWriteObject(GMetaArchiveWriterParam * param) = 0;
	virtual void G_API_CC endWriteObject(GMetaArchiveWriterParam * param) = 0;

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) = 0;
	virtual void G_API_CC writeClassType(uint32_t archiveID, IMetaClass * metaClass) = 0;

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) = 0;
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length) = 0;
};

struct IMetaArchiveWriter : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;

	virtual void G_API_CC writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;
	
	virtual void G_API_CC writeMember(const char * name, const void * instance, IMetaAccessible * accessible) = 0;

	virtual void G_API_CC trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers) = 0;

	virtual void G_API_CC beginWriteObject(GMetaArchiveWriterParam * param) = 0;
	virtual void G_API_CC endWriteObject(GMetaArchiveWriterParam * param) = 0;
};


IMetaArchiveWriter * createMetaArchiveWriter(uint32_t config, IMetaService * service, IMetaWriter * writer);

void serializeWriteObjectValue(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);
void serializeWriteObjectPointer(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);


namespace serialization_internal {

template <typename T>
struct SerializeWriteValueParam
{
	static const void * param(const T & object) {
		return &object;
	}
};

template <typename T>
struct SerializeWriteValueParam <T *>
{
	static const void * param(T * object) {
		return static_cast<void *>(object);
	}
};

} // namespace serialization_internal

template <typename T>
void serializeWriteValue(IMetaArchiveWriter * archiveWriter, const char * name, const T & object) 
{
	GMetaTypeData metaTypeData = createMetaType<T>().getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>(GExtendTypeCreateFlag_Serializer).getSerializer());
	archiveWriter->writeObject(name, serialization_internal::SerializeWriteValueParam<T>::param(object), &metaTypeData, serializer.get());
}



} // namespace cpgf


#endif
