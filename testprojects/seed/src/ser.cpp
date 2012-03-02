#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <iostream>
using namespace std;


/*
IMetaWriter -- the primary writer to write data to different output, such as binary, text, etc.
Meta archive writer -- write meta object/value to archive writer. It does the pointers resolve. It doesn't use meta item's archive class.
Meta archive -- archiving. It uses meta item's archive class.

function writeObject(object)
{
	if(object is not allowed to serialize) {
		return;
	}

	if(object has customized serializer) {
		customizedSerializer.writeObject();
	}
	else {
		processObject(object)
	}
}

function processObject(object)
{
	resolveObjectPointer(object);
	if(object has been written) {
		writeObjectReferenceTag();
		return;
	}

	foreach(base class in [ base classes ]) {
		if(base class is allowed to serialize) {
			writeObject(base class)
		}
	}

	beginObject(object);

	foreach(accessible in [ fields and properties ]) {
		if(accessible is allowed to serialize) {
			writeAccessible(accessible)
		}
	}

	endObject(object);
}

function beginObject(object)
{
}

function endObject(object)
{
}

function resolveObjectPointer(object)
{
	objectAddress = getObjectAddress(object);

	if(objectAddress is not in addressMap) {
		addressMap[objectAddress] = 1;
	}
	else {
		++addressMap[objectAddress];
	}
}

function writeAccessible(accessible)
{
	if(accessible has customized serializer in annotation) {
		customizedSerializer.writeAccessible();
	}
	else {
		processAccessible(accessible)
	}
}

function processAccessible(accessible)
{
	pointers = pointer dimension of accessible
	if(pointers == 0) {
		if(accessible is fundamental) {
			metaWriter.writeFundamental(accessible)
		}
		else if(accessible is class) {
			writeObject(accessible)
		}
		else {
			error
		}
	}
}

function customizedSerializer.writeObject(archiveWriter, object)
{
	if(need to call default serializer) {
		archiveWriter.processObject(object);
	}
	else {
		archiveWriter.beginObject(object);
		
		archiveWriter.writeAccessible(for each accessible);

		archiveWriter.endObject(object);
	}
}

function customizedSerializer.writeAccessible(archiveWriter, accessible)
{
	if(need to call default serializer) {
		archiveWriter.processAccessible(accessible);
	}
	else {
		// process accessible here
	}
}

*/

namespace cpgf {

struct IMetaArchiveWriter {};

struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(const char * name, const GVariant & value) = 0;
	virtual void G_API_CC writeString(const char * name, const char * value) = 0;

	virtual void G_API_CC beginWriteObject(const char * name, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC endWriteObject(const char * name, void * instance, IMetaClass * metaClass) = 0;

//	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
//	virtual void G_API_CC endWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
};

template <typename STREAM>
class GStreamMetaWriter : public IMetaWriter
{
public:
	explicit GStreamMetaWriter(STREAM & stream) : stream(stream), referenceCount(1) {
	}

protected:
	virtual void G_API_CC writeFundamental(const char * name, const GVariant & value) {
		this->stream << name << " -- ";

		switch(value.getType()) {
			case vtSignedInt:
				this->stream << fromVariant<int>(value);
				break;

			default:
				break;
		}
		
		this->stream << endl;
	}

	virtual void G_API_CC writeString(const char * name, const char * value) {
	}

	virtual void G_API_CC beginWriteObject(const char * name, void * instance, IMetaClass * metaClass) {
	}

	virtual void G_API_CC endWriteObject(const char * name, void * instance, IMetaClass * metaClass) {
	}

protected:
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
};


class GMetaArchiveWriter : public IMetaArchiveWriter
{
public:
	explicit GMetaArchiveWriter(IMetaService * service, IMetaWriter * writer);

	// take care of customized serializer, take care of pointer resolve.
	void writeObject(const char * name, void * instance, IMetaClass * metaClass);
	void writeMetaItem(const char * name, void * instance, IMetaItem * metaItem);
	
	// ignore customized serializer, take care of pointer resolve.
	void defaultWriteObject(const char * name, void * instance, IMetaClass * metaClass);
	void defaultWriteMetaItem(const char * name, void * instance, IMetaItem * metaItem);

