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

	// take care of customized serializer, take care of pointer tracking.
	virtual void G_API_CC readObject(const char * name, void * instance, IMetaClass * metaClass);
	virtual void G_API_CC readField(const char * name, void * instance, IMetaAccessible * accessible);
	
	// ignore customized serializer, take care of pointer tracking.
	virtual void G_API_CC defaultReaderObject(const char * name, void * instance, IMetaClass * metaClass);

	// ignore customized serializer, ignore pointer tracking, take care of base classes
	virtual void G_API_CC directReadObject(const char * name, void * instance, IMetaClass * metaClass);

	// ignore customized serializer, ignore pointer tracking, ignore base classes, only write the object itself
	virtual void G_API_CC directReadObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass);

	virtual uint32_t G_API_CC beginReadObject(const char * name, void * instance, IMetaClass * metaClass);
	virtual void G_API_CC endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	virtual IMemoryAllocator * G_API_CC getAllocator();
	
protected:
	void * readObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer);
	void * doReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer);
	void doDefaultReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectReadObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
	void doReadField(const char * name, void * instance, IMetaAccessible * accessible);

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
	this->service->addReference();
	this->reader->addReference();
}

GMetaArchiveReader::~GMetaArchiveReader()
{
}

void G_API_CC GMetaArchiveReader::readObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->readObjectHelper(name, instance, metaClass, NULL);
}

void * GMetaArchiveReader::readObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	void * p = this->doReadObject(archiveID, instance, metaClass, serializer);

	this->endReadObject(name, archiveID, instance, metaClass);

	return p;
}

void G_API_CC GMetaArchiveReader::readField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doReadField(name, instance, accessible);
}

void G_API_CC GMetaArchiveReader::defaultReaderObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDefaultReadObject(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

void G_API_CC GMetaArchiveReader::directReadObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDirectReadObject(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

void G_API_CC GMetaArchiveReader::directReadObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDirectReadObjectWithoutBase(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
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

	if(objectInformation.archiveID != archiveIDNone) {
		if(! this->getPointerTracker()->hasArchiveID(objectInformation.archiveID)) {
			this->getPointerTracker()->addArchiveID(objectInformation.archiveID, instance);
		}
	}
	
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

void * GMetaArchiveReader::doReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer)
{
	GScopedInterface<IMetaSerializer> serializerPointer;
	if(serializer == NULL && metaClass != NULL) {
		serializerPointer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
		serializer = serializerPointer.get();
	}

	if(serializer != NULL) {
		return serializer->readObject(this, this->reader.get(), archiveID, instance, metaClass);
	}
	else {
		this->doDefaultReadObject(archiveID, instance, metaClass);
		return instance;
	}
}

void GMetaArchiveReader::doDefaultReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	this->doDirectReadObject(archiveID, instance, metaClass);
}

void GMetaArchiveReader::doDirectReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	GScopedInterface<IMetaClass> baseClass;
	uint32_t i;
	uint32_t count;

	count = metaClass->getBaseCount();
	for(i = 0; i < count; ++i) {
		baseClass.reset(metaClass->getBaseClass(i));
		
		if(canSerializeBaseClass(this->config, baseClass.get(), metaClass)) {
			this->doReadObject(archiveIDNone, metaClass->castToBase(instance, i), baseClass.get(), NULL);
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
				this->readField(accessible->getName(), instance, accessible.get());
			}
		}
	}
}

void GMetaArchiveReader::doReadField(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();
	GVariantData data;
	uint32_t archiveID;

	GMetaArchiveItemType type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));
	switch(type) {
		case matNull: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			this->reader->readNullPointer(name);
			metaSetValue(accessible, instance, GVariant((void *)0));
			break;
		}

		case matFundamental: {
			if(metaType.isFundamental()) {
				this->reader->readFundamental(name, &archiveID, &data);
				accessible->set(instance, &data);
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			break;
		}

		case matString: {
			if(pointers == 1 && vtGetBaseType(metaType.getVariantType()) == vtChar) {
				char * s = this->reader->readString(name, this->getAllocator(), &archiveID);
				this->getAllocator()->free(s);
			}
			break;
		}

		case matObject: {
			if(pointers > 1) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
			if(pointers == 0) {
				if(metaClass) {
					this->readObjectHelper(name, accessible->getAddress(instance), metaClass.get(), NULL);
				}
				else {
					GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
					this->readObjectHelper(name, accessible->getAddress(instance), NULL, serializer.get());
				}
			}
			else {
				void * ptr = fromVariant<void *>(metaGetValue(accessible, instance));
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
					GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
					ptr = this->readObjectHelper(name, ptr, NULL, serializer.get());
				}
				metaSetValue(accessible, instance, ptr);
			}
			break;
		}

		case matReferenceObject: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			archiveID = this->reader->readReferenceID(name);
			if(this->getPointerTracker()->hasArchiveID(archiveID)) {
				metaSetValue(accessible, instance, this->getPointerTracker()->getPointer(archiveID));
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			break;
		}

		case matClassType: {
			GScopedInterface<IMetaClass> metaClass(this->reader->readClassType(name, &archiveID));
			if(metaClass && archiveID != archiveIDNone && ! this->getClassTypeTracker()->hasArchiveID(archiveID)) {
				this->getClassTypeTracker()->addArchiveID(archiveID, metaClass.get());
			}
			break;
		}
	}
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



