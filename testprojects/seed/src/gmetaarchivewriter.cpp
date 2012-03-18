#include "gmetaarchivewriter.h"
#include "cpgf/gmetaapiutil.h"

#include <map>
#include <string>

using namespace std;


namespace cpgf {


class GMetaArchiveWriterPointerTracker
{
private:
	typedef std::map<void *, uint32_t> MapType;

public:
	bool hasPointer(void * p) const;
	uint32_t getArchiveID(void * p) const;
	void addPointer(void * p, uint32_t archiveID);

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


bool GMetaArchiveWriterPointerTracker::hasPointer(void * p) const
{
	return this->pointerMap.find(p) != this->pointerMap.end();
}

uint32_t GMetaArchiveWriterPointerTracker::getArchiveID(void * p) const
{
	MapType::const_iterator it = this->pointerMap.find(p);

	if(it == this->pointerMap.end()) {
		return archiveIDNone;
	}
	else {
		return it->second;
	}
}

void GMetaArchiveWriterPointerTracker::addPointer(void * p, uint32_t archiveID)
{
	GASSERT(! this->hasPointer(p));

	this->pointerMap.insert(pair<void *, uint32_t>(p, archiveID));
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
	GASSERT(this->hasClassType(classType));
	
	this->classTypeMap.insert(pair<string, uint32_t>(classType, archiveID));
}


GMetaArchiveWriter::GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer)
	: config(config), service(service), writer(writer), currentArchiveID(0)
{
	this->service->addReference();
	this->writer->addReference();
}

GMetaArchiveWriter::~GMetaArchiveWriter()
{
}

void GMetaArchiveWriter::writeObjectValue(const char * name, void * instance, IMetaClass * metaClass)
{
	this->writeObjectHelper(name, instance, metaClass, NULL, aptByValue);
}

void GMetaArchiveWriter::writeObjectPointer(const char * name, void * instance, IMetaClass * metaClass)
{
	this->writeObjectHelper(name, instance, metaClass, NULL, aptByPointer);
}

void GMetaArchiveWriter::writeObjectHelper(const char * name, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType)
{
	uint32_t archiveID = this->getNextArchiveID();
	uint32_t classTypeID = this->getClassTypeID(instance, metaClass, pointerType);

	this->beginWriteObject(name, archiveID, instance, metaClass, classTypeID);

	this->doWriteObject(archiveID, instance, metaClass, serializer, pointerType);

	this->endWriteObject(name, archiveID, instance, metaClass, classTypeID);
}

void GMetaArchiveWriter::writeField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::defaultWriteObjectValue(const char * name, void * instance, IMetaClass * metaClass)
{
	this->defaultWriteObjectHelper(name, instance, metaClass, aptByValue);
}

void GMetaArchiveWriter::defaultWriteObjectPointer(const char * name, void * instance, IMetaClass * metaClass)
{
	this->defaultWriteObjectHelper(name, instance, metaClass, aptByPointer);
}

void GMetaArchiveWriter::defaultWriteObjectHelper(const char * name, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	uint32_t archiveID = this->getNextArchiveID();
	uint32_t classTypeID = this->getClassTypeID(instance, metaClass, pointerType);

	this->beginWriteObject(name, archiveID, instance, metaClass, classTypeID);

	this->doDefaultWriteObject(archiveID, instance, metaClass, pointerType);

	this->endWriteObject(name, archiveID, instance, metaClass, classTypeID);
}

void GMetaArchiveWriter::directWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->getNextArchiveID();

	this->beginWriteObject(name, archiveID, instance, metaClass, archiveIDNone);

	this->doDirectWriteObject(archiveID, instance, metaClass);

	this->endWriteObject(name, archiveID, instance, metaClass, archiveIDNone);
}

void GMetaArchiveWriter::directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->getNextArchiveID();

	this->beginWriteObject(name, archiveID, instance, metaClass, archiveIDNone);

	this->doDirectWriteObjectWithoutBase(archiveID, instance, metaClass);

	this->endWriteObject(name, archiveID, instance, metaClass, archiveIDNone);
}

