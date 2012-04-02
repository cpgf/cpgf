#ifndef __GMETAARCHIVECOMMON_H
#define __GMETAARCHIVECOMMON_H


#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"

#include "cpgf/gstringmap.h"


namespace cpgf {


const char * const SerializationAnnotation = "serialize";
const char * const SerializationAnnotationEnable = "enable";


const uint32_t archiveIDNone = 0;


enum GMetaArchiveItemType {
	matNull = 0,
	matObject = 1,
	matReferenceObject = 2,
	matClassType = 3,
	matFundamental = 4,
	matCustomized = 5,
};


struct IMetaArchiveWriter;
struct IMetaArchiveReader;

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

struct GMetaArchiveReaderParam
{
	const char * name;
	void * instance;
	IMetaClass * metaClass;
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
	virtual void G_API_CC writeClassType(const char * name, uint32_t archiveID, IMetaClass * metaClass) = 0;

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) = 0;
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length) = 0;
};

struct IMetaReader : public IObject
{
	virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
	virtual uint32_t G_API_CC getClassType(const char * name) = 0;

	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue) = 0;
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
	virtual void * G_API_CC readNullPointer(const char * name) = 0;

	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param) = 0;
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
	virtual IMetaClass * G_API_CC readClassType(const char * name, uint32_t * outArchiveID) = 0;

	virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
	virtual void G_API_CC endReadArray(const char * name) = 0;
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

struct IMetaArchiveReader : public IExtendObject
{
	virtual IMetaService * G_API_CC getMetaService() = 0;

	virtual void G_API_CC readObject(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;
	
	virtual void G_API_CC readMember(const char * name, void * instance, IMetaAccessible * accessible) = 0;

	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance) = 0;

	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param) = 0;
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param) = 0;
	
	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;
};


class GMetaArchiveConfig
{
private:
	enum ConfigFlags {
		macDisableSerializeField = 1 << 0,
		macDisableSerializeProperty = 1 << 1,
		macDisableTrackPointers = 1 << 2
	};

	enum {
		defaultConfig = 0
	};

public:
	GMetaArchiveConfig();
	explicit GMetaArchiveConfig(uint32_t flags);

	void setAllowTrackPointer(bool allow);
	bool allowTrackPointer() const;

	void setAllowSerializeField(bool allow);
	bool allowSerializeField() const;

	void setAllowSerializeProperty(bool allow);
	bool allowSerializeProperty() const;

	uint32_t getFlags() const;

private:
	GFlags<ConfigFlags> flags;
};


IMetaArchiveWriter * createMetaArchiveWriter(uint32_t config, IMetaService * service, IMetaWriter * writer);
IMetaArchiveReader * createMetaArchiveReader(uint32_t config, IMetaService * service, IMetaReader * reader);


bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item);
bool canSerializeObject(const GMetaArchiveConfig & config, IMetaClass * metaClass);
bool canSerializeField(const GMetaArchiveConfig & config, IMetaAccessible * accessible, IMetaClass * ownerClass);
bool canSerializeBaseClass(const GMetaArchiveConfig & config, IMetaClass * baseClass, IMetaClass * metaClass);


const int Error_Serialization_Begin = 301;
const int Error_Serialization_TypeMismatch = Error_Serialization_Begin + 0;
const int Error_Serialization_CannotFindObjectType = Error_Serialization_Begin + 1;
const int Error_Serialization_MissingMetaClass = Error_Serialization_Begin + 2;
const int Error_Serialization_UnknownType = Error_Serialization_Begin + 3;
const int Error_Serialization_End = 400;

void serializeError(int errorCode, ...);


void serializeWriteObjectValue(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);
void serializeWriteObjectPointer(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);

void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass);

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

template <typename T>
void serializeReadValue(IMetaArchiveReader * archiveReader, const char * name, T & instance) 
{
	GMetaTypeData metaTypeData = createMetaType<T>().getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer).getSerializer());
	archiveReader->readObject(name, &instance, &metaTypeData, serializer.get());
}

GVariant readFundamental(const void * address, const GMetaType & metaType);
void writeFundamental(void * address, const GMetaType & metaType, const GVariant & v);



} // namespace cpgf


#endif
