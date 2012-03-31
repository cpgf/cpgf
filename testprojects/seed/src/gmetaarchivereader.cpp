#include "gmetaarchivecommon.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gapiutil.h"

#include <map>

using namespace std;

namespace cpgf {


class GMetaArchiveReaderPointerTracker;
class GMetaArchiveReaderClassTypeTracker;

class GMetaArchiveReader : public IMetaArchiveReader
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaArchiveReader(const GMetaArchiveConfig & config, IMetaService * service, IMetaReader * reader);
	~GMetaArchiveReader();

	virtual IMetaService * G_API_CC getMetaService();

	// take care of customized serializer, take care of pointer tracking.
	virtual void G_API_CC readObject(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer);
	
	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance);

	virtual uint32_t G_API_CC beginReadObject(const char * name, void * instance, IMetaClass * metaClass);
	virtual void G_API_CC endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	virtual IMemoryAllocator * G_API_CC getAllocator();
	
protected:
	void * readObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer);
	
	void * doReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GBaseClassMap * baseClassMap);
	void doDirectReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, GBaseClassMap * baseClassMap);
	void doDirectReadObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	void doReadField(const char * name, void * instance, IMetaAccessible * accessible);
	void doReadProperty(const char * name, void * instance, IMetaAccessible * accessible);
	
	void doReadValue(const char * name, void * address, const GMetaType & metaType, IMetaSerializer * serializer);
	bool checkUntrackPointer(const char * name, void * address);

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


class GMetaArchiveReaderPointerTracker
{
private:
	typedef std::map<uint32_t, void *> MapType;

public:
	bool hasArchiveID(uint32_t archiveID) const;
	void * getPointer(uint32_t archiveID) const;
	void addArchiveID(uint32_t archiveID, void * p);

private:
	MapType pointerMap;
};

class GMetaArchiveReaderClassTypeTracker
{
private:
	typedef GSharedInterface<IMetaClass> MetaType;
	typedef std::map<uint32_t, MetaType> MapType;

public:
	bool hasArchiveID(uint32_t archiveID) const;
	IMetaClass * getMetaClass(uint32_t archiveID) const;
	void addArchiveID(uint32_t archiveID, IMetaClass * metaClass);

private:
	MapType classTypeMap;
};


bool GMetaArchiveReaderPointerTracker::hasArchiveID(uint32_t archiveID) const
{
	return this->pointerMap.find(archiveID) != this->pointerMap.end();
}

void * GMetaArchiveReaderPointerTracker::getPointer(uint32_t archiveID) const
{
	MapType::const_iterator it = this->pointerMap.find(archiveID);

	if(it == this->pointerMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

void GMetaArchiveReaderPointerTracker::addArchiveID(uint32_t archiveID, void * p)
{
	GASSERT(! this->hasArchiveID(archiveID));

	this->pointerMap.insert(pair<uint32_t, void *>(archiveID, p));
}


bool GMetaArchiveReaderClassTypeTracker::hasArchiveID(uint32_t archiveID) const
{
	return this->classTypeMap.find(archiveID) != this->classTypeMap.end();
}

IMetaClass * GMetaArchiveReaderClassTypeTracker::getMetaClass(uint32_t archiveID) const
{
	MapType::const_iterator it = this->classTypeMap.find(archiveID);

	if(it == this->classTypeMap.end()) {
		return NULL;
	}
	else {
		IMetaClass * metaClass = it->second.get();
		metaClass->addReference();
		return metaClass;
	}
}

void GMetaArchiveReaderClassTypeTracker::addArchiveID(uint32_t archiveID, IMetaClass * metaClass)
{
	GASSERT(! this->hasArchiveID(archiveID));

	this->classTypeMap.insert(pair<uint32_t, MetaType>(archiveID, MetaType(metaClass)));
}


GMetaArchiveReader::GMetaArchiveReader(const GMetaArchiveConfig & config, IMetaService * service, IMetaReader * reader)
	: config(config), service(service), reader(reader)
{
	if(this->service) {
		this->service->addReference();
	}

	this->reader->addReference();
}

GMetaArchiveReader::~GMetaArchiveReader()
{
}

IMetaService * G_API_CC GMetaArchiveReader::getMetaService()
{
	this->service->addReference();
	return this->service.get();
}

void G_API_CC GMetaArchiveReader::readObject(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer)
{
	GMetaType type(*metaType);

	if(this->checkUntrackPointer(name, instance)) {
		return;
	}

	this->doReadValue(name, instance, type, serializer);
}

void * GMetaArchiveReader::readObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer)
{
	if(this->checkUntrackPointer(name, instance)) {
		return instance;
	}

	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	GBaseClassMap baseClassMap;
	void * p = this->doReadObject(name, archiveID, instance, metaClass, serializer, &baseClassMap);

	this->endReadObject(name, archiveID, instance, metaClass);

	return p;
}

uint32_t G_API_CC GMetaArchiveReader::beginReadObject(const char * name, void * instance, IMetaClass * metaClass)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveIDNone;
	objectInformation.classTypeID = archiveIDNone;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->reader->beginReadObject(&objectInformation);

	return objectInformation.archiveID;
}

