#ifndef __GMETAARCHIVECOMMON_H
#define __GMETAARCHIVECOMMON_H


#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"


namespace cpgf {


const char * const SerializationAnnotation = "serialize";
const char * const SerializationAnnotationEnable = "enable";


const uint32_t archiveIDNone = 0;


struct IMetaArchiveWriter;
struct IMetaArchiveReader;

struct IMetaObjectSerializer : public IObject
{
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
};


struct IMetaFieldSerializer : public IObject
{
	virtual void G_API_CC writeField(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaAccessible * accessible) = 0;
	virtual void G_API_CC readField(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaAccessible * accessible) = 0;
};


struct GMetaArchiveObjectInformation
{
	const char * name;
	uint32_t archiveID;
	uint32_t classTypeID;
	void * instance;
	IMetaClass * metaClass;
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
const int Error_Serialization_End = 400;

inline void serializeError(int errorCode, ...)
{
	raiseException(errorCode, "Serialize error.");
}



} // namespace cpgf


#endif
