#ifndef __GMETATEXTSTREAMARCHIVE_H
#define __GMETATEXTSTREAMARCHIVE_H


#include "gmetaarchivetypemap.h"
#include "gmetaarchivecommon.h"
#include "cpgf/gvartypedata.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gapiutil.h"


namespace cpgf {


inline void checkType(PermanentType type, PermanentType expected)
{
	if(type != expected) {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

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
			return 0;
	}
}

template <typename Stream, template<typename> class TypeMap = PermenentTypeMap>
class GTextStreamMetaWriter : public IMetaWriter
{
	GMAKE_NONCOPYABLE(GTextStreamMetaWriter);

private:
	enum DelimiterType {
		dtNone, dtSpace, dtNewline
	};

public:
	explicit GTextStreamMetaWriter(Stream & stream) : stream(stream), variantTypeMap(defaultVariantTypeMap), delimiter(dtNone) {
	}

	~GTextStreamMetaWriter() {
	}

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value) {
		(void)name;
		(void)archiveID;

		GVariant v(*value);

		this->writeType(getMappedTypeFromMap(this->variantTypeMap, v.getType()));

		this->writeDelimiter();

		streamWriteFundamental<Stream, TypeMap>(this->stream, v);
	}

	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) {
		(void)name;
		(void)archiveID;

		this->writeType(ptString);
		
		this->writeDelimiter();

		this->doWriteString(value);
	}

	virtual void G_API_CC writeNullPointer(const char * name) {
		(void)name;

		this->writeType(ptNull);
	}

	virtual void G_API_CC beginWriteObject(const GMetaArchiveObjectInformation * objectInformation) {
		if(this->delimiter != dtNone) {
			this->stream << '\n';
			this->delimiter = dtNone;
		}

		this->writeType(ptObject);
		this->writeDelimiter();
		this->stream << static_cast<uint32_t>(objectInformation->classTypeID);
		this->writeDelimiter();
		this->stream << static_cast<uint32_t>(objectInformation->archiveID);
	}

	virtual void G_API_CC endWriteObject(const GMetaArchiveObjectInformation * objectInformation) {
		(void)objectInformation;

		this->delimiter = dtNewline;
	}

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID) {
		(void)name;
		(void)archiveID;

		this->writeType(ptReferenceID);
		
		this->writeDelimiter();
		
		this->stream << static_cast<uint32_t>(referenceArchiveID);
	}

	virtual void G_API_CC writeClassType(const char * name, uint32_t archiveID, IMetaClass * metaClass) {
		(void)name;
		(void)archiveID;

		this->writeType(ptClassType);
		
		this->writeDelimiter();

		this->stream << static_cast<uint32_t>(archiveID);
		this->writeDelimiter();
		
		this->doWriteString(metaClass->getTypeName());
	}

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) {
		(void)name;
		
		this->writeDelimiter();
		this->stream << static_cast<uint32_t>(length);
	}

	virtual void G_API_CC endWriteArray(const char * name, uint32_t length) {
		(void)name;
		(void)length;
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
				this->delimiter = dtSpace;
			}
		}
	}

	void writeType(int permanentType) {
		this->writeDelimiter();
		this->stream << static_cast<uint16_t>(permanentType);
	}

	void doWriteString(const char * s) {
		int len = static_cast<int>(strlen(s));

		this->stream << static_cast<uint32_t>(len);

		this->writeDelimiter();

		this->stream.write(s, len);
	}

private:
	Stream & stream;
	int * variantTypeMap;
	DelimiterType delimiter;
};

template <typename Stream, template<typename> class TypeMap = PermenentTypeMap>
class GTextStreamMetaReader : public IMetaReader
{
	GMAKE_NONCOPYABLE(GTextStreamMetaReader);

private:
	class StreamMarker {
	public:
		StreamMarker(GTextStreamMetaReader * reader) : reader(reader), mark(reader->stream.tellg()) {
		}

		~StreamMarker() {
			this->reader->stream.clear();
			this->reader->stream.seekg(this->mark);
		}

	private:
		GTextStreamMetaReader * reader;
		long mark;
	};

public:
	GTextStreamMetaReader(IMetaService * service, Stream & stream)
		: service(service), stream(stream), variantTypeMap(defaultVariantTypeMap)
	{
		if(this->service) {
			this->service->addReference();
		}
	}

