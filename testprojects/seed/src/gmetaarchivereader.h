#ifndef __GMETAARCHIVEREADER_H
#define __GMETAARCHIVEREADER_H

#include "gmetaarchivecommon.h"
#include "cpgf/ginterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {



class GMetaArchiveReaderConfig : public GMetaArchiveConfig
{
private:
	enum ConfigFlags {
		macResolvePointers = 1 << 0
	};

	enum {
		defaultConfig = macResolvePointers
	};

public:
	GMetaArchiveReaderConfig() : flags(defaultConfig) {
	}

	void setAllowReslovePointers(bool allow) {
		this->flags.setByBool(macResolvePointers, allow);
	}

	bool allowReslovePointers() const {
		return this->flags.has(macResolvePointers);
	}

private:
	GFlags<ConfigFlags> flags;
};


enum GMetaArchiveItemType {
	matNull = 0,
	matObject = 1,
	matReferenceObject = 2,
	matClassType = 3,
	matFundamental = 4,
	matString = 5,
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


struct IMetaArchiveReader {};

class GMetaArchiveReaderPointerTracker;
class GMetaArchiveReaderClassTypeTracker;

class GMetaArchiveReader : public IMetaArchiveReader
{
public:
	GMetaArchiveReader(const GMetaArchiveReaderConfig & config, IMetaService * service, IMetaReader * reader);
	~GMetaArchiveReader();

	// take care of customized serializer, take care of pointer resolve.
	void readObject(const char * name, void * instance, IMetaClass * metaClass);
	void readField(const char * name, void * instance, IMetaAccessible * accessible);
	
	// ignore customized serializer, take care of pointer resolve.
	void defaultReaderObject(const char * name, void * instance, IMetaClass * metaClass);
	void defaultReaderField(const char * name, void * instance, IMetaAccessible * accessible);

	// ignore customized serializer, ignore pointer resolve, take care of base classes
	void directReadObject(const char * name, void * instance, IMetaClass * metaClass);
	void directReadField(const char * name, void * instance, IMetaAccessible * accessible);

	// ignore customized serializer, ignore pointer resolve, ignore base classes, only write the object itself
	void directReadObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass);

	uint32_t beginReadObject(const char * name, void * instance, IMetaClass * metaClass);
	void endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
protected:
	void doReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDefaultReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectReadObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	void doReadField(const char * name, void * instance, IMetaAccessible * accessible);
	void doDefaultReadField(const char * name, void * instance, IMetaAccessible * accessible);
	void doDirectReadField(const char * name, void * instance, IMetaAccessible * accessible);

	GMetaArchiveReaderPointerTracker * getPointerTracker();
	GMetaArchiveReaderClassTypeTracker * getClassTypeTracker();

private:
	GMetaArchiveReaderConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaReader> reader;
	GScopedPointer<GMetaArchiveReaderPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveReaderClassTypeTracker> classTypeTracker;
	GScopedInterface<IMemoryAllocator> allocator;
};



} // namespace cpgf


#endif