void GMetaArchiveReader::endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveID;
	objectInformation.classTypeID = archiveIDNone;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->reader->endReadObject(&objectInformation);
}

void * GMetaArchiveReader::doReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GBaseClassMap * baseClassMap)
{
	GScopedInterface<IMetaSerializer> serializerPointer;
	if(serializer == NULL && metaClass != NULL) {
		serializerPointer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
		serializer = serializerPointer.get();
	}

	if(serializer != NULL) {
		if(instance == NULL) {
			instance = serializer->allocateObject(this, metaClass);
		}
		serializer->readObject(name, this, this->reader.get(), archiveID, instance, metaClass);
	}
	else {
		this->trackPointer(archiveID, instance);
		this->doDirectReadObject(name, archiveID, instance, metaClass, baseClassMap);
	}
	return instance;
}

void GMetaArchiveReader::doDirectReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, GBaseClassMap * baseClassMap)
{
	GScopedInterface<IMetaClass> baseClass;
	uint32_t i;
	uint32_t count;

	count = metaClass->getBaseCount();
	for(i = 0; i < count; ++i) {
		baseClass.reset(metaClass->getBaseClass(i));
		
		if(canSerializeBaseClass(this->config, baseClass.get(), metaClass)) {
			void * baseInstance = metaClass->castToBase(instance, i);
			if(! baseClassMap->hasMetaClass(baseInstance, baseClass.get())) {
				baseClassMap->addMetaClass(baseInstance, baseClass.get());
				this->doReadObject(name, archiveIDNone, baseInstance, baseClass.get(), NULL, baseClassMap);
			}
		}
	}

	this->doDirectReadObjectWithoutBase(archiveID, instance, metaClass);
}

void GMetaArchiveReader::doDirectReadObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	(void)archiveID;

	if(instance == NULL) {
		return;
	}

	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	if(this->config.allowSerializeField()) {
		count = metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(metaClass->getFieldAt(i));

			if(canSerializeField(this->config, accessible.get(), metaClass)) {
				this->doReadField(accessible->getName(), instance, accessible.get());
			}
		}
	}

	if(this->config.allowSerializeProperty()) {
		count = metaClass->getPropertyCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(metaClass->getPropertyAt(i));
			if(canSerializeField(this->config, accessible.get(), metaClass)) {
				this->doReadProperty(accessible->getName(), instance, accessible.get());
			}
		}
	}
}

