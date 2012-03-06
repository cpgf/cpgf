#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gflags.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <map>


#include <iostream>
using namespace std;


namespace cpgf {

const char * const SerializationAnnotation = "serialize";
const char * const SerializationAnnotationEnable = "enable";

enum GMetaArchivePointerType {
	aptByValue, aptByPointer, aptIgnore
};


struct IMetaArchiveWriter {};

const uint32_t archiveIDNone = 0;

class GMetaArchiveConfig
{
private:
	enum ConfigFlags {
		macResolvePointers = 1 << 0
	};

	enum {
		defaultConfig = macResolvePointers
	};

public:
	GMetaArchiveConfig() : flags(defaultConfig) {
	}

	void setAllowReslovePointers(bool allow) {
		this->flags.setByBool(macResolvePointers, allow);
	}

	bool allowReslovePointers() const {
		return this->flags.has(macResolvePointers);
	}

private:
	GFlags<ConfigFlags> flags;
};

struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariant & value) = 0;
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
	virtual void G_API_CC writeNullPointer(const char * name, uint32_t archiveID) = 0;

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) = 0;

//	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
//	virtual void G_API_CC endWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
};

template <typename STREAM>
class GStreamMetaWriter : public IMetaWriter
{
public:
	explicit GStreamMetaWriter(STREAM & stream) : stream(stream), referenceCount(1), indent(0) {
	}

protected:
	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariant & value) {
		this->writeIndent();
		
		this->stream << archiveID << "  " << name << " -- ";

		switch(value.getType()) {
			case vtSignedInt:
				this->stream << fromVariant<int>(value);
				break;

			default:
				break;
		}
		
		this->stream << endl;
	}

	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) {
		this->writeIndent();
		
		this->stream << archiveID << "  " << name << " -- " << value << endl;
	}

	virtual void G_API_CC writeNullPointer(const char * name, uint32_t archiveID) {
		this->writeIndent();
		
		this->stream << archiveID << "  " << name << " -- " << "NULL" << endl;
	}

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		this->writeIndent();
		
		++this->indent;

		this->stream << archiveID << " beginObject  " << name << endl;
	}

	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		--this->indent;
		this->writeIndent();
		
		this->stream << archiveID << " endObject  " << name << endl;
	}

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) {
		this->writeIndent();
		
		this->stream << archiveID << " Reference  " << name << "  " << referenceArchiveID << endl;
	}

protected:
	void writeIndent() {
		for(int i = 0; i < indent; ++i) {
			this->stream << "    ";
		}
	}

	uint32_t G_API_CC unused_queryInterface(void *, void *)
	{
		return 0;
	}

	uint32_t G_API_CC addReference()
	{
		++this->referenceCount;

		return this->referenceCount;
	}

	uint32_t G_API_CC releaseReference()
	{
		if(this->referenceCount > 0) {
			--this->referenceCount;
		}

		unsigned int refCount = this->referenceCount;

		if(this->referenceCount == 0) {
			delete this;
		}

		return refCount;
	}

private:
	STREAM & stream;
	uint32_t referenceCount;
	int indent;
};


bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item)
{
	GScopedInterface<IMetaAnnotation> annotation(item->getAnnotation(SerializationAnnotation));

	if(! annotation) {
	}
	else {
		GScopedInterface<IMetaAnnotationValue> annotationValue(annotation->getValue(SerializationAnnotationEnable));
		if(annotationValue) {
			if(! annotationValue->toBoolean()) {
				return false;
			}
		}
	}

	return true;
}

bool canSerializeObject(const GMetaArchiveConfig & config, IMetaClass * metaClass)
{
	return canSerializeItem(config, metaClass);
}

bool canSerializeField(const GMetaArchiveConfig & config, IMetaAccessible * accessible, IMetaClass * ownerClass)
{
	if(! accessible->canGet()) {
		return false;
	}
	if(! accessible->canSet()) {
		return false;
	}

	return canSerializeItem(config, accessible);
}