	// ignore customized serializer, ignore pointer resolve, take care of base classes
	void directWriteObject(const char * name, void * instance, IMetaClass * metaClass);

	// ignore customized serializer, ignore pointer resolve, ignore base classes, only write the object itself
	void directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass);

	void beginWriteObject(const char * name, void * instance, IMetaClass * metaClass);
	void endWriteObject(const char * name, void * instance, IMetaClass * metaClass);
	
	bool canWriteObject(IMetaClass * metaClass);
	bool canWriteMetaItem(IMetaItem * metaItem, IMetaClass * ownerClass);
	bool canWriteBaseClass(IMetaClass * baseClass, IMetaClass * metaClass);

protected:
	void doWriteObject(void * instance, IMetaClass * metaClass);
	void doWriteMetaItem(const char * name, void * instance, IMetaItem * metaItem);
	
	void doDefaultWriteObject(void * instance, IMetaClass * metaClass);
	void doDirectWriteObject(void * instance, IMetaClass * metaClass);
	void doDirectWriteObjectWithoutBase(void * instance, IMetaClass * metaClass);
	
	void doWriteMetaAccessible(const char * name, void * instance, IMetaAccessible * accessible);
	void doWriteValue(const char * name, const GVariant & value, const GMetaType & metaType);

private:
	GScopedInterface<IMetaService> service;
	GScopedInterface<IMetaWriter> writer;
};


struct IMetaSerializer : public IObject
{
	void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, void * instance, IMetaClass * metaClass);
	void G_API_CC writeMetaItem(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaItem * metaItem);
};



GMetaArchiveWriter::GMetaArchiveWriter(IMetaService * service, IMetaWriter * writer)
	: service(service), writer(writer)
{
	this->service->addReference();
	this->writer->addReference();
}

void GMetaArchiveWriter::writeObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->beginWriteObject(name, instance, metaClass);

	this->doWriteObject(instance, metaClass);

	this->endWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::writeMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	this->doWriteMetaItem(name, instance, metaItem);
}

void GMetaArchiveWriter::defaultWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->beginWriteObject(name, instance, metaClass);

	this->doDefaultWriteObject(instance, metaClass);

	this->endWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::doDefaultWriteObject(void * instance, IMetaClass * metaClass)
{
	// todo: resolve field pointers.

	this->doDirectWriteObject(instance, metaClass);
}

void GMetaArchiveWriter::defaultWriteMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	switch(static_cast<GMetaCategory>(metaItem->getCategory())) {
		case mcatField:
		case mcatProperty:
			this->doWriteMetaAccessible(name, instance, static_cast<IMetaAccessible *>(metaItem));
			break;

		default:
			break;
	}
}

void GMetaArchiveWriter::directWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->beginWriteObject(name, instance, metaClass);

	this->doDirectWriteObject(instance, metaClass);

	this->endWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::doDirectWriteObject(void * instance, IMetaClass * metaClass)
{
	GScopedInterface<IMetaClass> baseClass;
	uint32_t i;
	uint32_t count;

	count = metaClass->getBaseCount();
	for(i = 0; i < count; ++i) {
		baseClass.reset(metaClass->getBaseClass(i));
		
		if(this->canWriteBaseClass(baseClass.get(), metaClass)) {
			this->doWriteObject(metaClass->castToBase(instance, i), baseClass.get());
		}
	}

	this->doDirectWriteObjectWithoutBase(instance, metaClass);
}

void GMetaArchiveWriter::directWriteObjectWithoutBase(const char * name, void * instance, IMetaClass * metaClass)
{
	this->beginWriteObject(name, instance, metaClass);

	this->doDirectWriteObjectWithoutBase(instance, metaClass);

	this->endWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::doDirectWriteObjectWithoutBase(void * instance, IMetaClass * metaClass)
{
	GScopedInterface<IMetaAccessible> accessible;
	uint32_t i;
	uint32_t count;

	count = metaClass->getFieldCount();
	for(i = 0; i < count; ++i) {
		accessible.reset(metaClass->getFieldAt(i));
		
		if(this->canWriteMetaItem(accessible.get(), metaClass)) {
			this->doWriteMetaItem(accessible->getName(), instance, accessible.get());
		}
	}

	count = metaClass->getPropertyCount();
	for(i = 0; i < count; ++i) {
		accessible.reset(metaClass->getPropertyAt(i));
		if(this->canWriteMetaItem(accessible.get(), metaClass)) {
			this->doWriteMetaItem(accessible->getName(), instance, accessible.get());
		}
	}
}

void GMetaArchiveWriter::beginWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->writer->beginWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::endWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->writer->endWriteObject(name, instance, metaClass);
}

