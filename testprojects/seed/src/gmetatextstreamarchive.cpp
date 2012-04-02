#include "gmetatextstreamarchive.h"
#include "gmetaarchivereader.h"
#include "gmetaarchivewriter.h"


namespace cpgf {

namespace {

void checkType(PermanentType type, PermanentType expected)
{
	if(type != expected) {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

} // unnamed namespace


namespace serialization_internal {

class GTextStreamMetaWriter : public IMetaWriter
{
	GMAKE_NONCOPYABLE(GTextStreamMetaWriter);

private:
	enum DelimiterType {
		dtNone, dtSpace, dtNewline
	};

public:
	GTextStreamMetaWriter(std::ostream & stream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental);

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value);
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value);
	virtual void G_API_CC writeNullPointer(const char * name);
	
	virtual void G_API_CC beginWriteObject(GMetaArchiveWriterParam * param);
	virtual void G_API_CC endWriteObject(GMetaArchiveWriterParam * param);

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID);
	virtual void G_API_CC writeClassType(uint32_t archiveID, IMetaClass * metaClass);

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length);
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length);

protected:
	void writeDelimiter();
	void writeType(int permanentType);
	void doWriteString(const char * s);

private:
	std::ostream & stream;
	serialization_internal::FuncStreamWriteFundamental streamWriteFundamental;
	int * variantTypeMap;
	DelimiterType delimiter;
};

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

    friend class StreamMarker;

public:
	GTextStreamMetaReader(IMetaService * service, std::istream & stream, serialization_internal::FuncStreamReadFundamental streamReadFundamental);

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual uint32_t G_API_CC getArchiveType(const char * name);
	virtual uint32_t G_API_CC getClassType(const char * name);
	
	virtual void G_API_CC readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue);
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID);
	virtual void * G_API_CC readNullPointer(const char * name);

	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param);
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param);

	virtual uint32_t G_API_CC readReferenceID(const char * name);
	virtual IMetaClass * G_API_CC readClassType(const char * name, uint32_t * outArchiveID);

	virtual uint32_t G_API_CC beginReadArray(const char * name);
	virtual void G_API_CC endReadArray(const char * name);

protected:
	void skipDelimiter();

	PermanentType readType();

	char * doReadString(IMemoryAllocator * allocator);

private:
	GScopedInterface<IMetaService> service;
	std::istream & stream;
	serialization_internal::FuncStreamReadFundamental streamReadFundamental;
	int * variantTypeMap;
};


IMetaWriter * doCreateTextStreamMetaWriter(std::ostream & stream, FuncStreamWriteFundamental func)
{
	return new GTextStreamMetaWriter(stream, func);
}

IMetaReader * doCreateTextStreamMetaReader(IMetaService * service, std::istream & stream, FuncStreamReadFundamental func)
{
	return new GTextStreamMetaReader(service, stream, func);
}


GTextStreamMetaWriter::GTextStreamMetaWriter(std::ostream & stream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental)
	: stream(stream), streamWriteFundamental(streamWriteFundamental), variantTypeMap(defaultVariantTypeMap), delimiter(dtNone)
{
}

void G_API_CC GTextStreamMetaWriter::writeFundamental(const char * name, uint32_t archiveID, const GVariantData * value)
{
	(void)name;
	(void)archiveID;

	GVariant v(*value);

	this->writeType(getMappedTypeFromMap(this->variantTypeMap, v.getType()));

	this->writeDelimiter();

	this->streamWriteFundamental(this->stream, v);
}

void G_API_CC GTextStreamMetaWriter::writeString(const char * name, uint32_t archiveID, const char * value)
{
	(void)name;
	(void)archiveID;

	this->writeType(ptString);
	this->writeDelimiter();
	
	this->stream << static_cast<uint32_t>(archiveID);
	this->writeDelimiter();

	this->doWriteString(value);
}

void G_API_CC GTextStreamMetaWriter::writeNullPointer(const char * name)
{
	(void)name;

	this->writeType(ptNull);
}

void G_API_CC GTextStreamMetaWriter::beginWriteObject(GMetaArchiveWriterParam * param)
{
	if(this->delimiter != dtNone) {
		this->stream << '\n';
		this->delimiter = dtNone;
	}

	this->writeType(ptObject);
	this->writeDelimiter();
	this->stream << static_cast<uint32_t>(param->classTypeID);
	this->writeDelimiter();
	this->stream << static_cast<uint32_t>(param->archiveID);
}

void G_API_CC GTextStreamMetaWriter::endWriteObject(GMetaArchiveWriterParam * param)
{
	(void)param;

	this->delimiter = dtNewline;
}

void G_API_CC GTextStreamMetaWriter::writeReferenceID(const char * name, uint32_t archiveID, uint32_t referenceArchiveID)
{
	(void)name;
	(void)archiveID;

	this->writeType(ptReferenceID);
	
	this->writeDelimiter();
	
	this->stream << static_cast<uint32_t>(referenceArchiveID);
}

void G_API_CC GTextStreamMetaWriter::writeClassType(uint32_t archiveID, IMetaClass * metaClass)
{
	(void)archiveID;

	this->writeType(ptClassType);
	
	this->writeDelimiter();

	this->stream << static_cast<uint32_t>(archiveID);
	this->writeDelimiter();
	
	this->doWriteString(metaClass->getTypeName());
}