bool canSerializeBaseClass(const GMetaArchiveConfig & config, IMetaClass * baseClass, IMetaClass * metaClass)
{
	if(baseClass == NULL) {
		return false;
	}

	return true;
}

class GMetaArchivePointerSolver
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

bool GMetaArchivePointerSolver::hasPointer(void * p) const
{
	return this->pointerMap.find(p) != this->pointerMap.end();
}

uint32_t GMetaArchivePointerSolver::getArchiveID(void * p) const
{
	GASSERT(this->hasPointer(p));
	
	return this->pointerMap.find(p)->second;
}

void GMetaArchivePointerSolver::addPointer(void * p, uint32_t archiveID)
{
	GASSERT(! this->hasPointer(p));

	this->pointerMap.insert(pair<void *, uint32_t>(p, archiveID));
}

class GMetaArchiveWriter : public IMetaArchiveWriter
{
public:
	explicit GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer);

	// take care of customized serializer, take care of pointer resolve.
	void writeObjectValue(const char * name, void * instance, IMetaClass * metaClass);
	void writeObjectPointer(const char * name, void * instance, IMetaClass * metaClass);
	void writeField(const char * name, void * instance, IMetaAccessible * accessible);
	
	// ignore customized serializer, take care of pointer resolve.
	void defaultWriteObjectValue(const char * name, void * instance, IMetaClass * metaClass);
	void defaultWriteObjectPointer(const char * name, void * instance, IMetaClass * metaClass);
	void defaultWriteField(const char * name, void * instance, IMetaAccessible * accessible);

	// ignore customized serializer, ignore pointer resolve, take care of base classes
	void directWriteObject(const char * name, void * instance, IMetaClass * metaClass);
	void directWriteField(const char * name, void * instance, IMetaAccessible * accessible);

	// ignore customized serializer, ignore pointer resolve, ignore base classes, only write the object itself
	void directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass);

	void beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