bool GMetaArchiveWriter::canWriteObject(IMetaClass * metaClass)
{
	return true;
}

bool GMetaArchiveWriter::canWriteMetaItem(IMetaItem * metaItem, IMetaClass * ownerClass)
{
	switch(static_cast<GMetaCategory>(metaItem->getCategory())) {
		case mcatField:
		case mcatProperty: {
			IMetaAccessible * accessible = static_cast<IMetaAccessible *>(metaItem);
			if(! accessible->canGet()) {
				return false;
			}
			if(! accessible->canSet()) {
				return false;
			}
		}
			break;

		default:
			break;
	}

	return true;
}

bool GMetaArchiveWriter::canWriteBaseClass(IMetaClass * baseClass, IMetaClass * metaClass)
{
	if(baseClass == NULL) {
		return false;
	}

	return true;
}

void GMetaArchiveWriter::doWriteObject(void * instance, IMetaClass * metaClass)
{
	IMetaSerializer * serializer = NULL;
	if(serializer != NULL) {
		serializer->writeObject(this, instance, metaClass);
	}
	else {
		this->doDefaultWriteObject(instance, metaClass);
	}
}

void GMetaArchiveWriter::doWriteMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	this->defaultWriteMetaItem(name, instance, metaItem);
}

void GMetaArchiveWriter::doWriteMetaAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doWriteValue(name, metaGetValue(accessible, instance), metaGetItemType(accessible));
}

void GMetaArchiveWriter::doWriteValue(const char * name, const GVariant & value, const GMetaType & metaType)
{
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
		if(metaType.isFundamental()) {
			this->writer->writeFundamental(name, value);
		}
		else if(metaType.baseIsClass()) {
		}
		else {
			// error
		}
	}
	else if(pointers == 1) {
	}
}


} // namespace cpgf


using namespace cpgf;

#define CLASS TestClass
#define NAME_CLASS "TestClass"

class CLASS_DATA
{
public:
	CLASS_DATA() : s(""), i(0) {
	}
	
	CLASS_DATA(const std::string s, int i) : s(s), i(i) {
	}

	explicit CLASS_DATA(const std::string s) : s(s), i(0) {
	}

	explicit CLASS_DATA(int i) : s(""), i(i) {
	}

	void reset() {
		s = "";
		i = 0;
	}
	
	bool operator == (const CLASS_DATA & other) const /**/ {
		return this->s == other.s && this->i == other.i;
	}
	
	std::string s;
	int i;
};


class CLASS {
public:
	int fieldInt;
	string fieldString;
	
	CLASS_DATA fieldData;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;
	CLASS_DATA fieldNoncopyableData;
}; // class CLASS


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)

		._field("fieldInt", &CLASS::fieldInt)
		._field("fieldString", &CLASS::fieldString)
		._field("fieldData", &CLASS::fieldData)
		._field("fieldReadonlyInt", &CLASS::fieldReadonlyInt, GMetaPolicyReadOnly())
		._field("fieldWriteonlyString", &CLASS::fieldWriteonlyString, GMetaPolicyWriteOnly())
		._field("fieldNoncopyableData", &CLASS::fieldNoncopyableData, GMetaPolicyNoncopyable())
	;
}

void testSer()
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	GStreamMetaWriter<ostream> stream(cout);
	GMetaArchiveWriter archiveWriter(service.get(), &stream);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS * pobj = &instance;
	instance.fieldInt = 38;

	field.reset(metaClass->getField("fieldInt"));
	archiveWriter.writeMetaItem(field->getName(), pobj, field.get());
	
	archiveWriter.writeObject("obj", pobj, metaClass.get());
}

