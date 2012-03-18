#ifndef __GMETAARCHIVECOMMON_H
#define __GMETAARCHIVECOMMON_H


#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"


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
	void * instance;
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


struct IMetaArchiveReader
{
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

	void setAllowTrackPointer(bool allow) {
		this->flags.setByBool(macTrackPointers, allow);
	}

	bool allowTrackPointer() const {
		return this->flags.has(macTrackPointers);
	}

	bool allowSerializeField() const { return true; }
	bool allowSerializeProperty() const { return true; }

private:
	GFlags<ConfigFlags> flags;
};


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



} // namespace cpgf


#endif