void G_API_CC GTextStreamMetaWriter::beginWriteArray(const char * name, uint32_t length)
{
	(void)name;
	
	if(this->delimiter != dtNone) {
		this->stream << '\n';
		this->delimiter = dtNone;
	}

	this->writeType(ptArray);
	
	this->writeDelimiter();

	this->stream << static_cast<uint32_t>(length);
}

void G_API_CC GTextStreamMetaWriter::endWriteArray(const char * name, uint32_t length)
{
	(void)name;
	(void)length;
}

void GTextStreamMetaWriter::writeDelimiter()
{
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

void GTextStreamMetaWriter::writeType(int permanentType)
{
	this->writeDelimiter();
	this->stream << static_cast<uint16_t>(permanentType);
}

void GTextStreamMetaWriter::doWriteString(const char * s)
{
	int len = static_cast<int>(strlen(s));

	this->stream << static_cast<uint32_t>(len);

	this->writeDelimiter();

	this->stream.write(s, len);
}



GTextStreamMetaReader::GTextStreamMetaReader(IMetaService * service, std::istream & stream, serialization_internal::FuncStreamReadFundamental streamReadFundamental)
	: service(service), stream(stream), streamReadFundamental(streamReadFundamental), variantTypeMap(defaultVariantTypeMap)
{
	if(this->service) {
		this->service->addReference();
	}
}

uint32_t G_API_CC GTextStreamMetaReader::getArchiveType(const char * name)
{
	(void)name;

	StreamMarker marker(this);
	PermanentType type = this->readType();
	switch(type) {
		case ptNull:
			return matNull;

		case ptObject:
			return matObject;

		case ptString:
			return matCustomized;

		case ptReferenceID:
			return matReferenceObject;

		case ptClassType:
			return matClassType;

		default:
			return matFundamental;
	}
}

uint32_t G_API_CC GTextStreamMetaReader::getClassType(const char * name)
{
	(void)name;

	StreamMarker marker(this);
	PermanentType type = this->readType();
	checkType(type, ptObject);

	uint32_t classTypeID;
	this->stream >> classTypeID;
	this->skipDelimiter();

	return classTypeID;
}

void G_API_CC GTextStreamMetaReader::readFundamental(const char * name, uint32_t * outArchiveID, GVariantData * outValue)
{
	(void)name;
	(void)outArchiveID;

	PermanentType type = this->readType();
	
	GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);
	if(vt == vtEmpty) {
		serializeError(Error_Serialization_TypeMismatch);
	}

	GVariant v(this->streamReadFundamental(this->stream, vt));
	this->skipDelimiter();
	*outValue = v.takeData();
}

char * G_API_CC GTextStreamMetaReader::readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID)
{
	(void)name;
	(void)outArchiveID;

	PermanentType type = this->readType();
	checkType(type, ptString);

	this->stream >> *outArchiveID;
	this->skipDelimiter();
	
	return this->doReadString(allocator);
}

void * G_API_CC GTextStreamMetaReader::readNullPointer(const char * name)
{
	(void)name;

	PermanentType type = this->readType();
	checkType(type, ptNull);
	
	return NULL;
}

uint32_t G_API_CC GTextStreamMetaReader::beginReadObject(GMetaArchiveReaderParam * param)
{
	(void)param;

	PermanentType type = this->readType();
	checkType(type, ptObject);

	uint32_t classTypeID;
	uint32_t archiveID;

	this->stream >> classTypeID;
	
	this->skipDelimiter();
	
	this->stream >> archiveID;
	
	this->skipDelimiter();

	return archiveID;
}

void G_API_CC GTextStreamMetaReader::endReadObject(GMetaArchiveReaderParam * param)
{
	(void)param;

}

uint32_t G_API_CC GTextStreamMetaReader::readReferenceID(const char * name)
{
	(void)name;

	PermanentType type = this->readType();
	checkType(type, ptReferenceID);

	uint32_t referenceArchiveID;
	this->stream >> referenceArchiveID;
	
	this->skipDelimiter();
	
	return referenceArchiveID;
}

IMetaClass * G_API_CC GTextStreamMetaReader::readClassType(const char * name, uint32_t * outArchiveID)
{
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

uint32_t G_API_CC GTextStreamMetaReader::beginReadArray(const char * name)
{
	(void)name;

	PermanentType type = this->readType();
	checkType(type, ptArray);

	uint16_t length;
	this->stream >> length;

	return length;
}

void G_API_CC GTextStreamMetaReader::endReadArray(const char * name)
{
	(void)name;
}

void GTextStreamMetaReader::skipDelimiter()
{
	if(! this->stream.eof()) {
		int c = this->stream.get();
		if(! this->stream.eof()) {
			if(c != ' ' && c != '\n') {
				GASSERT(false);
			}
		}
	}
}

PermanentType GTextStreamMetaReader::readType()
{
	GASSERT(! this->stream.eof());

	uint16_t type;
	this->stream >> type;
	this->skipDelimiter();
	return static_cast<PermanentType>(type);
}

char * GTextStreamMetaReader::doReadString(IMemoryAllocator * allocator)
{
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


} // namespace serialization_internal


} // namespace cpgf

