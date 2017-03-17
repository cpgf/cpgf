#include "cpgf/serialization/gmetaarchivereader.h"
#include "../pinclude/gmetaarchivecommonimpl.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gerrorcode.h"

#include <map>

using namespace std;

namespace cpgf {


class GMetaArchiveReaderPointerTracker;
class GMetaArchiveReaderClassTypeTracker;

class GMetaArchiveReader : public IMetaArchiveReader, public IMetaSerializerReader
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaArchiveReader(IMetaService * service, IMetaStorageReader * reader);
	virtual ~GMetaArchiveReader();

	virtual IMetaService * G_API_CC getMetaService();
	virtual IMetaStorageReader * G_API_CC getStorageReader();

	virtual void G_API_CC readData(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer);
	
	virtual void G_API_CC readMember(GMetaArchiveReaderParam * param, IMetaAccessible * accessible);

	virtual void G_API_CC trackPointer(uint32_t archiveID, void * instance);

	virtual IMemoryAllocator * G_API_CC getAllocator();
	
protected:
	void * readObjectHelper(const char * name, void * instance, const GMetaType & metaType, IMetaClass * metaClass, IMetaSerializer * serializer);
	
	void * doReadObject(GMetaArchiveReaderParam * param, GBaseClassMap * baseClassMap);
	void doReadObjectHierarchy(GMetaArchiveReaderParam * param, GBaseClassMap * baseClassMap);
	void doReadObjectWithoutBase(GMetaArchiveReaderParam * param);
	void doDirectReadObjectWithoutBase(GMetaArchiveReaderParam * param);
	
	void doReadMember(void * instance, IMetaAccessible * accessible);
	
	void doReadValue(const char * name, void * address, const GMetaType & metaType, IMetaSerializer * serializer);
	bool checkUntrackPointer(const char * name, void * address);

	GMetaArchiveReaderPointerTracker * getPointerTracker();
	GMetaArchiveReaderClassTypeTracker * getClassTypeTracker();

	void checkBeginReadObject(GMetaArchiveReaderParam * param);
	uint32_t doBeginReadObject(GMetaArchiveReaderParam * param);
	void doEndReadObject(GMetaArchiveReaderParam * param);

private:
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaStorageReader> reader;
	GScopedPointer<GMetaArchiveReaderPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveReaderClassTypeTracker> classTypeTracker;
	GScopedInterface<IMemoryAllocator> allocator;
	GClassSerializeHeader serializeHeader;
	GMetaArchiveConfigMap configMap;
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
		return nullptr;
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
		return nullptr;
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


GMetaArchiveReader::GMetaArchiveReader(IMetaService * service, IMetaStorageReader * reader)
	: service(service), reader(reader)
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

IMetaStorageReader * G_API_CC GMetaArchiveReader::getStorageReader()
{
	this->reader->addReference();
	return this->reader.get();
}

void G_API_CC GMetaArchiveReader::readData(const char * name, void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer)
{
	GMetaType type(*metaType);

	if(this->checkUntrackPointer(name, instance)) {
		return;
	}

	this->doReadValue(name, instance, type, serializer);
}

void G_API_CC GMetaArchiveReader::readMember(GMetaArchiveReaderParam * param, IMetaAccessible * accessible)
{
	this->checkBeginReadObject(param);
	this->doReadMember(param->instance, accessible);
}

void * GMetaArchiveReader::readObjectHelper(const char * name, void * instance, const GMetaType & metaType, IMetaClass * metaClass, IMetaSerializer * serializer)
{
	if(this->checkUntrackPointer(name, instance)) {
		return instance;
	}

	GClassSerializeHeaderGuard serializerHeaderGuard(&this->serializeHeader);

	GBaseClassMap baseClassMap;
	GMetaArchiveReaderParam param;
	GMetaTypeData metaTypeData = metaType.refData();
	
	param.name = name;
	param.instance = instance;
	param.metaType = &metaTypeData;
	param.metaClass = metaClass;
	param.serializer = serializer;
	param.config = this->configMap.getConfig(metaClass).getData();
	param.archiveVersion = param.config.version;

	void * p = this->doReadObject(&param, &baseClassMap);

	if(this->serializeHeader.needEnd()) {
		this->doEndReadObject(&param);
	}

	return p;
}

void * GMetaArchiveReader::doReadObject(GMetaArchiveReaderParam * param, GBaseClassMap * baseClassMap)
{
	this->doReadObjectHierarchy(param, baseClassMap);
	return param->instance;
}

void GMetaArchiveReader::doReadObjectHierarchy(GMetaArchiveReaderParam * param, GBaseClassMap * baseClassMap)
{
	// metaClass can be nullptr if serializer is not nullptr
	if(param->metaClass != nullptr) {
		GScopedInterface<IMetaClass> baseClass;
		uint32_t i;
		uint32_t count;
		GScopedInterface<IMetaSerializer> serializer;

		count = param->metaClass->getBaseCount();
		for(i = 0; i < count; ++i) {
			baseClass.reset(param->metaClass->getBaseClass(i));

			GMetaArchiveConfig config = this->configMap.getConfig(baseClass.get());
		
			if(canSerializeBaseClass(config, baseClass.get(), param->metaClass)) {
				void * baseInstance = param->metaClass->castToBase(param->instance, i);
				if(! baseClassMap->hasMetaClass(baseInstance, baseClass.get())) {
					baseClassMap->addMetaClass(baseInstance, baseClass.get());
					serializer.reset(metaGetItemExtendType(baseClass.get(), GExtendTypeCreateFlag_Serializer).getSerializer());
				
					GMetaArchiveReaderParam newParam;
					GMetaType baseMetaType(metaGetItemType(baseClass.get()));
					GMetaTypeData baseMetaTypeData = baseMetaType.refData();
				
					newParam.name = param->name;
					newParam.instance = baseInstance;
					newParam.metaType = &baseMetaTypeData;
					newParam.metaClass = baseClass.get();
					newParam.serializer = serializer.get();
					newParam.config = this->configMap.getConfig(baseClass.get()).getData();
					newParam.archiveVersion = newParam.config.version;
				
					this->doReadObject(&newParam, baseClassMap);
				}
			}
		}
	}

	this->doReadObjectWithoutBase(param);
}

void GMetaArchiveReader::doReadObjectWithoutBase(GMetaArchiveReaderParam * param)
{
	if(param->serializer != nullptr) {
		if(param->instance == nullptr) {
			param->instance = param->serializer->allocateObject(this, param->metaClass);
		}
		param->serializer->readObject(this, this, param);
	}
	else {
		this->doDirectReadObjectWithoutBase(param);
	}
}

void GMetaArchiveReader::doDirectReadObjectWithoutBase(GMetaArchiveReaderParam * param)
{
	metaSerializerReadObjectMembers(this, this, param);
}

class GMetaSerializerScopedGuard
{
public:
	GMetaSerializerScopedGuard(IMetaArchiveReader * reader, IMetaSerializer * serializer, IMetaClass * metaClass, void * instance)
		: reader(reader), serializer(serializer), metaClass(metaClass), instance(instance) {
	}

