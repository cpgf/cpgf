#include "gmetaarchivereader.h"
#include "cpgf/gmetaapiutil.h"

#include "pinclude/gapiimpl.h"

#include <map>

using namespace std;

namespace cpgf {


class GMetaArchiveReaderTracker
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

bool GMetaArchiveReaderTracker::hasArchiveID(uint32_t archiveID) const
{
	return this->pointerMap.find(archiveID) != this->pointerMap.end();
}

void * GMetaArchiveReaderTracker::getPointer(uint32_t archiveID) const
{
	GASSERT(this->hasArchiveID(archiveID));
	
	return this->pointerMap.find(archiveID)->second;
}

void GMetaArchiveReaderTracker::addArchiveID(uint32_t archiveID, void * p)
{
	GASSERT(! this->hasArchiveID(archiveID));

	this->pointerMap.insert(pair<uint32_t, void *>(archiveID, p));
}


GMetaArchiveReader::GMetaArchiveReader(const GMetaArchiveReaderConfig & config, IMetaService * service, IMetaReader * reader)
	: config(config), service(service), reader(reader), pointerSolver(new GMetaArchiveReaderTracker)
{
	this->service->addReference();
	this->reader->addReference();
}

GMetaArchiveReader::~GMetaArchiveReader()
{
}

void GMetaArchiveReader::readObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doReadObject(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveReader::readField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doReadField(name, instance, accessible);
}

void GMetaArchiveReader::defaultReaderObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDefaultReadObject(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveReader::defaultReaderField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDefaultReadField(name, instance, accessible);
}

void GMetaArchiveReader::directReadObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDirectReadObject(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveReader::directReadField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDirectReadField(name, instance, accessible);
}

void GMetaArchiveReader::directReadObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->beginReadObject(name, instance, metaClass);

	this->doDirectReadObjectWithoutBase(archiveID, instance, metaClass);

	this->endReadObject(name, archiveID, instance, metaClass);
}

uint32_t GMetaArchiveReader::beginReadObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = archiveIDNone;
	
	this->reader->beginReadObject(name, instance, metaClass, &archiveID);

	if(archiveID != archiveIDNone) {
		if(! this->pointerSolver->hasArchiveID(archiveID)) {
			this->pointerSolver->addArchiveID(archiveID, instance);
		}
	}
	
	return archiveID;
}

void GMetaArchiveReader::endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	this->reader->endReadObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveReader::doReadObject(uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	IMetaObjectSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->readObject(this, archiveID, instance, metaClass);
	}
	else {
		this->doDefaultReadObject(archiveID, instance, metaClass);
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
			this->doReadObject(archiveIDNone, metaClass->castToBase(instance, i), baseClass.get());
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
	IMetaFieldSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->readField(this, name, instance, accessible);
		return;
	}
	else {
		this->doDefaultReadField(name, instance, accessible);
	}
}

void GMetaArchiveReader::doDefaultReadField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDirectReadField(name, instance, accessible);
}

void GMetaArchiveReader::doDirectReadField(const char * name, void * instance, IMetaAccessible * accessible)
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
				if(! this->allocator) {
					this->allocator.reset(new ImplMemoryAllocator);
				}
				char * s = this->reader->readString(name, this->allocator.get(), &archiveID);
				this->allocator->free(s);
			}
			break;
		}

		case matObject: {
			bool canRead = true;
			if(pointers > 1) {
				canRead = false;
			}
			if(! metaType.baseIsClass() || metaType.getBaseName() == NULL) {
				canRead = false;
			}
			GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
			if(! metaClass) {
				canRead = false;
			}
			if(! canRead) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			if(pointers == 0) {
				this->readObject(metaClass->getName(), accessible->getAddress(instance), metaClass.get());
			}
			else {
				void * ptr = fromVariant<void *>(metaGetValue(accessible, instance));
				if(ptr == NULL) {
					ptr = metaClass->createInstance();
				}
				else {
					void * castedPtr;
					GScopedInterface<IMetaClass> castedMetaClass(findAppropriateDerivedClass(ptr, metaClass.get(), &castedPtr));
					ptr = castedPtr;
					metaClass.reset(castedMetaClass.take());
				}
				metaSetValue(accessible, instance, ptr);
				this->readObject(metaClass->getName(), ptr, metaClass.get());
			}
			break;
		}

		case matReferenceObject: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			archiveID = this->reader->readReferenceID(name);
			if(this->pointerSolver->hasArchiveID(archiveID)) {
				metaSetValue(accessible, instance, this->pointerSolver->getPointer(archiveID));
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			
			break;
		}
	}
}



} // namespace cpgf