protected:
	void writeObjectHelper(const char * name, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void defaultWriteObjectHelper(const char * name, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	
	void doWriteObject(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void doDefaultWriteObject(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void doDirectWriteObject(void * instance, IMetaClass * metaClass);
	void doDirectWriteObjectWithoutBase(void * instance, IMetaClass * metaClass);
	
	void doWriteField(const char * name, void * instance, IMetaAccessible * accessible);
	void doDefaultWriteField(const char * name, void * instance, IMetaAccessible * accessible);
	void doDirectWriteField(const char * name, void * instance, IMetaAccessible * accessible);
	
	uint32_t getNextArchiveID();

private:
	GMetaArchiveConfig config;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaWriter> writer;
	uint32_t currentArchiveID;
	GScopedPointer<GMetaArchivePointerSolver> pointerSolver;
};


struct IMetaObjectSerializer : public IObject
{
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, void * instance, IMetaClass * metaClass) = 0;
};


struct IMetaFieldSerializer : public IObject
{
	virtual void G_API_CC writeField(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaAccessible * accessible) = 0;
};


GMetaArchiveWriter::GMetaArchiveWriter(const GMetaArchiveConfig & config, IMetaService * service, IMetaWriter * writer)
	: config(config), service(service), writer(writer), currentArchiveID(0)
{
	this->service->addReference();
	this->writer->addReference();
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

	this->doWriteObject(instance, metaClass, pointerType);

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

	this->doDefaultWriteObject(instance, metaClass, pointerType);

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

	this->doDirectWriteObject(instance, metaClass);

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

	this->doDirectWriteObjectWithoutBase(instance, metaClass);

	this->endWriteObject(name, archiveID, instance, metaClass);
}

void GMetaArchiveWriter::doWriteObject(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	IMetaObjectSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->writeObject(this, instance, metaClass);
	}
	else {
		this->doDefaultWriteObject(instance, metaClass, pointerType);
	}
}

void GMetaArchiveWriter::doDefaultWriteObject(void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
{
	if(pointerType != aptIgnore) {
		if(true) {
			if(! this->pointerSolver) {
				this->pointerSolver.reset(new GMetaArchivePointerSolver);
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
		}
	}

	this->doDirectWriteObject(instance, metaClass);
}

void GMetaArchiveWriter::doDirectWriteObject(void * instance, IMetaClass * metaClass)
{
	GScopedInterface<IMetaClass> baseClass;
	uint32_t i;
	uint32_t count;

	count = metaClass->getBaseCount();
	for(i = 0; i < count; ++i) {
		baseClass.reset(metaClass->getBaseClass(i));
		
		if(canSerializeBaseClass(this->config, baseClass.get(), metaClass)) {
			this->doWriteObject(metaClass->castToBase(instance, i), baseClass.get(), aptIgnore);
		}
	}

	this->doDirectWriteObjectWithoutBase(instance, metaClass);
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(void * instance, IMetaClass * metaClass)
{
	if(instance == NULL) {
		return;
	}

	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	count = metaClass->getFieldCount();
	for(i = 0; i < count; ++i) {
		accessible.reset(metaClass->getFieldAt(i));

		if(canSerializeField(this->config, accessible.get(), metaClass)) {
			this->doWriteField(accessible->getName(), instance, accessible.get());
		}
	}

	count = metaClass->getPropertyCount();
	for(i = 0; i < count; ++i) {
		accessible.reset(metaClass->getPropertyAt(i));
		if(canSerializeField(this->config, accessible.get(), metaClass)) {
			this->doWriteField(accessible->getName(), instance, accessible.get());
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
	// todo: resolve pointers.

	this->doDirectWriteField(name, instance, accessible);
}

void GMetaArchiveWriter::doDirectWriteField(const char * name, void * instance, IMetaAccessible * accessible)
{
	GMetaType metaType = metaGetItemType(accessible);
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
		if(metaType.isFundamental()) {
			this->writer->writeFundamental(name, this->getNextArchiveID(), metaGetValue(accessible, instance));
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
				this->writer->writeNullPointer(name, this->getNextArchiveID());
			}
			else {
				if(vtGetBaseType(metaType.getVariantType()) == vtChar) {
					this->writer->writeString(name, this->getNextArchiveID(), static_cast<char *>(ptr));
				}
				else if(metaType.baseIsClass()) {
					if(metaType.getBaseName() != NULL) {
						GScopedInterface<IMetaClass> metaClass(this->service->findClassByName(metaType.getBaseName()));
						if(metaClass) {
							this->writeObjectPointer(name, ptr, metaClass.get());
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


using namespace cpgf;

#define CLASS TestClass
#define NAME_CLASS "TestClass"

class CLASS {
public:
	CLASS() : a(NULL), object(NULL) {
	}

	const char * a;
	int fieldInt;
	string fieldString;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;

	CLASS * object;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)

		._field("a", &CLASS::a)
			._annotation("serialize")
				._element("enable", false)

		._field("fieldInt", &CLASS::fieldInt)
		._field("fieldString", &CLASS::fieldString)
		._field("fieldReadonlyInt", &CLASS::fieldReadonlyInt, GMetaPolicyReadOnly())
		._field("fieldWriteonlyString", &CLASS::fieldWriteonlyString, GMetaPolicyWriteOnly())
		._field("object", &CLASS::object)
	;
}

void testSer()
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	GStreamMetaWriter<ostream> stream(cout);
	GMetaArchiveWriter archiveWriter(GMetaArchiveConfig(), service.get(), &stream);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS anotherObject;
	CLASS * pobj = &instance;
	instance.fieldInt = 38;
	instance.a = "abcdef";
	instance.object = &anotherObject;
	anotherObject.fieldInt = 98;

	field.reset(metaClass->getField("fieldInt"));
//	archiveWriter.writeField(field->getName(), pobj, field.get());
	
	archiveWriter.writeObjectValue("obj", pobj, metaClass.get());
}

