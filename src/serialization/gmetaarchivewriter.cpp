#include "cpgf/serialization/gmetaarchivewriter.h"
#include "../pinclude/gmetaarchivecommonimpl.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gerrorcode.h"

#include <map>
#include <string>

#include <iostream>

using namespace std;


namespace cpgf {


class GMetaArchiveWriterPointerTracker;
class GMetaArchiveWriterClassTypeTracker;

class GMetaArchiveWriter : public IMetaArchiveWriter, public IMetaSerializerWriter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaArchiveWriter(IMetaService * service, IMetaStorageWriter * writer);
	virtual ~GMetaArchiveWriter();

	virtual IMetaService * G_API_CC getMetaService();
	virtual IMetaStorageWriter * G_API_CC getStorageWriter();
	
	virtual void G_API_CC writeData(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer);
	virtual void G_API_CC writeMember(GMetaArchiveWriterParam * param, IMetaAccessible * accessible);

	virtual void G_API_CC trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers);
	
protected:
	void writeObjectHelper(const char * name, const void * instance, const GMetaType & metaType, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers);
	
	void doWriteObjectHierarchy(GMetaArchiveWriterParam * param, GBaseClassMap * baseClassMap);
	void doWriteObjectWithoutBase(GMetaArchiveWriterParam * param);
	
	void doDirectWriteObjectWithoutBase(GMetaArchiveWriterParam * param);
	
	void doWriteMember(const void * instance, IMetaAccessible * accessible);
	
	void doWriteValue(const char * name, const void * instance, const GMetaType & metaType, IMetaSerializer * serializer, uint32_t pointers);
	
	uint32_t getClassTypeID(const void * instance, IMetaClass * metaClass, uint32_t pointers, IMetaClass ** outCastedMetaClass);

	uint32_t getNextArchiveID();

	GMetaArchiveWriterPointerTracker * getPointerTracker();
	GMetaArchiveWriterClassTypeTracker * getClassTypeTracker();
	
	bool checkTrackedPointer(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers);
	void checkBeginWriteObject(GMetaArchiveWriterParam * param);
	void doBeginWriteObject(GMetaArchiveWriterParam * param);
	void doEndWriteObject(GMetaArchiveWriterParam * param);

private:
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaStorageWriter> writer;
	uint32_t currentArchiveID;
	GScopedPointer<GMetaArchiveWriterPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveWriterClassTypeTracker> classTypeTracker;
	GClassSerializeHeader serializeHeader;
	GMetaArchiveConfigMap configMap;
};


class GMetaArchiveWriterPointerTracker
{
private:
	class GMetaArchiveWriterPointerTrackerItem
	{
	public:
		GMetaArchiveWriterPointerTrackerItem()
			: instance(nullptr), typeName()
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

	class GMetaArchiveWriterPointerTrackerValue
	{
	public:
		GMetaArchiveWriterPointerTrackerValue()
			: archiveID(archiveIDNone), pointers(1)
		{
		}

		GMetaArchiveWriterPointerTrackerValue(uint32_t archiveID, uint32_t pointers)
			: archiveID(archiveID), pointers(pointers)
		{
		}

	public:
		uint32_t archiveID;
		uint32_t pointers;
	};

