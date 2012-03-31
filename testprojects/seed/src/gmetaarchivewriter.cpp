#include "gmetaarchivecommon.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gapiutil.h"

#include <map>
#include <string>

#include <iostream>

using namespace std;


namespace cpgf {


enum GMetaArchivePointerType {
	aptByValue, aptByPointer
};

class GMetaArchiveWriterPointerTracker;
class GMetaArchiveWriterClassTypeTracker;

class GMetaArchiveWriter : public IMetaArchiveWriter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer);
	~GMetaArchiveWriter();

	// take care of customized serializer, take care of pointer tracking.
	virtual void G_API_CC writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer);

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID);
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID);

protected:
	void writeObjectHelper(const char * name, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType);
	
	void doWriteObjectHierarchy(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GBaseClassMap * baseClassMap);
	void doWriteObjectWithoutBase(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer);
	
	void doDirectWriteObjectWithoutBase(uint32_t archiveID, const void * instance, IMetaClass * metaClass);
	
	void doWriteField(const char * name, const void * instance, IMetaAccessible * accessible);
	void doWriteProperty(const char * name, const void * instance, IMetaAccessible * accessible);
	
	void doWriteValue(const char * name, const void * instance, const GMetaType & metaType, IMetaSerializer * serializer, GMetaArchivePointerType pointerType);
	
	uint32_t getClassTypeID(const void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType, IMetaClass ** outCastedMetaClass);

	uint32_t getNextArchiveID();

	GMetaArchiveWriterPointerTracker * getPointerTracker();
	GMetaArchiveWriterClassTypeTracker * getClassTypeTracker();
	
	bool trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType);

private:
	GMetaArchiveConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaWriter> writer;
	uint32_t currentArchiveID;
	GScopedPointer<GMetaArchiveWriterPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveWriterClassTypeTracker> classTypeTracker;
};


class GMetaArchiveWriterPointerTracker
{
private:
	class GMetaArchiveWriterPointerTrackerItem
	{
	public:
		GMetaArchiveWriterPointerTrackerItem()
			: instance(NULL), typeName()
		{
		}

		GMetaArchiveWriterPointerTrackerItem(const void * instance, const char * typeName)
			: instance(instance), typeName(typeName)
		{
		}

		bool operator < (const GMetaArchiveWriterPointerTrackerItem & other) const {
			return this->instance < other.instance
				|| (this->instance == other.instance && this->typeName < other.typeName)
				;
		}

	public:
		const void * instance;
		string typeName;
	};

	typedef std::map<GMetaArchiveWriterPointerTrackerItem, uint32_t> MapType;

public:
	bool hasPointer(const void * instance, const char * typeName) const;
	uint32_t getArchiveID(const void * instance, const char * typeName) const;
	void addPointer(const void * instance, const char * typeName, uint32_t archiveID);

private:
	MapType pointerMap;
};

class GMetaArchiveWriterClassTypeTracker
{
private:
	typedef std::map<string, uint32_t> MapType;

public:
	bool hasClassType(const string & classType) const;
	uint32_t getArchiveID(const string & classType) const;
	void addClassType(const string & classType, uint32_t archiveID);

private:
	MapType classTypeMap;
};


bool GMetaArchiveWriterPointerTracker::hasPointer(const void * instance, const char * typeName) const
{
	return this->pointerMap.find(GMetaArchiveWriterPointerTrackerItem(instance, typeName)) != this->pointerMap.end();
}

uint32_t GMetaArchiveWriterPointerTracker::getArchiveID(const void * instance, const char * typeName) const
{
	MapType::const_iterator it = this->pointerMap.find(GMetaArchiveWriterPointerTrackerItem(instance, typeName));

	if(it == this->pointerMap.end()) {
		return archiveIDNone;
	}
	else {
		return it->second;
	}
}

void GMetaArchiveWriterPointerTracker::addPointer(const void * instance, const char * typeName, uint32_t archiveID)
{
	GASSERT(! this->hasPointer(instance, typeName));

	this->pointerMap.insert(pair<GMetaArchiveWriterPointerTrackerItem, uint32_t>(GMetaArchiveWriterPointerTrackerItem(instance, typeName), archiveID));
}


bool GMetaArchiveWriterClassTypeTracker::hasClassType(const string & classType) const
{
	return this->classTypeMap.find(classType) != this->classTypeMap.end();
}

uint32_t GMetaArchiveWriterClassTypeTracker::getArchiveID(const string & classType) const
{
	MapType::const_iterator it = this->classTypeMap.find(classType);

	if(it == this->classTypeMap.end()) {
		return archiveIDNone;
	}
	else {
		return it->second;
	}
}