	~GMetaSerializerScopedGuard() {
		this->serializer->freeObject(this->reader.get(), this->metaClass.get(), this->instance);
	}

private:
	GSharedInterface<IMetaArchiveReader> reader;
	GSharedInterface<IMetaSerializer> serializer;
	GSharedInterface<IMetaClass> metaClass;
	void * instance;
};

class GMetaClassScopedGuard
{
public:
	GMetaClassScopedGuard(IMetaClass * metaClass, void * instance) : metaClass(metaClass), instance(instance) {
	}

	~GMetaClassScopedGuard() {
		this->metaClass->destroyInstance(this->instance);
	}

private:
	GSharedInterface<IMetaClass> metaClass;
	void * instance;
};

void GMetaArchiveReader::doReadMember(void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();
	const char * name = accessible->getName();

	bool isProperty = metaIsProperty(accessible->getCategory());

	if(isProperty) {
		if(pointers == 0 && metaType.isFundamental()) {
			GVariantData data;
			this->reader->readFundamental(name, &data);
			accessible->set(instance, &data);
			return;
		}
	}

	if(pointers <= 1) {
		GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());

		void * ptr;

		ptr = accessible->getAddress(instance);
		if(ptr == nullptr) { // this happens when accessible is a property with both getter and setter.
			if(pointers > 0) {
				void * buffer[sizeof(void *)];
				buffer[0] = nullptr;
				this->doReadValue(name, buffer, metaType, serializer.get());
				metaSetValue(accessible, instance, GVariant(buffer[0]));
				return;
			}
			else {
				// set value via property setter, we need to create a temporary object.
				GScopedInterface<IMetaClass> metaClass;
				if(this->service) {
					metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
				}
				if(serializer) {
					ptr = serializer->allocateObject(this, metaClass.get());
					GMetaSerializerScopedGuard guard(this, serializer.get(), metaClass.get(), ptr);
					this->doReadValue(name, ptr, metaType, serializer.get());
					metaSetValue(accessible, instance, GVariant(ptr));
					return;
				}
				else if(metaClass) {
					ptr = metaClass->createInstance();
					GMetaClassScopedGuard guard(metaClass.get(), ptr);
					this->doReadValue(name, ptr, metaType, serializer.get());
					metaSetValue(accessible, instance, GVariant(ptr));
					return;
				}
				else {
					serializeError(Error_Serialization_TypeMismatch);
				}
			}
		}

		GASSERT(ptr != nullptr || serializer);

		this->doReadValue(name, ptr, metaType, serializer.get());
	}
	else {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

void GMetaArchiveReader::doReadValue(const char * name, void * address, const GMetaType & metaType, IMetaSerializer * serializer)
{
	size_t pointers = metaType.getPointerDimension();
	GVariantData data;
	uint32_t archiveID;

	GMetaArchiveItemType type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));

