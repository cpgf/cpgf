#ifndef CPGF_GMETAARCHIVECOMMON_H
#define CPGF_GMETAARCHIVECOMMON_H

#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gexception.h"


namespace cpgf {


const char * const SerializationAnnotation = "serialize";
const char * const SerializationAnnotationEnable = "enable";
const char * const SerializationAnnotationVersion = "version";
const char * const SerializationAnnotationDefaultAll = "defaultAll";
const char * const SerializationAnnotationFields = "fields";
const char * const SerializationAnnotationProperties = "properties";
const char * const SerializationAnnotationTrackPointers = "trackPointers";


const uint32_t archiveIDNone = 0;
const uint32_t classIDNone = 0;


enum GMetaArchiveItemType {
	matMissed = 0,
	matNull = 1,
	matObject = 2,
	matReferenceObject = 3,
	matClassType = 4,
	matFundamental = 5,
	matCustomized = 6,
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

bool canSerializeMetaType(const GMetaType & metaType);

void serializeCheckType(int type, int expected);

void serializeError(int errorCode);

void writeFundamental(void * address, const GMetaType & metaType, const GVariant & v);
GVariant readFundamental(const void * address, const GMetaType & metaType);



} // namespace cpgf


#endif