void GMetaArchiveWriterClassTypeTracker::addClassType(const string & classType, uint32_t archiveID)
{
	GASSERT(! this->hasClassType(classType));
	
	this->classTypeMap.insert(pair<string, uint32_t>(classType, archiveID));
}


GMetaArchiveWriter::GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer)
	: config(config), service(service), writer(writer), currentArchiveID(0)
{
	if(this->service) {
		this->service->addReference();
	}

	this->writer->addReference();
}

GMetaArchiveWriter::~GMetaArchiveWriter()
{
}

void G_API_CC GMetaArchiveWriter::writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer)
{
	GMetaType type(*metaType);
	
	if(type.isPointer()) {
		this->doWriteValue(name, instance, type, serializer, aptByPointer);
	}
	else {
		this->doWriteValue(name, instance, type, serializer, aptByValue);
	}
}

void GMetaArchiveWriter::writeObjectHelper(const char * name, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType)
{
	uint32_t archiveID = this->getNextArchiveID();

	if(this->trackPointer(archiveID, instance, metaClass, serializer, pointerType)) {
		return;
	}

	IMetaClass * outCastedMetaClass;
	uint32_t classTypeID = this->getClassTypeID(instance, metaClass, pointerType, &outCastedMetaClass);
	GScopedInterface<IMetaClass> castedMetaClass(outCastedMetaClass);

	this->beginWriteObject(name, archiveID, instance, castedMetaClass.get(), classTypeID);

	GBaseClassMap baseClassMap;
	this->doWriteObjectHierarchy(archiveID, instance, castedMetaClass.get(), serializer, &baseClassMap);

	this->endWriteObject(name, archiveID, instance, castedMetaClass.get(), classTypeID);
}

void GMetaArchiveWriter::doWriteObjectHierarchy(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GBaseClassMap * baseClassMap)
{
	// metaClass can be NULL if serializer is not NULL
	if(metaClass != NULL) {
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
					this->doWriteObjectHierarchy(archiveIDNone, baseInstance, baseClass.get(), NULL, baseClassMap);
				}
			}
		}
	}

	this->doWriteObjectWithoutBase(archiveID, instance, metaClass, serializer);
}

void GMetaArchiveWriter::doWriteObjectWithoutBase(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer)
{
	GScopedInterface<IMetaSerializer> serializerPointer;
	if(serializer == NULL && metaClass != NULL) {
		serializerPointer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
		serializer = serializerPointer.get();
	}

	if(serializer != NULL) {
		serializer->writeObject(this, this->writer.get(), archiveID, instance, metaClass);
	}
	else {
		if(metaClass == NULL) {
			serializeError(Error_Serialization_MissingMetaClass);
		}

		this->doDirectWriteObjectWithoutBase(archiveID, instance, metaClass);
	}
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(uint32_t archiveID, const void * instance, IMetaClass * metaClass)
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
				this->doWriteField(accessible->getName(), instance, accessible.get());
			}
		}
	}

	if(this->config.allowSerializeProperty()) {
		count = metaClass->getPropertyCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(metaClass->getPropertyAt(i));
			if(canSerializeField(this->config, accessible.get(), metaClass)) {
				this->doWriteProperty(accessible->getName(), instance, accessible.get());
			}
		}
	}
}

void GMetaArchiveWriter::doWriteField(const char * name, const void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers <= 1) {
		GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
		
		void * ptr;
		GMetaArchivePointerType pointerType;

		if(metaType.isArray()) {
			ptr = accessible->getAddress(instance);
			pointerType = aptByValue;
		}
		else {
			if(pointers == 0) {
				ptr = accessible->getAddress(instance);
				if(ptr == NULL) { // this happens when accessible is a property with both getter and setter.
					GVariant v(metaGetValue(accessible, instance));
					if(canFromVariant<void *>(v)) {
						ptr = fromVariant<void *>(v);
					}
				}
				pointerType = aptByValue;
			}
			else {
				ptr = fromVariant<void *>(metaGetValue(accessible, instance));
				pointerType = aptByPointer;
			}
		}

		GASSERT(ptr != NULL || serializer);

		this->doWriteValue(name, ptr, metaType, serializer.get(), pointerType);
	}
	else {
	}
}