void GMetaArchiveWriter::doWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, IMetaSerializer * serializer, GMetaArchivePointerType pointerType)
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
		this->doDefaultWriteObject(archiveID, instance, metaClass, pointerType);
	}
}

void GMetaArchiveWriter::doDefaultWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	if(metaClass == NULL) {
		serializeError(Error_Serialization_MissingMetaClass);
	}
	
	if(pointerType != aptIgnore) {
		if(this->config.allowTrackPointer()) {

			if(this->getPointerTracker()->hasPointer(instance)) {
				if(pointerType == aptByValue) {
					// error
				}
				else {
					this->writer->writeReferenceID("", this->getNextArchiveID(), this->getPointerTracker()->getArchiveID(instance));
				}

				return;
			}

			this->getPointerTracker()->addPointer(instance, archiveID);
		}
	}

	this->doDirectWriteObject(archiveID, instance, metaClass);
}

uint32_t GMetaArchiveWriter::getClassTypeID(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	uint32_t classTypeID = archiveIDNone;

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
					this->writer->writeClassType("", classTypeID, metaClass);
				}
			}
		}
	}

	return classTypeID;
}

void GMetaArchiveWriter::doDirectWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	GScopedInterface<IMetaClass> baseClass;
	uint32_t i;
	uint32_t count;

	count = metaClass->getBaseCount();
	for(i = 0; i < count; ++i) {
		baseClass.reset(metaClass->getBaseClass(i));
		
		if(canSerializeBaseClass(this->config, baseClass.get(), metaClass)) {
			this->doWriteObject(archiveIDNone, metaClass->castToBase(instance, i), baseClass.get(), NULL, aptIgnore);
		}
	}

	this->doDirectWriteObjectWithoutBase(archiveID, instance, metaClass);
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass)
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
				this->doWriteField(accessible->getName(), instance, accessible.get());
			}
		}
	}
}

void GMetaArchiveWriter::doWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
		if(metaType.isFundamental()) {
			GVariant v(metaGetValue(accessible, instance));
			this->writer->writeFundamental(name, this->getNextArchiveID(), &v.data);
		}
		else if(metaType.baseIsClass()) {
			bool written = false;
			if(metaType.getBaseName() != NULL) {
				GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
				if(metaClass) {
					this->writeObjectHelper(name, accessible->getAddress(instance), metaClass.get(), NULL, aptByValue);
					written = true;
				}
			}
			if(! written) {
				GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
				this->writeObjectHelper(name, accessible->getAddress(instance), NULL, serializer.get(), aptByValue);
			}
		}
	}
	else {
		if(pointers == 1) {
			void * ptr = fromVariant<void *>(metaGetValue(accessible, instance));
			if(ptr == NULL) {
				this->writer->writeNullPointer(name);
			}
			else {
				if(vtGetBaseType(metaType.getVariantType()) == vtChar) {
					this->writer->writeString(name, this->getNextArchiveID(), static_cast<char *>(ptr));
				}
				else if(metaType.baseIsClass()) {
					bool written = false;
					if(metaType.getBaseName() != NULL) {
						GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
						if(metaClass) {
							this->writeObjectHelper(name, ptr, metaClass.get(), NULL, aptByPointer);
							written = true;
						}
					}
					if(! written) {
						GScopedInterface<IMetaSerializer> serializer(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Serializer).getSerializer());
						this->writeObjectHelper(name, ptr, NULL, serializer.get(), aptByPointer);
					}
				}
			}
		}
		else {
		}
	}
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

void GMetaArchiveWriter::beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, uint32_t classTypeID)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveID;
	objectInformation.classTypeID = classTypeID;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->writer->beginWriteObject(&objectInformation);
}

void GMetaArchiveWriter::endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass, uint32_t classTypeID)
{
	GMetaArchiveObjectInformation objectInformation;
	
	objectInformation.name = name;
	objectInformation.archiveID = archiveID;
	objectInformation.classTypeID = classTypeID;
	objectInformation.instance = instance;
	objectInformation.metaClass = metaClass;

	this->writer->endWriteObject(&objectInformation);
}



} // namespace cpgf