	typedef std::map<GMetaArchiveWriterPointerTrackerItem, GMetaArchiveWriterPointerTrackerValue> MapType;

public:
	bool hasPointer(const void * instance, const char * typeName) const;
	uint32_t getArchiveID(const void * instance, const char * typeName, uint32_t * outPointers) const;
	void addPointer(const void * instance, const char * typeName, uint32_t archiveID, uint32_t pointers);

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

uint32_t GMetaArchiveWriterPointerTracker::getArchiveID(const void * instance, const char * typeName, uint32_t * outPointers) const
{
	*outPointers = 1;

	MapType::const_iterator it = this->pointerMap.find(GMetaArchiveWriterPointerTrackerItem(instance, typeName));

	if(it == this->pointerMap.end()) {
		return archiveIDNone;
	}
	else {
		*outPointers = it->second.pointers;
		return it->second.archiveID;
	}
}

void GMetaArchiveWriterPointerTracker::addPointer(const void * instance, const char * typeName, uint32_t archiveID, uint32_t pointers)
{
	GASSERT(! this->hasPointer(instance, typeName));

	this->pointerMap.insert(pair<GMetaArchiveWriterPointerTrackerItem, GMetaArchiveWriterPointerTrackerValue>(
		GMetaArchiveWriterPointerTrackerItem(instance, typeName), GMetaArchiveWriterPointerTrackerValue(archiveID, pointers))
	);
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


GMetaArchiveWriter::GMetaArchiveWriter(IMetaService * service, IMetaStorageWriter * writer)
	: service(service), writer(writer), currentArchiveID(0)
{
	if(this->service) {
		this->service->addReference();
	}

	this->writer->addReference();
}

GMetaArchiveWriter::~GMetaArchiveWriter()
{
}

IMetaService * G_API_CC GMetaArchiveWriter::getMetaService()
{
	this->service->addReference();
	return this->service.get();
}

IMetaStorageWriter * G_API_CC GMetaArchiveWriter::getStorageWriter()
{
	this->writer->addReference();
	return this->writer.get();
}

void G_API_CC GMetaArchiveWriter::writeData(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer)
{
	GMetaType type(*metaType);
	
	this->doWriteValue(name, instance, type, serializer, type.getPointerDimension());
}

void G_API_CC GMetaArchiveWriter::writeMember(GMetaArchiveWriterParam * param, IMetaAccessible * accessible)
{
	this->checkBeginWriteObject(param);
	this->doWriteMember(param->instance, accessible);
}

void GMetaArchiveWriter::writeObjectHelper(const char * name, const void * instance, const GMetaType & metaType, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers)
{
	uint32_t archiveID = this->getNextArchiveID();

	IMetaClass * outCastedMetaClass;
	uint32_t classTypeID = this->getClassTypeID(instance, metaClass, pointers, &outCastedMetaClass);
	GScopedInterface<IMetaClass> castedMetaClass(outCastedMetaClass);

	if(this->checkTrackedPointer(name, archiveID, instance, castedMetaClass.get(), serializer, pointers)) {
		return;
	}

	GClassSerializeHeaderGuard serializerHeaderGuard(&this->serializeHeader);

	GBaseClassMap baseClassMap;
	GMetaArchiveWriterParam param;
	GMetaTypeData metaTypeData = metaType.refData();
	
	param.name = name;
	param.archiveID = archiveID;
	param.classTypeID = classTypeID;
	param.instance = instance;
	param.metaType = &metaTypeData;
	param.metaClass = castedMetaClass.get();
	param.pointers = pointers;
	param.serializer = serializer;
	param.config = this->configMap.getConfig(castedMetaClass.get()).getData();

	this->doWriteObjectHierarchy(&param, &baseClassMap);

	if(this->serializeHeader.needEnd()) {
		this->doEndWriteObject(&param);
	}
}

void GMetaArchiveWriter::doWriteObjectHierarchy(GMetaArchiveWriterParam * param, GBaseClassMap * baseClassMap)
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
					
					GMetaType baseMetaType(metaGetItemType(baseClass.get()));
					GMetaTypeData baseMetaTypeData = baseMetaType.refData();

					GMetaArchiveWriterParam newParam;
					newParam.name = param->name;
					newParam.archiveID = archiveIDNone;
					newParam.classTypeID = param->classTypeID;
					newParam.instance = baseInstance;
					newParam.metaType = &baseMetaTypeData;
					newParam.metaClass = baseClass.get();
					newParam.serializer = serializer.get();
					newParam.pointers = 0;
					newParam.config = this->configMap.getConfig(baseClass.get()).getData();

					this->doWriteObjectHierarchy(&newParam, baseClassMap);
				}
			}
		}
	}

	this->doWriteObjectWithoutBase(param);
}