void GMetaArchiveWriter::doWriteProperty(const char * name, const void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0 && metaType.isFundamental()) {
		GVariant v(metaGetValue(accessible, instance));
		this->writer->writeFundamental(name, this->getNextArchiveID(), &v.data);
		return;
	}

	this->doWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::doWriteValue(const char * name, const void * address, const GMetaType & metaType, IMetaSerializer * serializer, GMetaArchivePointerType pointerType)
{
	if(metaType.isArray()) {
		this->writeObjectHelper(name, address, NULL, serializer, aptByValue);
		return;
	}

	unsigned int pointers = metaType.getPointerDimension();
	bool isPointer = metaType.isPointer();

	if(!isPointer && metaType.isFundamental()) {
		GVariant v(readFundamental(address, metaType));
		this->writer->writeFundamental(name, this->getNextArchiveID(), &v.data);
		return;
	}

	if(pointers > 1) {
		serializeError(Error_Serialization_UnknownType);
	}

	if(metaType.baseIsClass()) {
		if(address == NULL && isPointer) {
			this->writer->writeNullPointer(name);
		}
		else {
			GScopedInterface<IMetaClass> metaClass;
			GScopedInterface<IMetaSerializer> scopedSerializer;
			if(metaType.getBaseName() != NULL) {
				metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
			}
			if(serializer == NULL) {
				scopedSerializer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
				serializer = scopedSerializer.get();
			}
			this->writeObjectHelper(name, address, metaClass.get(), serializer, pointerType);
		}
	}
}

bool GMetaArchiveWriter::trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType)
{
	if(this->config.allowTrackPointer()) {
		const char * typeName = NULL;
		if(serializer != NULL) {
			typeName = serializer->getClassTypeName(this, this->writer.get(), archiveID, NULL, metaClass);
		}
		if(typeName == NULL && metaClass != NULL) {
			typeName = metaClass->getTypeName();
		}

		if(typeName == NULL) {
			return false;
		}

		if(this->getPointerTracker()->hasPointer(instance, typeName)) {
			if(pointerType == aptByValue) {
				serializeError(Error_Serialization_TypeMismatch);
				return false;
			}
			else {
				if(archiveID == archiveIDNone) {
					archiveID = this->getNextArchiveID();
				}
				this->writer->writeReferenceID("", archiveID, this->getPointerTracker()->getArchiveID(instance, typeName));
			}
				return true;
		}

		this->getPointerTracker()->addPointer(instance, typeName, archiveID);
	}

	return false;
}

uint32_t GMetaArchiveWriter::getClassTypeID(const void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType, IMetaClass ** outCastedMetaClass)
{
	uint32_t classTypeID = archiveIDNone;

	*outCastedMetaClass = NULL;

	if(metaClass != NULL) {
		if(pointerType == aptByPointer) {
			void * castedPtr;
			GScopedInterface<IMetaClass> castedMetaClass(findAppropriateDerivedClass(instance, metaClass, &castedPtr));
			if(! castedMetaClass->equals(metaClass)) {
				const char * typeName = castedMetaClass->getTypeName();
				if(this->getClassTypeTracker()->hasClassType(typeName)) {
					classTypeID = this->getClassTypeTracker()->getArchiveID(typeName);
				}
				else {
					classTypeID = this->getNextArchiveID();
					this->getClassTypeTracker()->addClassType(typeName, classTypeID);
					this->writer->writeClassType("", classTypeID, castedMetaClass.get());
				}
				*outCastedMetaClass = castedMetaClass.take();
			}
		}
		
		if(*outCastedMetaClass == NULL) {
			*outCastedMetaClass = metaClass;
			metaClass->addReference();
		}
	}

	return classTypeID;
}

uint32_t GMetaArchiveWriter::getNextArchiveID()
{
	++this->currentArchiveID;
	return this->currentArchiveID;
}

GMetaArchiveWriterPointerTracker * GMetaArchiveWriter::getPointerTracker()
{
	if(! this->pointerSolver) {
		this->pointerSolver.reset(new GMetaArchiveWriterPointerTracker);
	}

	return this->pointerSolver.get();
}

GMetaArchiveWriterClassTypeTracker * GMetaArchiveWriter::getClassTypeTracker()
{
	if(! this->classTypeTracker) {
		this->classTypeTracker.reset(new GMetaArchiveWriterClassTypeTracker);
	}

	return this->classTypeTracker.get();
}

void G_API_CC GMetaArchiveWriter::beginWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveID;
	objectInformation.classTypeID = classTypeID;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->writer->beginWriteObject(&objectInformation);
}

void G_API_CC GMetaArchiveWriter::endWriteObject(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t classTypeID)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveID;
	objectInformation.classTypeID = classTypeID;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->writer->endWriteObject(&objectInformation);
}

IMetaArchiveWriter * createMetaArchiveWriter(uint32_t config, IMetaService * service, IMetaWriter * writer)
{
	return new GMetaArchiveWriter(GMetaArchiveConfig(config), service, writer);
}


} // namespace cpgf