	if(type == matMissed) {
		return;
	}

	if(type == matClassType) {
		uint32_t classTypeID = classIDNone;
		GScopedInterface<IMetaClass> metaClass(this->reader->readMetaClassAndTypeID(this->service.get(), &classTypeID));
		type = static_cast<GMetaArchiveItemType>(this->reader->getArchiveType(name));
		
		// according to GMetaArchiveWriter, a class type can't follow another class type.
		GASSERT(type != matClassType);

		if(metaClass && classTypeID != classIDNone && ! this->getClassTypeTracker()->hasArchiveID(classTypeID)) {
			this->getClassTypeTracker()->addArchiveID(classTypeID, metaClass.get());
		}
	}

	if(metaType.baseIsArray()) {
		this->readObjectHelper(name, address, metaType, nullptr, serializer);
		return;
	}

	switch(type) {
		case matNull: {
			if(pointers == 0) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			this->reader->readNullPointer(name);
			*(void **)address = nullptr;
			break;
		}

		case matFundamental: {
			if(metaType.isFundamental()) {
				vtSetType(data.typeData, metaType.getVariantType());
				this->reader->readFundamental(name, &data);
				writeFundamental(address, metaType, createVariantFromData(data));
			}
			else {
				serializeError(Error_Serialization_TypeMismatch);
			}
			break;
		}

		case matObject:
		case matCustomized:
		{
			if(pointers > 1) {
				serializeError(Error_Serialization_TypeMismatch);
			}
			GScopedInterface<IMetaClass> metaClass;
			if(this->service) {
				metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
			}
			if(pointers == 0) {
				this->readObjectHelper(name, address, metaType, metaClass.get(), serializer);
			}
			else {
				void * ptr = *(void **)address;
				if(metaClass) {
					if(ptr == nullptr) {
						uint32_t classTypeID = this->reader->getClassTypeID(name);
						if(classTypeID != classIDNone) {
							IMetaClass * storedMetaClass = this->getClassTypeTracker()->getMetaClass(classTypeID);
							
							if(storedMetaClass == nullptr) {
								metaClass.reset(this->reader->readMetaClass(this->service.get(), classTypeID));
								storedMetaClass = metaClass.get();
								if(storedMetaClass == nullptr) {
									serializeError(Error_Serialization_CannotFindObjectType);
								}
								if(metaClass && classTypeID != classIDNone) {
									this->getClassTypeTracker()->addArchiveID(classTypeID, metaClass.get());
								}
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
					this->readObjectHelper(name, ptr, metaType, metaClass.get(), nullptr);
				}
				else {
					ptr = this->readObjectHelper(name, ptr, metaType, nullptr, serializer);
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

void GMetaArchiveReader::checkBeginReadObject(GMetaArchiveReaderParam * param)
{
	if(this->serializeHeader.needBegin()) {
		this->doBeginReadObject(param);
		this->serializeHeader.addedHeader();
	}
}

uint32_t GMetaArchiveReader::doBeginReadObject(GMetaArchiveReaderParam * param)
{
	uint32_t archiveID = this->reader->beginReadObject(param->name, &param->archiveVersion);

	this->trackPointer(archiveID, param->instance);
	
	return archiveID;
}

void GMetaArchiveReader::doEndReadObject(GMetaArchiveReaderParam * param)
{
	this->reader->endReadObject(param->name, param->config.version);
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


IMetaArchiveReader * createMetaArchiveReader(IMetaService * service, IMetaStorageReader * reader)
{
	return new GMetaArchiveReader(service, reader);
}


void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass)
{
	GMetaTypeData metaType = metaGetItemType(metaClass).refData();
	GScopedInterface<IMetaSerializer> serializer;
	if(metaClass != nullptr) {
		serializer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
	}
	archiveReader->readData(name, instance, &metaType, serializer.get());
}

void metaSerializerReadObjectMembers(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param)
{
	if(param->instance == nullptr) {
		return;
	}

	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	GMetaArchiveConfig config(getItemMetaArchiveConfig(param->metaClass));
	GScopedInterface<IMetaService> service(archiveReader->getMetaService());

	if(config.allowSerializeField()) {
		count = param->metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getFieldAt(i));

			if(canSerializeField(config, accessible.get(), service.get())) {
				serializerReader->readMember(param, accessible.get());
			}
		}
	}

	if(config.allowSerializeProperty()) {
		count = param->metaClass->getPropertyCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getPropertyAt(i));
			if(canSerializeField(config, accessible.get(), service.get())) {
				serializerReader->readMember(param, accessible.get());
			}
		}
	}
}



} // namespace cpgf



