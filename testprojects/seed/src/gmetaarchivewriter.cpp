#include "gmetaarchivecommon.h"
#include "gmetaarchivecommonimpl.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gapiutil.h"

#include <map>
#include <string>

#include <iostream>

using namespace std;


namespace cpgf {


class GMetaArchiveWriterPointerTracker;
class GMetaArchiveWriterClassTypeTracker;

class GMetaArchiveWriter : public IMetaArchiveWriter
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer);
	~GMetaArchiveWriter();

	virtual IMetaService * G_API_CC getMetaService();

	virtual void G_API_CC writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer);
	virtual void G_API_CC writeMember(const char * name, const void * instance, IMetaAccessible * accessible);

	virtual void G_API_CC trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers);
	
	virtual void G_API_CC beginWriteObject(GMetaArchiveWriterParam * param);
	virtual void G_API_CC endWriteObject(GMetaArchiveWriterParam * param);

protected:
	void writeObjectHelper(const char * name, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, int pointers);
	
	void doWriteObjectHierarchy(GMetaArchiveWriterParam * param, GBaseClassMap * baseClassMap);
	void doWriteObjectWithoutBase(GMetaArchiveWriterParam * param);
	
	void doDirectWriteObjectWithoutBase(GMetaArchiveWriterParam * param);
	
	void doWriteMember(const char * name, const void * instance, IMetaAccessible * accessible);
	
	void doWriteValue(const char * name, const void * instance, const GMetaType & metaType, IMetaSerializer * serializer, int pointers);
	
	uint32_t getClassTypeID(const void * instance, IMetaClass * metaClass, int pointers, IMetaClass ** outCastedMetaClass);

	uint32_t getNextArchiveID();

	GMetaArchiveWriterPointerTracker * getPointerTracker();
	GMetaArchiveWriterClassTypeTracker * getClassTypeTracker();
	
	bool checkTrackedPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, int pointers);

private:
	GMetaArchiveConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaWriter> writer;
	uint32_t currentArchiveID;
	GScopedPointer<GMetaArchiveWriterPointerTracker> pointerSolver;
	GScopedPointer<GMetaArchiveWriterClassTypeTracker> classTypeTracker;
	GClassSerializeHeader serializeHeader;
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

	class GMetaArchiveWriterPointerTrackerValue
	{
	public:
		GMetaArchiveWriterPointerTrackerValue()
			: archiveID(archiveIDNone), pointers(1)
		{
		}

		GMetaArchiveWriterPointerTrackerValue(uint32_t archiveID, int pointers)
			: archiveID(archiveID), pointers(pointers)
		{
		}

	public:
		uint32_t archiveID;
		int pointers;
	};

	typedef std::map<GMetaArchiveWriterPointerTrackerItem, GMetaArchiveWriterPointerTrackerValue> MapType;

public:
	bool hasPointer(const void * instance, const char * typeName) const;
	uint32_t getArchiveID(const void * instance, const char * typeName, int * outPointers) const;
	void addPointer(const void * instance, const char * typeName, uint32_t archiveID, int pointers);

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

uint32_t GMetaArchiveWriterPointerTracker::getArchiveID(const void * instance, const char * typeName, int * outPointers) const
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

void GMetaArchiveWriterPointerTracker::addPointer(const void * instance, const char * typeName, uint32_t archiveID, int pointers)
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

IMetaService * G_API_CC GMetaArchiveWriter::getMetaService()
{
	this->service->addReference();
	return this->service.get();
}

void G_API_CC GMetaArchiveWriter::writeObject(const char * name, const void * instance, const GMetaTypeData * metaType, IMetaSerializer * serializer)
{
	GMetaType type(*metaType);
	
	this->doWriteValue(name, instance, type, serializer, type.getPointerDimension());
}

void G_API_CC GMetaArchiveWriter::writeMember(const char * name, const void * instance, IMetaAccessible * accessible)
{
	this->doWriteMember(name, instance, accessible);
}

void GMetaArchiveWriter::writeObjectHelper(const char * name, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, int pointers)
{
	uint32_t archiveID = this->getNextArchiveID();

	IMetaClass * outCastedMetaClass;
	uint32_t classTypeID = this->getClassTypeID(instance, metaClass, pointers, &outCastedMetaClass);
	GScopedInterface<IMetaClass> castedMetaClass(outCastedMetaClass);

	if(this->checkTrackedPointer(archiveID, instance, castedMetaClass.get(), serializer, pointers)) {
		return;
	}

	GClassSerializeHeaderGuard serializerHeaderGuard(&this->serializeHeader);

	GBaseClassMap baseClassMap;
	GMetaArchiveWriterParam param;
	
	param.name = name;
	param.archiveID = archiveID;
	param.classTypeID = classTypeID;
	param.instance = instance;
	param.metaClass = castedMetaClass.get();
	param.pointers = pointers;
	param.serializer = serializer;
	this->doWriteObjectHierarchy(&param, &baseClassMap);

	if(this->serializeHeader.needEnd()) {
		this->endWriteObject(&param);
	}
}

