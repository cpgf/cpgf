#ifndef __GMETAARCHIVEREADER_H
#define __GMETAARCHIVEREADER_H

#include "cpgf/serialization/gmetaarchivecommon.h"

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"


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
	void * originalInstance;
	IMetaClass * originalMetaClass;
	IMetaSerializer * serializer;
	GMetaArchiveConfigData config;
	uint32_t archiveVersion;
};

#pragma pack(pop)

class GMetaArchiveConfig;

struct IMetaReader : public IObject
{
	virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
	virtual uint32_t G_API_CC getClassType(const char * name) = 0;

	virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue) = 0;
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
	virtual void * G_API_CC readNullPointer(const char * name) = 0;

	virtual uint32_t G_API_CC beginReadObject(const char * name, uint32_t * outVersion) = 0;
	virtual void G_API_CC endReadObject(const char * name, uint32_t version) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
	virtual IMetaClass * G_API_CC readClassAndTypeID(uint32_t * outClassTypeID) = 0;
	virtual IMetaClass * G_API_CC readClass(uint32_t classTypeID) = 0;

	virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
	virtual void G_API_CC endReadArray(const char * name) = 0;
};


struct IMetaArchiveReader : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;
	virtual IMetaReader * G_API_CC getMetaReader() = 0;
	
	virtual void G_API_CC readData(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;

	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance) = 0;

	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;
};


// used only by serializer
struct IMetaSerializerReader : public IExtendObject
{
	virtual void G_API_CC readMember(GMetaArchiveReaderParam * param, IMetaAccessible * accessible) = 0;
};


IMetaArchiveReader * createMetaArchiveReader(IMetaService * service, IMetaReader * reader);

void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass);

template <typename T>
void serializeReadValue(IMetaArchiveReader * archiveReader, const char * name, T & instance, const GMetaModule * module) 
{
	GMetaType metaType = createMetaType<T>();
	fixupMetaType(&metaType, module);
	GMetaTypeData metaTypeData = metaType.getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer, module).getSerializer());
	archiveReader->readData(name, &instance, &metaTypeData, serializer.get());
}

template <typename T>
void serializeReadValue(IMetaArchiveReader * archiveReader, const char * name, T & instance) 
{
	serializeReadValue(archiveReader, name, instance, NULL);
}

void metaSerializerReadObjectMembers(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param);



} // namespace cpgf


#endif