void GMetaArchiveReader::doReadValue(const char * name, void * address, const GMetaType & metaType, IMetaSerializer * serializer)
{
	size_t pointers = metaType.getPointerDimension();
	GVariantData data;
	uint32_t archiveID;

	GMetaArchiveItemType type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));

	if(type == matClassType) {
		uint32_t classTypeID;
		GScopedInterface<IMetaClass> metaClass(this->reader->readClassType(name, &classTypeID));
		type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));
		if(metaClass && classTypeID != archiveIDNone && ! this->getClassTypeTracker()->hasArchiveID(classTypeID)) {
			this->getClassTypeTracker()->addArchiveID(classTypeID, metaClass.get());
		}
	}

	if(metaType.isArray()) {
		this->readObjectHelper(name, address, NULL, serializer);
		return;
	}

	switch(type) {
		case matNull: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			this->reader->readNullPointer(name);
			*(void **)address = NULL;
			break;
		}

		case matFundamental: {
			if(metaType.isFundamental()) {
				this->reader->readFundamental(name, &archiveID, &data);
				writeFundamental(address, metaType, GVariant(data));
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			break;
		}

		case matObject: {
			if(pointers > 1) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			GScopedInterface<IMetaClass> metaClass;
			if(this->service) {
				metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
			}
			if(pointers == 0) {
				this->readObjectHelper(name, address, metaClass.get(), serializer);
			}
			else {
				void * ptr = *(void **)address;
				if(metaClass) {
					if(ptr == NULL) {
						uint32_t classTypeID = this->reader->getClassType(name);
						if(classTypeID != archiveIDNone) {
							IMetaClass * storedMetaClass = this->getClassTypeTracker()->getMetaClass(classTypeID);
							if(storedMetaClass == NULL) {
								serializeError(Error_Serialization_CannotFindObjectType);
							}
							else {
								metaClass.reset(storedMetaClass);
							}
						}
						ptr = metaClass->createInstance();
					}
					else {
						void * castedPtr;
						GScopedInterface<IMetaClass> castedMetaClass(findAppropriateDerivedClass(ptr, metaClass.get(), &castedPtr));
						ptr = castedPtr;
						metaClass.reset(castedMetaClass.take());
					}
					this->readObjectHelper(name, ptr, metaClass.get(), NULL);
				}
				else {
					ptr = this->readObjectHelper(name, ptr, NULL, serializer);
				}
				*(void **)address = ptr;
			}
			break;
		}

		case matReferenceObject: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			archiveID = this->reader->readReferenceID(name);
			if(this->getPointerTracker()->hasArchiveID(archiveID)) {
				*(void **)address = this->getPointerTracker()->getPointer(archiveID);
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			break;
		}

		default:
			GASSERT(false);

			break;
	}
}

bool GMetaArchiveReader::checkUntrackPointer(const char * name, void * address)
{
	GMetaArchiveItemType type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));

	if(type == matReferenceObject) {
		uint32_t archiveID = this->reader->readReferenceID(name);
		if(this->getPointerTracker()->hasArchiveID(archiveID)) {
			*(void **)address = this->getPointerTracker()->getPointer(archiveID);

			return true;
		}
		else {
			serializeError(Error_Serialization_TypeMismatch);
		}
	}

	return false;
}

void G_API_CC GMetaArchiveReader::trackPointer(uint32_t archiveID, void * instance)
{
	if(archiveID != archiveIDNone) {
		if(! this->getPointerTracker()->hasArchiveID(archiveID)) {
			this->getPointerTracker()->addArchiveID(archiveID, instance);
		}
	}
	
}

void GMetaArchiveReader::doReadField(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers <= 1) {
		GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());

		char buffer[64];
		void * ptr;

		if(metaType.isArray()) {
			ptr = accessible->getAddress(instance);
		}
		else {
			ptr = accessible->getAddress(instance);
			if(ptr == NULL) { // this happens when accessible is a property with both getter and setter.
				ptr = buffer;
			}
		}

		GASSERT(ptr != NULL || serializer);

		this->doReadValue(name, ptr, metaType, serializer.get());
		if(ptr == buffer) {
			metaSetValue(accessible, instance, GVariant(ptr));
		}
	}
	else {
	}
}

void GMetaArchiveReader::doReadProperty(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0 && metaType.isFundamental()) {
		GVariantData data;
		uint32_t archiveID;
		this->reader->readFundamental(name, &archiveID, &data);
		accessible->set(instance, &data);
		return;
	}

	this->doReadField(name, instance, accessible);
}

IMemoryAllocator * G_API_CC GMetaArchiveReader::getAllocator()
{
	if(! this->allocator) {
		this->allocator.reset(new GImplMemoryAllocator);
	}

	return this->allocator.get();
}

GMetaArchiveReaderPointerTracker * GMetaArchiveReader::getPointerTracker()
{
	if(! this->pointerSolver) {
		this->pointerSolver.reset(new GMetaArchiveReaderPointerTracker);
	}

	return this->pointerSolver.get();
}

GMetaArchiveReaderClassTypeTracker * GMetaArchiveReader::getClassTypeTracker()
{
	if(! this->classTypeTracker) {
		this->classTypeTracker.reset(new GMetaArchiveReaderClassTypeTracker);
	}

	return this->classTypeTracker.get();
}


IMetaArchiveReader * createMetaArchiveReader(uint32_t config, IMetaService * service, IMetaReader * reader)
{
	return new GMetaArchiveReader(GMetaArchiveConfig(config), service, reader);
}


} // namespace cpgf