void GMetaArchiveWriter::doWriteObjectWithoutBase(GMetaArchiveWriterParam * param)
{
	if(param->serializer != nullptr) {
		GMetaArchiveWriterParam newParam(*param);
		param->serializer->writeObject(this, this, &newParam);
	}
	else {
		if(param->metaClass == nullptr) {
			serializeError(Error_Serialization_MissingMetaClass);
		}

		this->doDirectWriteObjectWithoutBase(param);
	}
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(GMetaArchiveWriterParam * param)
{
	metaSerializerWriteObjectMembers(this, this, param);
}

void GMetaArchiveWriter::doWriteMember(const void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	uint32_t pointers = metaType.getPointerDimension();
	const char * name = accessible->getName();

	bool isProperty = metaIsProperty(accessible->getCategory());

	if(isProperty) {
		if(pointers == 0 && metaType.isFundamental()) {
			GVariant v(metaGetValue(accessible, instance));
			this->writer->writeFundamental(name, &v.refData());
			return;
		}
	}

	if(pointers <= 1) {
		GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
		
		void * ptr;
		GVariant valueHolder;

		if(metaType.baseIsArray()) {
			ptr = accessible->getAddress(instance);
		}
		else {
			if(pointers == 0) {
				ptr = accessible->getAddress(instance);
				if(ptr == nullptr) { // this happens when accessible is a property with both getter and setter.
					valueHolder = GVariant(metaGetValue(accessible, instance));
					if(canFromVariant<void *>(valueHolder)) {
						ptr = objectAddressFromVariant(valueHolder);
					}
				}
			}
			else {
				ptr = objectAddressFromVariant(metaGetValue(accessible, instance));
			}
		}

		GASSERT(ptr != nullptr || serializer);

		this->doWriteValue(name, ptr, metaType, serializer.get(), pointers);
	}
	else {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

void GMetaArchiveWriter::doWriteValue(const char * name, const void * address, const GMetaType & metaType, IMetaSerializer * serializer, uint32_t pointers)
{
	if(metaType.baseIsArray()) {
		this->writeObjectHelper(name, address, metaType, nullptr, serializer, 0);
		return;
	}

	bool isPointer = pointers > 0;

	if(!isPointer && metaType.isFundamental()) {
		GVariant v(readFundamental(address, metaType));
		this->writer->writeFundamental(name, &v.refData());
		return;
	}

	if(pointers > 1) {
		serializeError(Error_Serialization_UnknownType);
	}

	if(metaType.baseIsClass()) {
		if(address == nullptr && isPointer) {
			this->writer->writeNullPointer(name);
		}
		else {
			GScopedInterface<IMetaClass> metaClass;
			if(metaType.getBaseName() != nullptr) {
				metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
			}
			this->writeObjectHelper(name, address, metaType, metaClass.get(), serializer, pointers);
		}
	}
}

bool GMetaArchiveWriter::checkTrackedPointer(const char * name, uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers)
{
	GMetaArchiveConfig config = this->configMap.getConfig(metaClass);

	if(config.allowTrackPointer()) {
		const char * typeName = nullptr;
		if(serializer != nullptr) {
			typeName = serializer->getClassTypeName(this, instance, metaClass);
		}
		if(typeName == nullptr && metaClass != nullptr) {
			typeName = metaClass->getQualifiedName();
		}

		if(typeName == nullptr) {
			return false;
		}

		if(this->getPointerTracker()->hasPointer(instance, typeName)) {
			uint32_t referencePointers;
			uint32_t referenceID = this->getPointerTracker()->getArchiveID(instance, typeName, &referencePointers);

			if(pointers == 0) {
				if(referencePointers > 0) { // if the object is stored via a pointer firstly, error!
					serializeError(Error_Serialization_TypeMismatch);
				}
				return false;
			}
			else {
				if(archiveID == archiveIDNone) {
					archiveID = this->getNextArchiveID();
				}

				this->writer->writeReferenceID(name, referenceID);
			}
				return true;
		}
	}

	return false;
}

void G_API_CC GMetaArchiveWriter::trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers)
{
	GMetaArchiveConfig config = this->configMap.getConfig(metaClass);

	if(config.allowTrackPointer()) {
		const char * typeName = nullptr;
		if(serializer != nullptr) {
			typeName = serializer->getClassTypeName(this, instance, metaClass);
		}
		if(typeName == nullptr && metaClass != nullptr) {
			typeName = metaClass->getQualifiedName();
		}

		if(typeName != nullptr) {
			if(! this->getPointerTracker()->hasPointer(instance, typeName)) {
				this->getPointerTracker()->addPointer(instance, typeName, archiveID, pointers);
			}
		}
	}
}

uint32_t GMetaArchiveWriter::getClassTypeID(const void * instance, IMetaClass * metaClass, uint32_t pointers, IMetaClass ** outCastedMetaClass)
{
	uint32_t classTypeID = classIDNone;

	*outCastedMetaClass = nullptr;

	if(metaClass != nullptr) {
		if(pointers > 0) {
			void * castedPtr;
			GScopedInterface<IMetaClass> castedMetaClass(findAppropriateDerivedClass(instance, metaClass, &castedPtr));
			if(! castedMetaClass->equals(metaClass)) {
				const char * typeName = castedMetaClass->getQualifiedName();
				if(this->getClassTypeTracker()->hasClassType(typeName)) {
					classTypeID = this->getClassTypeTracker()->getArchiveID(typeName);
				}
				else {
					classTypeID = this->getNextArchiveID();
					this->getClassTypeTracker()->addClassType(typeName, classTypeID);
					this->writer->writeMetaClass(classTypeID, castedMetaClass.get());
				}
				*outCastedMetaClass = castedMetaClass.take();
			}
		}
		
		if(*outCastedMetaClass == nullptr) {
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

void GMetaArchiveWriter::checkBeginWriteObject(GMetaArchiveWriterParam * param)
{
	if(this->serializeHeader.needBegin()) {
		this->doBeginWriteObject(param);
		this->serializeHeader.addedHeader();
	}
}

void GMetaArchiveWriter::doBeginWriteObject(GMetaArchiveWriterParam * param)
{
	this->trackPointer(param->archiveID, param->instance, param->metaClass, param->serializer, param->pointers);
	
	this->writer->beginWriteObject(param->name, param->archiveID, param->classTypeID, param->config.version);
}

void GMetaArchiveWriter::doEndWriteObject(GMetaArchiveWriterParam * param)
{
	this->writer->endWriteObject(param->name, param->archiveID, param->classTypeID, param->config.version);
}

IMetaArchiveWriter * createMetaArchiveWriter(IMetaService * service, IMetaStorageWriter * writer)
{
	return new GMetaArchiveWriter(service, writer);
}


void serializeWriteObject(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass)
{
	GMetaTypeData metaType = metaGetItemType(metaClass).refData();
	GScopedInterface<IMetaSerializer> serializer;
	if(metaClass != nullptr) {
		serializer.reset(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_Serializer).getSerializer());
	}
	archiveWriter->writeData(name, instance, &metaType, serializer.get());
}

void metaSerializerWriteObjectMembers(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param)
{
	if(param->instance == nullptr) {
		return;
	}
	
	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	GMetaArchiveConfig config(getItemMetaArchiveConfig(param->metaClass));
	GScopedInterface<IMetaService> service(archiveWriter->getMetaService());

	if(config.allowSerializeField()) {
		count = param->metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getFieldAt(i));

			if(canSerializeField(config, accessible.get(), service.get())) {
				serializerWriter->writeMember(param, accessible.get());
			}
		}
	}

	if(config.allowSerializeProperty()) {
		count = param->metaClass->getPropertyCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getPropertyAt(i));
			if(canSerializeField(config, accessible.get(), service.get())) {
				serializerWriter->writeMember(param, accessible.get());
			}
		}
	}
}



} // namespace cpgf



