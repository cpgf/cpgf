#ifndef __GMETAARCHIVEREADER_H
#define __GMETAARCHIVEREADER_H

#include "gmetaarchivecommon.h"
#include "cpgf/ginterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {



struct IMetaArchiveReader {};

class GMetaArchiveReaderPointerTracker;
class GMetaArchiveReaderClassTypeTracker;

class GMetaArchiveReader : public IMetaArchiveReader
{
public:
	GMetaArchiveReader(const GMetaArchiveConfig & config, IMetaService * service, IMetaReader * reader);
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
	GMetaArchiveConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaReader> reader;
	GScopedPointer<GMetaArchiveReaderPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveReaderClassTypeTracker> classTypeTracker;
	GScopedInterface<IMemoryAllocator> allocator;
};



} // namespace cpgf


#endif
