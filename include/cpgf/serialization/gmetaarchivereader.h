#ifndef CPGF_GMETAARCHIVEREADER_H
#define CPGF_GMETAARCHIVEREADER_H

#include "cpgf/serialization/gmetaarchivecommon.h"

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gscopedinterface.h"


namespace cpgf {


class GMetaModule;


#pragma pack(push, 1)
#pragma pack(1)

struct GMetaArchiveReaderParam
{
	const char * name;
	const GMetaTypeData * metaType;
	IMetaClass * metaClass;
	void * instance;
	IMetaSerializer * serializer;
	uint32_t archiveVersion;
	GMetaArchiveConfigData config;
};

#pragma pack(pop)

struct IMetaStorageReader : public IObject
{
	virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
	virtual uint32_t G_API_CC getClassTypeID(const char * name) = 0;

	virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue) = 0;
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
	virtual void * G_API_CC readNullPointer(const char * name) = 0;

	virtual uint32_t G_API_CC beginReadObject(const char * name, uint32_t * outVersion) = 0;
	virtual void G_API_CC endReadObject(const char * name, uint32_t version) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
	virtual IMetaClass * G_API_CC readMetaClassAndTypeID(IMetaService * service, uint32_t * outClassTypeID) = 0;
	virtual IMetaClass * G_API_CC readMetaClass(IMetaService * service, uint32_t classTypeID) = 0;

	virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
	virtual void G_API_CC endReadArray(const char * name) = 0;
};


struct IMetaArchiveReader : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;
	virtual IMetaStorageReader * G_API_CC getStorageReader() = 0;
	
	virtual void G_API_CC readData(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;

	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance) = 0;

	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;
};


// used by serializer
struct IMetaSerializerReader : public IExtendObject
{
	virtual void G_API_CC readMember(GMetaArchiveReaderParam * param, IMetaAccessible * accessible) = 0;
};

// used by serializer
void metaSerializerReadObjectMembers(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param);


IMetaArchiveReader * createMetaArchiveReader(IMetaService * service, IMetaStorageReader * reader);

void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass);

template <typename T>
void serializeReadData(IMetaArchiveReader * archiveReader, const char * name, T & instance, const GMetaModule * module) 
{
	GMetaType metaType = createMetaType<T>();
	fixupMetaType(&metaType, module);
	GMetaTypeData metaTypeData = metaType.refData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer, module).getSerializer());
	archiveReader->readData(name, &instance, &metaTypeData, serializer.get());
}

template <typename T>
void serializeReadData(IMetaArchiveReader * archiveReader, const char * name, T & instance) 
{
	serializeReadData(archiveReader, name, instance, nullptr);
}



} // namespace cpgf


#endif