	~GTextStreamMetaReader() {
	}

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual uint32_t G_API_CC getArchiveType(const char * name) {
		(void)name;

		StreamMarker marker(this);
		PermanentType type = this->readType();
		switch(type) {
			case ptNull:
				return matNull;

			case ptObject:
				return matObject;

			case ptString:
				return matString;

			case ptReferenceID:
				return matReferenceObject;

			case ptClassType:
				return matClassType;

			default:
				return matFundamental;
		}
	}

	virtual uint32_t G_API_CC getClassType(const char * name) {
		(void)name;

		StreamMarker marker(this);
		PermanentType type = this->readType();
		checkType(type, ptObject);

		uint32_t classTypeID;
		this->stream >> classTypeID;
		this->skipDelimiter();

		return classTypeID;
	}

	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue) {
		(void)name;
		(void)outArchiveID;

		PermanentType type = this->readType();
		
		GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);
		if(vt == vtEmpty) {
			serializeError(Error_Serialization_TypeMismatch);
		}

		GVariant v(streamReadFundamental<Stream, TypeMap>(this->stream, vt));
		this->skipDelimiter();
		*outValue = v.takeData();
	}

	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) {
		(void)name;
		(void)outArchiveID;

		PermanentType type = this->readType();
		checkType(type, ptString);

		return this->doReadString(allocator);
	}

	virtual void * G_API_CC readNullPointer(const char * name) {
		(void)name;

		PermanentType type = this->readType();
		checkType(type, ptNull);
		
		return NULL;
	}

	virtual void G_API_CC beginReadObject(GMetaArchiveObjectInformation * objectInformation) {
		PermanentType type = this->readType();
		checkType(type, ptObject);

		this->stream >> objectInformation->classTypeID;
		
		this->skipDelimiter();
		
		this->stream >> objectInformation->archiveID;
		
		this->skipDelimiter();
	}

	virtual void G_API_CC endReadObject(const GMetaArchiveObjectInformation * objectInformation) {
		(void)objectInformation;

	}

	virtual uint32_t G_API_CC readReferenceID(const char * name) {
		(void)name;

		PermanentType type = this->readType();
		checkType(type, ptReferenceID);

		uint32_t referenceArchiveID;
		this->stream >> referenceArchiveID;
		
		this->skipDelimiter();
		
		return referenceArchiveID;
	}

	virtual IMetaClass * G_API_CC readClassType(const char * name, uint32_t * outArchiveID) {
		(void)name;
		(void)outArchiveID;

		PermanentType type = this->readType();
		checkType(type, ptClassType);

		uint32_t id;
		this->stream >> id;
		this->skipDelimiter();
		*outArchiveID = id;
		
		GScopedArray<char> classType(this->doReadString(NULL));
		return this->service->findClassByName(classType.get());
	}

	virtual uint32_t G_API_CC beginReadArray(const char * name) {
		(void)name;

		uint16_t length;
		this->stream >> length;
		this->skipDelimiter();

		return length;
	}

	virtual void G_API_CC endReadArray(const char * name) {
		(void)name;
	}

protected:
	void skipDelimiter() {
		if(! this->stream.eof()) {
			char c = this->stream.get();
			if(c != ' ' && c != '\n') {
				GASSERT(false);
			}
		}
	}

	PermanentType readType() {
		GASSERT(! this->stream.eof());

		uint16_t type;
		this->stream >> type;
		this->skipDelimiter();
		return static_cast<PermanentType>(type);
	}

	char * doReadString(IMemoryAllocator * allocator) {
		GASSERT(! this->stream.eof());

		uint32_t len;
		this->stream >> len;
		this->skipDelimiter();
		char * s;
		if(allocator == NULL) {
			s = new char[len + 1];
		}
		else {
			s = static_cast<char *>(allocator->allocate(len + 1));
		}
		this->stream.read(s, len);
		s[len] = 0;
		
		this->skipDelimiter();

		return s;
	}

private:
	GScopedInterface<IMetaService> service;
	Stream & stream;
	int * variantTypeMap;
};



} // namespace cpgf


#endif

