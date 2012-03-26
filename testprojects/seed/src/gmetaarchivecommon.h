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
	matString = 5,
};

struct IMetaArchiveWriter;
struct IMetaArchiveReader;

struct GMetaArchiveObjectInformation
{
	const char * name;
	uint32_t archiveID;
	uint32_t classTypeID;
	const void * instance;
	IMetaClass * metaClass;
};

struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value) = 0;
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
	virtual void G_API_CC writeNullPointer(const char * name) = 0;

	virtual void G_API_CC beginWriteObject(const GMetaArchiveObjectInformation * objectInformation) = 0;
	virtual void G_API_CC endWriteObject(const GMetaArchiveObjectInformation * objectInformation) = 0;

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) = 0;
	virtual void G_API_CC writeClassType(const char * name, uint32_t archiveID, IMetaClass * metaClass) = 0;

//	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
//	virtual void G_API_CC endWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
};

struct IMetaReader : public IObject
{
	virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
	virtual uint32_t G_API_CC getClassType(const char * name) = 0;

	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue) = 0;
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
	virtual void * G_API_CC readNullPointer(const char * name) = 0;

	virtual void G_API_CC beginReadObject(GMetaArchiveObjectInformation * objectInformation) = 0;
	virtual void G_API_CC endReadObject(const GMetaArchiveObjectInformation * objectInformation) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
	virtual IMetaClass * G_API_CC readClassType(const char * name, uint32_t * outArchiveID) = 0;

};


struct IMetaArchiveWriter : public IExtendObject
{
	// take care of customized serializer, take care of pointer tracking.
	virtual void G_API_CC writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;

	// ignore customized serializer, take care of pointer tracking.
	virtual void G_API_CC defaultWriteObjectValue(const char * name, const void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC defaultWriteObjectPointer(const char * name, const void * instance, IMetaClass * metaClass) = 0;

	// ignore customized serializer, ignore pointer tracking, take care of base classes
	virtual void G_API_CC directWriteObject(const char * name, const void * instance, IMetaClass * metaClass) = 0;

	// ignore customized serializer, ignore pointer tracking, ignore base classes, only write the object itself
	virtual void G_API_CC directWriteObjectWithoutBase(const char * name, const void * instance, IMetaClass * metaClass) = 0;

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID) = 0;
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID) = 0;
};

struct IMetaArchiveReader : public IExtendObject
{
	// take care of customized serializer, take care of pointer tracking.
	virtual void G_API_CC readObject(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer) = 0;
	
	// ignore customized serializer, take care of pointer tracking.
	virtual void G_API_CC defaultReaderObject(const char * name, void * instance, IMetaClass * metaClass) = 0;

	// ignore customized serializer, ignore pointer tracking, take care of base classes
	virtual void G_API_CC directReadObject(const char * name, void * instance, IMetaClass * metaClass) = 0;

	// ignore customized serializer, ignore pointer tracking, ignore base classes, only write the object itself
	virtual void G_API_CC directReadObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass) = 0;

	virtual uint32_t G_API_CC beginReadObject(const char * name, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
	
	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;
};


class GMetaArchiveConfig
{
private:
	enum ConfigFlags {
		macSerializeField = 1 << 0,
		macSerializeProperty = 1 << 1,
		macTrackPointers = 1 << 2
	};

	enum {
		defaultConfig = macSerializeField | macSerializeProperty
			| macTrackPointers
	};

public:
	GMetaArchiveConfig() : flags(defaultConfig) {
	}

	explicit GMetaArchiveConfig(uint32_t flags) : flags(flags) {
	}

	void setAllowTrackPointer(bool allow) {
		this->flags.setByBool(macTrackPointers, allow);
	}

	bool allowTrackPointer() const {
		return this->flags.has(macTrackPointers);
	}

	bool allowSerializeField() const { return true; }
	bool allowSerializeProperty() const { return true; }

	uint32_t getFlags() const {
		return this->flags;
	}

private:
	GFlags<ConfigFlags> flags;
};


class GBaseClassMap
{
private:
	struct MapItem {
		void * instance;
		IMetaClass * metaClass;
	};

	typedef GStringMap<MapItem, GStringMapReuseKey> MapType;

public:
	~GBaseClassMap() {
		for(MapType::iterator it = this->itemMap.begin(); it != this->itemMap.end(); ++it) {
			it->second.metaClass->releaseReference();
		}
	}

	bool hasMetaClass(void * instance, IMetaClass * metaClass) const {
		MapType::const_iterator it = this->itemMap.find(metaClass->getTypeName());
		return it != this->itemMap.end() && it->second.instance == instance;
	}

	void addMetaClass(void * instance, IMetaClass * metaClass) {
		MapItem item;
		item.instance = instance;
		item.metaClass = metaClass;
		this->itemMap.set(metaClass->getTypeName(), item);
		metaClass->addReference();
	}

private:
	MapType itemMap;
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
const int Error_Serialization_End = 400;

inline void serializeError(int errorCode, ...)
{
	raiseException(errorCode, "Serialize error.");
}

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
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer).getSerializer());
	archiveWriter->writeObject(name, serialization_internal::SerializeWriteValueParam<T>::param(object), &metaTypeData, serializer.get());
}

template <typename T>
void serializeReadValue(IMetaArchiveReader * archiveReader, const char * name, T * instance) 
{
	GMetaTypeData metaTypeData = createMetaType<T>().getData();
	GScopedInterface<IMetaSerializer> serializer(createMetaExtendType<T>( GExtendTypeCreateFlag_Serializer).getSerializer());
	archiveReader->readObject(name, instance, &metaTypeData, serializer.get());
}

GVariant readFundamental(const void * address, const GMetaType & metaType);
void writeFundamental(void * address, const GMetaType & metaType, const GVariant & v);



} // namespace cpgf


#endif
