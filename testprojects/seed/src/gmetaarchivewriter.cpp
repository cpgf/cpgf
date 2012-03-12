#include "gmetaarchivewriter.h"
#include "cpgf/gmetaapiutil.h"

#include <map>

using namespace std;


namespace cpgf {


class GMetaArchiveWriterTracker
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

bool GMetaArchiveWriterTracker::hasPointer(void * p) const
{
	return this->pointerMap.find(p) != this->pointerMap.end();
}

uint32_t GMetaArchiveWriterTracker::getArchiveID(void * p) const
{
	GASSERT(this->hasPointer(p));
	
	return this->pointerMap.find(p)->second;
}

void GMetaArchiveWriterTracker::addPointer(void * p, uint32_t archiveID)
{
	GASSERT(! this->hasPointer(p));

	this->pointerMap.insert(pair<void *, uint32_t>(p, archiveID));
}


GMetaArchiveWriter::GMetaArchiveWriter(const GMetaArchiveWriterConfig & config, IMetaService * service, IMetaWriter * writer)
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
	this->writeObjectHelper(name, instance, metaClass, aptByValue);
}

void GMetaArchiveWriter::writeObjectPointer(const char * name, void * instance, IMetaClass * metaClass)
{
	this->writeObjectHelper(name, instance, metaClass, aptByPointer);
}

void GMetaArchiveWriter::writeObjectHelper(const char * name, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	uint32_t archiveID = this->getNextArchiveID();

	this->beginWriteObject(name, archiveID, instance, metaClass);

	this->doWriteObject(archiveID, instance, metaClass, pointerType);

	this->endWriteObject(name, archiveID, instance, metaClass);
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

	this->beginWriteObject(name, archiveID, instance, metaClass);

	this->doDefaultWriteObject(archiveID, instance, metaClass, pointerType);

	this->endWriteObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveWriter::defaultWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDefaultWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::directWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->getNextArchiveID();

	this->beginWriteObject(name, archiveID, instance, metaClass);

	this->doDirectWriteObject(archiveID, instance, metaClass);

	this->endWriteObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveWriter::directWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDirectWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass)
{
	uint32_t archiveID = this->getNextArchiveID();

	this->beginWriteObject(name, archiveID, instance, metaClass);

	this->doDirectWriteObjectWithoutBase(archiveID, instance, metaClass);

	this->endWriteObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveWriter::doWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	IMetaObjectSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->writeObject(this, archiveID, instance, metaClass);
	}
	else {
		this->doDefaultWriteObject(archiveID, instance, metaClass, pointerType);
	}
}

void GMetaArchiveWriter::doDefaultWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	if(pointerType != aptIgnore) {
		if(this->config.allowTrackPointer()) {
			if(! this->pointerSolver) {
				this->pointerSolver.reset(new GMetaArchiveWriterTracker);
			}

			if(this->pointerSolver->hasPointer(instance)) {
				if(pointerType == aptByValue) {
					// error
				}
				else {
					this->writer->writeReferenceID("", this->getNextArchiveID(), this->pointerSolver->getArchiveID(instance));
				}

				return;
			}

			this->pointerSolver->addPointer(instance, archiveID);
		}
	}

	this->doDirectWriteObject(archiveID, instance, metaClass);
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
			this->doWriteObject(archiveIDNone, metaClass->castToBase(instance, i), baseClass.get(), aptIgnore);
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
	IMetaFieldSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->writeField(this, name, instance, accessible);
		return;
	}
	else {
		this->doDefaultWriteField(name, instance, accessible);
	}
}

void GMetaArchiveWriter::doDefaultWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doDirectWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::doDirectWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
		if(metaType.isFundamental()) {
			GVariant v(metaGetValue(accessible, instance));
			this->writer->writeFundamental(name, this->getNextArchiveID(), &v.data);
		}
		else if(metaType.baseIsClass()) {
			if(metaType.getBaseName() != NULL) {
				GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
				if(metaClass) {
					this->writeObjectValue(metaClass->getName(), accessible->getAddress(instance), metaClass.get());
				}
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
					if(metaType.getBaseName() != NULL) {
						GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
						if(metaClass) {
							void * castedPtr;
							GScopedInterface<IMetaClass> castedMetaClass(findAppropriateDerivedClass(ptr, metaClass.get(), &castedPtr));
							this->writeObjectPointer(name, castedPtr, castedMetaClass.get());
						}
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

void GMetaArchiveWriter::beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	this->writer->beginWriteObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveWriter::endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass)
{
	this->writer->endWriteObject(name, archiveID, instance, metaClass);
}



} // namespace cpgf



