#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gflags.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "pinclude/gapiimpl.h"

#include <string.h>

#include <map>


#include <iostream>
#include <sstream>
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
	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value) = 0;
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
	virtual void G_API_CC writeNullPointer(const char * name) = 0;

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) = 0;

//	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
//	virtual void G_API_CC endWriteArray(const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
};

struct IMetaReader : public IObject
{
	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue) = 0;
	virtual char * G_API_CC readString(const char * name, uint32_t * outArchiveID) = 0;
	virtual void G_API_CC freeString(char * s) = 0;
	virtual void * G_API_CC readNullPointer(const char * name) = 0;

	virtual void G_API_CC beginReadObject(const char * name, void * instance, IMetaClass * metaClass, uint32_t * outArchiveID) = 0;
	virtual void G_API_CC endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;

	virtual uint32_t G_API_CC readReferenceID(const char * name, uint32_t archiveID) = 0;

};


namespace {

enum PermanentType {
	ptBool = 1,
	ptChar = 2,
	ptWchar = 3,
	ptSignedChar = 4,
	ptUnsignedChar = 5,
	ptSignedShort = 6,
	ptUnsignedShort = 7,
	ptSignedInt = 8,
	ptUnsignedInt = 9,
	ptSignedLong = 10,
	ptUnsignedLong = 11,
	ptSignedLongLong = 12,
	ptUnsignedLongLong = 13,
	ptFloat = 14,
	ptDouble = 15,
	ptLongDouble = 16,
	ptObject = 17,
	ptString = 18,
	ptWideString = 19,

	ptNull = 50, // for null pointer
	ptReferenceID = 51, // for one pointer reference to another

};

int defaultVariantTypeMap[] = {
	vtBool, ptBool,
	vtChar, ptChar,
	vtWchar, ptWchar,
	vtSignedChar, ptSignedChar,
	vtUnsignedChar, ptUnsignedChar,
	vtSignedShort, ptSignedShort,
	vtUnsignedShort, ptUnsignedShort,
	vtSignedInt, ptSignedInt,
	vtUnsignedInt, ptUnsignedInt,
	vtSignedLong, ptSignedLong,
	vtUnsignedLong, ptUnsignedLong,
	vtSignedLongLong, ptSignedLongLong,
	vtUnsignedLongLong, ptUnsignedLongLong,
	vtFloat, ptFloat,
	vtDouble, ptDouble,
	vtLongDouble, ptLongDouble,
	vtObject, ptObject,
	vtString, ptString,
	vtWideString, ptWideString,

	vtEmpty,
};

}

GVariantType getVariantTypeFromMap(int * variantTypeMap, int mappedType)
{
	while(*variantTypeMap != vtEmpty) {
		if(*(variantTypeMap + 1) == mappedType) {
			return static_cast<GVariantType>(*variantTypeMap);
		}
		variantTypeMap += 2;
	}

	return vtEmpty;
}

int getMappedTypeFromMap(int * variantTypeMap, GVariantType vt)
{
	while(*variantTypeMap != vtEmpty) {
		if(*variantTypeMap == vt) {
			return *(variantTypeMap + 1);
		}
		variantTypeMap += 2;
	}

	return -1;
}

template <typename T>
struct PermenentTypeMap
{
	typedef T Result;
};

template <>
struct PermenentTypeMap <wchar_t>
{
	typedef short Result;
};

template <typename Stream, template<typename> class TypeMap >
void streamWriteFundamental(Stream & stream, const GVariant & value)
{
	switch(value.getType()) {
		case vtBool:
			stream << fromVariant<typename TypeMap<bool>::Result>(value);
			break;

		case vtChar:
			stream << fromVariant<typename TypeMap<char>::Result>(value);
			break;

		case vtWchar:
			stream << fromVariant<typename TypeMap<wchar_t>::Result>(value);
			break;

		case vtSignedChar:
			stream << fromVariant<typename TypeMap<signed char>::Result>(value);
			break;

		case vtUnsignedChar:
			stream << fromVariant<typename TypeMap<unsigned char>::Result>(value);
			break;

		case vtSignedShort:
			stream << fromVariant<typename TypeMap<signed short>::Result>(value);
			break;

		case vtUnsignedShort:
			stream << fromVariant<typename TypeMap<unsigned short>::Result>(value);
			break;

		case vtSignedInt:
			stream << fromVariant<typename TypeMap<signed int>::Result>(value);
			break;

		case vtUnsignedInt:
			stream << fromVariant<typename TypeMap<unsigned int>::Result>(value);
			break;

		case vtSignedLong:
			stream << fromVariant<typename TypeMap<signed long>::Result>(value);
			break;

		case vtUnsignedLong:
			stream << fromVariant<typename TypeMap<unsigned long>::Result>(value);
			break;

		case vtSignedLongLong:
			stream << fromVariant<typename TypeMap<signed long long>::Result>(value);
			break;

		case vtUnsignedLongLong:
			stream << fromVariant<typename TypeMap<unsigned long long>::Result>(value);
			break;

		case vtFloat:
			stream << fromVariant<typename TypeMap<float>::Result>(value);
			break;

		case vtDouble:
			stream << fromVariant<typename TypeMap<double>::Result>(value);
			break;

		case vtLongDouble:
			stream << fromVariant<typename TypeMap<long double>::Result>(value);
			break;

		default:
			break;
	}
}

