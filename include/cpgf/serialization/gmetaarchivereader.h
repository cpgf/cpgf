#ifndef __GMETAARCHIVEREADER_H
#define __GMETAARCHIVEREADER_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


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

	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param) = 0;
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
	virtual IMetaClass * G_API_CC readClassAndTypeID(uint32_t * outClassTypeID) = 0;
	virtual IMetaClass * G_API_CC readClass(uint32_t classTypeID) = 0;

	virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
	virtual void G_API_CC endReadArray(const char * name) = 0;
};


struct IMetaArchiveReader : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;

	virtual void G_API_CC readData(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;
	
	virtual void G_API_CC readMember(const char * name, void * instance, IMetaAccessible * accessible) = 0;

	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance) = 0;

	virtual void G_API_CC readObjectMembers(GMetaArchiveReaderParam * param) = 0;
	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param) = 0;
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param) = 0;
	
	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;
};


IMetaArchiveReader * createMetaArchiveReader(const GMetaArchiveConfig & config, IMetaService * service, IMetaReader * reader);

void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass);


template <typename T>
void serializeReadValue(IMetaArchiveReader * archiveReader, const char * name, T & instance) 
{
	GMetaTypeData metaTypeData = createMetaType<T>().getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer).getSerializer());
	archiveReader->readData(name, &instance, &metaTypeData, serializer.get());
}


} // namespace cpgf


#endif
