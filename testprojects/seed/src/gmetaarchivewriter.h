#ifndef __GMETAARCHIVEWRITER_H
#define __GMETAARCHIVEWRITER_H

#include "gmetaarchivecommon.h"
#include "cpgf/ginterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {


struct IMetaArchiveWriter {};

enum GMetaArchivePointerType {
	aptByValue, aptByPointer, aptIgnore
};

class GMetaArchiveWriterPointerTracker;
class GMetaArchiveWriterClassTypeTracker;

class GMetaArchiveWriter : public IMetaArchiveWriter
{
public:
	GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer);
	~GMetaArchiveWriter();

	// take care of customized serializer, take care of pointer tracking.
	void writeObjectValue(const char * name, void * instance, IMetaClass * metaClass);
	void writeObjectPointer(const char * name, void * instance, IMetaClass * metaClass);
	void writeField(const char * name, void * instance, IMetaAccessible * accessible);
	
	// ignore customized serializer, take care of pointer tracking.
	void defaultWriteObjectValue(const char * name, void * instance, IMetaClass * metaClass);
	void defaultWriteObjectPointer(const char * name, void * instance, IMetaClass * metaClass);

	// ignore customized serializer, ignore pointer tracking, take care of base classes
	void directWriteObject(const char * name, void * instance, IMetaClass * metaClass);

	// ignore customized serializer, ignore pointer tracking, ignore base classes, only write the object itself
	void directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass);

	void beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, uint32_t classTypeID);
	void endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, uint32_t classTypeID);

protected:
	void writeObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType);
	void defaultWriteObjectHelper(const char * name, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	
	void doWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType);
	void doDefaultWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void doDirectWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectWriteObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	void doWriteField(const char * name, void * instance, IMetaAccessible * accessible);
	
	uint32_t getClassTypeID(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);

	uint32_t getNextArchiveID();

	GMetaArchiveWriterPointerTracker * getPointerTracker();
	GMetaArchiveWriterClassTypeTracker * getClassTypeTracker();

private:
	GMetaArchiveConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaWriter> writer;
	uint32_t currentArchiveID;
	GScopedPointer<GMetaArchiveWriterPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveWriterClassTypeTracker> classTypeTracker;
};



} // namespace cpgf


#endif
