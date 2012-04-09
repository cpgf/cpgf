#ifndef __GMETAARCHIVECOMMON_H
#define __GMETAARCHIVECOMMON_H

#include "cpgf/serialization/gmetaarchivetypemap.h"

#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"

#include "cpgf/gstringmap.h"


namespace cpgf {


const char * const SerializationAnnotation = "serialize";
const char * const SerializationAnnotationEnable = "enable";
const char * const SerializationAnnotationVersion = "version";
const char * const SerializationAnnotationFields = "fields";
const char * const SerializationAnnotationProperties = "properties";
const char * const SerializationAnnotationTrackPointers = "trackPointers";


const uint32_t archiveIDNone = 0;


enum GMetaArchiveItemType {
	matNull = 0,
	matObject = 1,
	matReferenceObject = 2,
	matClassType = 3,
	matFundamental = 4,
	matCustomized = 5,
};


#pragma pack(push, 1)
#pragma pack(1)

struct GMetaArchiveConfigData
{
	uint32_t version;
	uint32_t flags;
};

#pragma pack(pop)


class GMetaArchiveConfig
{
private:
	enum ConfigFlags {
		macAllowSerialize = 1 << 0,
		macAllowSerializeField = 1 << 1,
		macAllowSerializeProperty = 1 << 2,
		macAllowTrackPointers = 1 << 3,
		macDefaultSerializeAll = 2 << 4
	};

	enum {
		defaultConfig =
			macAllowSerialize
			| macAllowSerializeField
			| macAllowSerializeProperty
			| macAllowTrackPointers
			| macDefaultSerializeAll
	};

public:
	GMetaArchiveConfig();
	explicit GMetaArchiveConfig(const GMetaArchiveConfigData & data);

	void setAllowSerialize(bool allow);
	bool allowSerialize() const;

	void setAllowTrackPointer(bool allow);
	bool allowTrackPointer() const;

	void setAllowSerializeField(bool allow);
	bool allowSerializeField() const;

	void setAllowSerializeProperty(bool allow);
	bool allowSerializeProperty() const;

	void setDefaultSerializeAll(bool defaultSerializeAll);
	bool defaultSerializeAll() const;

	void setVersion(uint32_t version);
	uint32_t getVersion() const;

	GMetaArchiveConfigData getData() const;

private:
	uint32_t version;
	GFlags<ConfigFlags> flags;
};


GMetaArchiveConfig getItemMetaArchiveConfig(IMetaItem * item);

bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item);
bool canSerializeObject(const GMetaArchiveConfig & config, IMetaClass * metaClass);
bool canSerializeField(const GMetaArchiveConfig & config, IMetaAccessible * accessible, IMetaClass * ownerClass);
bool canSerializeBaseClass(const GMetaArchiveConfig & config, IMetaClass * baseClass, IMetaClass * metaClass);
bool canSerializeMetaType(const GMetaType & metaType);

void serializeCheckType(PermanentType type, PermanentType expected);

const int Error_Serialization_Begin = 301;
const int Error_Serialization_TypeMismatch = Error_Serialization_Begin + 0;
const int Error_Serialization_CannotFindObjectType = Error_Serialization_Begin + 1;
const int Error_Serialization_MissingMetaClass = Error_Serialization_Begin + 2;
const int Error_Serialization_UnknownType = Error_Serialization_Begin + 3;
const int Error_Serialization_InvalidStorage = Error_Serialization_Begin + 4;
const int Error_Serialization_End = 400;

void serializeError(int errorCode, ...);


void writeFundamental(void * address, const GMetaType & metaType, const GVariant & v);
GVariant readFundamental(const void * address, const GMetaType & metaType);



} // namespace cpgf


#endif