void GMetaArchiveWriter::doWriteObjectHierarchy(GMetaArchiveWriterParam * param, GBaseClassMap * baseClassMap)
{
	// metaClass can be NULL if serializer is not NULL
	if(param->metaClass != NULL) {
		GScopedInterface<IMetaClass> baseClass;
		uint32_t i;
		uint32_t count;

		count = param->metaClass->getBaseCount();
		for(i = 0; i < count; ++i) {
			baseClass.reset(param->metaClass->getBaseClass(i));
		
			if(canSerializeBaseClass(this->config, baseClass.get(), param->metaClass)) {
				void * baseInstance = param->metaClass->castToBase(param->instance, i);
				if(! baseClassMap->hasMetaClass(baseInstance, baseClass.get())) {
					baseClassMap->addMetaClass(baseInstance, baseClass.get());
					
					GMetaArchiveWriterParam newParam;
					newParam.name = param->name;
					newParam.archiveID = archiveIDNone;
					newParam.classTypeID = param->classTypeID;
					newParam.instance = baseInstance;
					newParam.metaClass = baseClass.get();
					newParam.serializer = NULL;
					newParam.pointers = 0;
					this->doWriteObjectHierarchy(&newParam, baseClassMap);
				}
			}
		}
	}

	this->doWriteObjectWithoutBase(param);
}

void GMetaArchiveWriter::doWriteObjectWithoutBase(GMetaArchiveWriterParam * param)
{
	if(param->serializer != NULL) {
		GMetaArchiveWriterParam newParam(*param);
		param->serializer->writeObject(this, this->writer.get(), &newParam);
	}
	else {
		if(param->metaClass == NULL) {
			serializeError(Error_Serialization_MissingMetaClass);
		}

		this->doDirectWriteObjectWithoutBase(param);
	}
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(GMetaArchiveWriterParam * param)
{
	if(param->instance == NULL) {
		return;
	}
	
	if(this->serializeHeader.needBegin()) {
		this->beginWriteObject(param);
		this->serializeHeader.addedHeader();
	}

	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	if(this->config.allowSerializeField()) {
		count = param->metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getFieldAt(i));

			if(canSerializeField(this->config, accessible.get(), param->metaClass)) {
				this->doWriteMember(accessible->getName(), param->instance, accessible.get());
			}
		}
	}

	if(this->config.allowSerializeProperty()) {
		count = param->metaClass->getPropertyCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getPropertyAt(i));
			if(canSerializeField(this->config, accessible.get(), param->metaClass)) {
				this->doWriteMember(accessible->getName(), param->instance, accessible.get());
			}
		}
	}
}

void GMetaArchiveWriter::doWriteMember(const char * name, const void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	bool isProperty = metaIsProperty(accessible->getCategory());

	if(isProperty) {
		if(pointers == 0 && metaType.isFundamental()) {
			GVariant v(metaGetValue(accessible, instance));
			this->writer->writeFundamental(name, this->getNextArchiveID(), &v.data);
			return;
		}
	}

	if(pointers <= 1) {
		GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
		
		void * ptr;

		if(metaType.baseIsArray()) {
			ptr = accessible->getAddress(instance);
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
			}
			else {
				ptr = fromVariant<void *>(metaGetValue(accessible, instance));
			}
		}

		GASSERT(ptr != NULL || serializer);

		this->doWriteValue(name, ptr, metaType, serializer.get(), pointers);
	}
	else {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

void GMetaArchiveWriter::doWriteValue(const char * name, const void * address, const GMetaType & metaType, IMetaSerializer * serializer, int pointers)
{
	if(metaType.baseIsArray()) {
		this->writeObjectHelper(name, address, NULL, serializer, 0);
		return;
	}

	bool isPointer = pointers > 0;

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
			if(metaType.getBaseName() != NULL) {
				metaClass.reset(this->service->findClassByName(metaType.getBaseName()));
			}
			this->writeObjectHelper(name, address, metaClass.get(), serializer, pointers);
		}
	}
}

bool GMetaArchiveWriter::checkTrackedPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, int pointers)
{
	if(this->config.allowTrackPointer()) {
		const char * typeName = NULL;
		if(serializer != NULL) {
			typeName = serializer->getClassTypeName(this, instance, metaClass);
		}
		if(typeName == NULL && metaClass != NULL) {
			typeName = metaClass->getTypeName();
		}

		if(typeName == NULL) {
			return false;
		}

		if(this->getPointerTracker()->hasPointer(instance, typeName)) {
			int referencePointers;
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

				this->writer->writeReferenceID("", archiveID, referenceID);
			}
				return true;
		}
	}

	return false;
}

void G_API_CC GMetaArchiveWriter::trackPointer(uint32_t archiveID, const void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, uint32_t pointers)
{
	if(this->config.allowTrackPointer()) {
		const char * typeName = NULL;
		if(serializer != NULL) {
			typeName = serializer->getClassTypeName(this, instance, metaClass);
		}
		if(typeName == NULL && metaClass != NULL) {
			typeName = metaClass->getTypeName();
		}

		if(typeName != NULL) {
			if(! this->getPointerTracker()->hasPointer(instance, typeName)) {
				this->getPointerTracker()->addPointer(instance, typeName, archiveID, pointers);
			}
		}
	}
}

uint32_t GMetaArchiveWriter::getClassTypeID(const void * instance, IMetaClass * metaClass, int pointers, IMetaClass ** outCastedMetaClass)
{
	uint32_t classTypeID = archiveIDNone;

	*outCastedMetaClass = NULL;

	if(metaClass != NULL) {
		if(pointers > 0) {
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

void G_API_CC GMetaArchiveWriter::beginWriteObject(GMetaArchiveWriterParam * param)
{
	this->trackPointer(param->archiveID, param->instance, param->metaClass, param->serializer, param->pointers);
	
	this->writer->beginWriteObject(param);
}

void G_API_CC GMetaArchiveWriter::endWriteObject(GMetaArchiveWriterParam * param)
{
	this->writer->endWriteObject(param);
}

IMetaArchiveWriter * createMetaArchiveWriter(uint32_t config, IMetaService * service, IMetaWriter * writer)
{
	return new GMetaArchiveWriter(GMetaArchiveConfig(config), service, writer);
}


} // namespace cpgf