template <typename Stream, template<typename> class TypeMap >
GVariant streamReadFundamental(Stream & stream, GVariantType vt)
{
	switch(vt) {
		case vtBool: {
			typename TypeMap<bool>::Result value;
			stream >> value;
			return value;
		}

		case vtChar: {
			typename TypeMap<char>::Result value;
			stream >> value;
			return value;
		}

		case vtWchar: {
			typename TypeMap<wchar_t>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedChar: {
			typename TypeMap<signed char>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedChar: {
			typename TypeMap<unsigned char>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedShort: {
			typename TypeMap<signed short>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedShort: {
			typename TypeMap<unsigned short>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedInt: {
			typename TypeMap<signed int>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedInt: {
			typename TypeMap<unsigned int>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedLong: {
			typename TypeMap<signed long>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedLong: {
			typename TypeMap<unsigned long>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedLongLong: {
			typename TypeMap<signed long long>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedLongLong: {
			typename TypeMap<unsigned long long>::Result value;
			stream >> value;
			return value;
		}

		case vtFloat: {
			typename TypeMap<float>::Result value;
			stream >> value;
			return value;
		}

		case vtDouble: {
			typename TypeMap<double>::Result value;
			stream >> value;
			return value;
		}

		case vtLongDouble: {
			typename TypeMap<long double>::Result value;
			stream >> value;
			return value;
		}

		default:
			break;
	}
}

template <typename Stream, template<typename> class TypeMap = PermenentTypeMap>
class GTextStreamMetaWriter : public ImplObject, public IMetaWriter
{
private:
	enum DelimiterType {
		dtNone, dtSpace, dtNewline
	};

public:
	explicit GTextStreamMetaWriter(Stream & stream) : stream(stream), variantTypeMap(defaultVariantTypeMap), delimiter(dtNone) {
	}

protected:
	IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value) {
		GVariant v(*value);

		this->writeType(getMappedTypeFromMap(this->variantTypeMap, v.getType()));

		this->writeDelimiter();

		streamWriteFundamental<Stream, TypeMap>(this->stream, v);
	}

	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) {
		this->writeType(ptString);
		
		this->writeDelimiter();

		size_t len = strlen(value);

		this->stream << static_cast<uint32_t>(len);

		this->writeDelimiter();

		this->stream.write(value, len);
	}

	virtual void G_API_CC writeNullPointer(const char * name) {
		this->writeType(ptNull);
	}

	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)name;
		(void)instance;
		(void)metaClass;

		if(this->delimiter != dtNone) {
			this->stream << '\n';
			this->delimiter = dtNone;
		}

		this->writeType(ptObject);
		this->writeDelimiter();
		this->stream << static_cast<uint32_t>(archiveID);
	}

	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)name;
		(void)instance;
		(void)metaClass;

		this->delimiter == dtNewline;
	}

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) {
		this->writeType(ptReferenceID);
		
		this->writeDelimiter();
		
		this->stream << static_cast<uint32_t>(referenceArchiveID);
	}

protected:
	void writeDelimiter() {
		if(this->delimiter == dtNone) {
			this->delimiter = dtSpace;
		}
		else {
			if(this->delimiter == dtSpace) {
				this->stream << ' ';
			}
			else {
				this->stream << "\n";
				this->delimiter = dtNone;
			}
		}
	}

	void writeType(int permanentType) {
		this->writeDelimiter();
		this->stream << static_cast<uint16_t>(permanentType);
	}

private:
	Stream & stream;
	int * variantTypeMap;
	DelimiterType delimiter;
};

template <typename Stream, template<typename> class TypeMap = PermenentTypeMap>
class GTextStreamMetaReader : public ImplObject, public IMetaReader
{
public:
	explicit GTextStreamMetaReader(Stream & stream) : stream(stream), variantTypeMap(defaultVariantTypeMap) {
	}

protected:
	IMPL_OBJECT

	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue) {
		PermanentType type = this->readType();
		GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);

		GVariant v(streamReadFundamental<Stream, TypeMap>(this->stream, vt));
		*outValue = v.takeData();
	}

	virtual char * G_API_CC readString(const char * name, uint32_t * outArchiveID) {
		PermanentType type = this->readType();
		uint32_t len;
		this->stream >> len;
		this->skipDelimiter();
		char * s = new char[len + 1];
		this->stream.read(s, len);
		s[len] = 0;
		return s;
	}

	virtual void G_API_CC freeString(char * s) {
		delete s;
	}

	virtual void * G_API_CC readNullPointer(const char * name) {
		PermanentType type = this->readType();
		if(type == ptNull) {
		}

		return NULL;
	}

	virtual void G_API_CC beginReadObject(const char * name, void * instance, IMetaClass * metaClass, uint32_t * outArchiveID) {
		(void)name;
		(void)instance;
		(void)metaClass;

		PermanentType type = this->readType();
		this->stream >> *outArchiveID;
	}

	virtual void G_API_CC endReadObject(const char * name, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)name;
		(void)instance;
		(void)metaClass;

	}

	virtual uint32_t G_API_CC readReferenceID(const char * name, uint32_t archiveID) {
		PermanentType type = this->readType();
		uint32_t referenceArchiveID;
		this->stream >> referenceArchiveID;
		return referenceArchiveID;
	}

protected:
	void skipDelimiter() {
		this->stream.get();
	}

	PermanentType readType() {
		uint16_t type;
		this->stream >> type;
		this->skipDelimiter();
		return static_cast<PermanentType>(type);
	}

private:
	Stream & stream;
	int * variantTypeMap;
};


bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item)
{
	(void)config;

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
	(void) ownerClass;

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
	(void)config;
	(void)metaClass;

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
	
	void doWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void doDefaultWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType);
	void doDirectWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	void doDirectWriteObjectWithoutBase(uint32_t archiveID, void * instance, IMetaClass * metaClass);
	
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
		serializer->writeObject(this, instance, metaClass);
	}
	else {
		this->doDefaultWriteObject(archiveID, instance, metaClass, pointerType);
	}
}

void GMetaArchiveWriter::doDefaultWriteObject(uint32_t archiveID, void * instance, IMetaClass * metaClass, GMetaArchivePointerType pointerType)
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

class TestClassSerializeBase {
public:
	TestClassSerializeBase() : baseA(5.8) {
	}

	double baseA;
};

class TestClassSerialize : public TestClassSerializeBase {
public:
	TestClassSerialize() : a(NULL), object(NULL) {
	}

	const char * a;
	int fieldInt;
	string fieldString;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;

	TestClassSerialize * object;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestClassSerializeBase>
		::define("TestClassSerializeBase")

		._field("baseA", &TestClassSerializeBase::baseA)
	;

	GDefineMetaClass<TestClassSerialize, TestClassSerializeBase>
		::define("TestClassSerialize")

		._field("a", &TestClassSerialize::a)
			._annotation("serialize")
				._element("enable", false)

		._field("fieldInt", &TestClassSerialize::fieldInt)
		._field("fieldString", &TestClassSerialize::fieldString)
		._field("fieldReadonlyInt", &TestClassSerialize::fieldReadonlyInt, GMetaPolicyReadOnly())
		._field("fieldWriteonlyString", &TestClassSerialize::fieldWriteonlyString, GMetaPolicyWriteOnly())
		._field("object", &TestClassSerialize::object)
	;
}

void testSer()
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GMetaArchiveWriter archiveWriter(GMetaArchiveConfig(), service.get(), &outputStream);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestClassSerialize"));

	GScopedInterface<IMetaField> field;

	TestClassSerialize instance;
	TestClassSerialize anotherObject;
	anotherObject.fieldInt = 98;
	TestClassSerialize * pobj = &instance;
	instance.fieldInt = 38;
	instance.a = "abcdef";
	instance.object = &anotherObject;
	anotherObject.fieldInt = 98;

	field.reset(metaClass->getField("fieldInt"));
//	archiveWriter.writeField(field->getName(), pobj, field.get());
	
	archiveWriter.writeObjectValue("obj", pobj, metaClass.get());

	cout << stream.str().c_str();

	stream.seekg(0);
	
	GTextStreamMetaReader<stringstream> inputStream(stream);
}

