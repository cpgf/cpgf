#include "cpgf/serialization/gmetastorage_textstream.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivetypemap.h"
#include "cpgf/gerrorcode.h"

#include <stack>

namespace cpgf {

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

class GMetaArchiveTypeSession
{
private:
	typedef std::stack<int> StackType;

	enum { typeNone = -1, typeNoneSession = -2 };

public:
	void beginSession();
	void endSession();
	
	void beginNoneSession();
	void endNoneSession();

	bool isInSession() const;
	bool needType() const;
	void setType(int type);
	int getType() const;

private:
	mutable GScopedPointer<StackType> typeStack;
};

void GMetaArchiveTypeSession::beginSession()
{
	if(! this->typeStack) {
		this->typeStack.reset(new StackType);
	}

	this->typeStack->push(typeNone);
}

void GMetaArchiveTypeSession::endSession()
{
	this->typeStack->pop();
}

void GMetaArchiveTypeSession::beginNoneSession()
{
	if(! this->typeStack) {
		this->typeStack.reset(new StackType);
	}

	this->typeStack->push(typeNoneSession);
}

void GMetaArchiveTypeSession::endNoneSession()
{
	this->typeStack->pop();
}

bool GMetaArchiveTypeSession::isInSession() const
{
	return this->typeStack && ! this->typeStack->empty() && this->typeStack->top() != typeNoneSession;
}

bool GMetaArchiveTypeSession::needType() const
{
	return this->isInSession() && this->typeStack->top() == typeNone;
}

void GMetaArchiveTypeSession::setType(int type)
{
	GASSERT(type != typeNone);
	GASSERT(this->needType());

	this->typeStack->top() = type;
}

int GMetaArchiveTypeSession::getType() const
{
	GASSERT(this->isInSession());
	GASSERT(! this->needType());

	return this->typeStack->top();
}

namespace serialization_internal {

class GTextStreamMetaWriter : public IMetaStorageWriter
{
	GMAKE_NONCOPYABLE(GTextStreamMetaWriter);

private:
	enum DelimiterType {
		dtNone, dtSpace, dtNewline
	};

public:
	GTextStreamMetaWriter(std::ostream & outputStream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental);
	virtual ~GTextStreamMetaWriter();

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value);
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value);
	virtual void G_API_CC writeNullPointer(const char * name);
	
	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version);
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version);

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID);
	virtual void G_API_CC writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass);

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length);
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length);

protected:
	void setTypeInSession(int type);

	void writeDelimiter();
	void writeType(int permanentType);
	void doWriteString(const char * s);

private:
	std::ostream & outputStream;
	serialization_internal::FuncStreamWriteFundamental streamWriteFundamental;
	int * variantTypeMap;
	DelimiterType delimiter;
	GMetaArchiveTypeSession typeSession;
};

class GTextStreamMetaReader : public IMetaStorageReader
{
	GMAKE_NONCOPYABLE(GTextStreamMetaReader);

private:
	class StreamMarker {
	public:
		StreamMarker(GTextStreamMetaReader * reader) : reader(reader), mark((long)(reader->inputStream.tellg())) {
		}

		~StreamMarker() {
			this->reader->inputStream.clear();
			this->reader->inputStream.seekg(this->mark);
		}

	private:
		GTextStreamMetaReader * reader;
		long mark;
	};

    friend class StreamMarker;

public:
	GTextStreamMetaReader(std::istream & inputStream, serialization_internal::FuncStreamReadFundamental streamReadFundamental);
	virtual ~GTextStreamMetaReader();

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual uint32_t G_API_CC getArchiveType(const char * name);
	virtual uint32_t G_API_CC getClassTypeID(const char * name);
	
	virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue);
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID);
	virtual void * G_API_CC readNullPointer(const char * name);

	virtual uint32_t G_API_CC beginReadObject(const char * name, uint32_t * outVersion);
	virtual void G_API_CC endReadObject(const char * name, uint32_t version);

	virtual uint32_t G_API_CC readReferenceID(const char * name);
	virtual IMetaClass * G_API_CC readMetaClassAndTypeID(IMetaService * service, uint32_t * outClassTypeID);
	virtual IMetaClass * G_API_CC readMetaClass(IMetaService * service, uint32_t classTypeID);

	virtual uint32_t G_API_CC beginReadArray(const char * name);
	virtual void G_API_CC endReadArray(const char * name);

protected:
	PermanentType getTypeInSession(bool removeType);

	void skipDelimiter();

	PermanentType readType();

	char * doReadString(IMemoryAllocator * allocator);

private:
	std::istream & inputStream;
	serialization_internal::FuncStreamReadFundamental streamReadFundamental;
	int * variantTypeMap;
	GMetaArchiveTypeSession typeSession;
};


IMetaStorageWriter * doCreateTextStreamMetaWriter(std::ostream & outputStream, FuncStreamWriteFundamental func)
{
	return new GTextStreamMetaWriter(outputStream, func);
}

IMetaStorageReader * doCreateTextStreamMetaReader(std::istream & inputStream, FuncStreamReadFundamental func)
{
	return new GTextStreamMetaReader(inputStream, func);
}


GTextStreamMetaWriter::GTextStreamMetaWriter(std::ostream & outputStream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental)
	: outputStream(outputStream), streamWriteFundamental(streamWriteFundamental), variantTypeMap(defaultVariantTypeMap), delimiter(dtNone)
{
}

GTextStreamMetaWriter::~GTextStreamMetaWriter()
{
}

void GTextStreamMetaWriter::setTypeInSession(int type)
{
	bool needType = true;

	if(this->typeSession.isInSession()) {
		needType = this->typeSession.needType();
		if(needType) {
			this->typeSession.setType(type);
		}
	}

	if(needType) {
		this->writeType(type);
	}
}

void G_API_CC GTextStreamMetaWriter::writeFundamental(const char * /*name*/, const GVariantData * value)
{
	GVariant v(createVariantFromData(*value));

	int type = getMappedTypeFromMap(this->variantTypeMap, v.getType());
	
	this->setTypeInSession(type);
	this->writeDelimiter();

	this->streamWriteFundamental(this->outputStream, v);
}

void G_API_CC GTextStreamMetaWriter::writeString(const char * /*name*/, uint32_t archiveID, const char * value)
{
	this->writeType(ptString);
	this->writeDelimiter();
	
	this->outputStream << static_cast<uint32_t>(archiveID);
	this->writeDelimiter();

	this->doWriteString(value);
}

void G_API_CC GTextStreamMetaWriter::writeNullPointer(const char * /*name*/)
{
	this->writeType(ptNull);
}

void G_API_CC GTextStreamMetaWriter::beginWriteObject(const char * /*name*/, uint32_t archiveID, uint32_t classTypeID, uint32_t version)
{
	if(this->delimiter != dtNone) {
		this->outputStream << '\n';
		this->delimiter = dtNone;
	}

	this->writeType(ptObject);
	this->writeDelimiter();

	this->outputStream << static_cast<uint32_t>(version);
	this->writeDelimiter();

	this->outputStream << static_cast<uint32_t>(classTypeID);
	this->writeDelimiter();
	
	this->outputStream << static_cast<uint32_t>(archiveID);
	
	this->typeSession.beginNoneSession();
}

void G_API_CC GTextStreamMetaWriter::endWriteObject(const char * /*name*/, uint32_t /*archiveID*/, uint32_t /*classTypeID*/, uint32_t /*version*/)
{
	this->delimiter = dtNewline;

	this->typeSession.endNoneSession();
}

void G_API_CC GTextStreamMetaWriter::writeReferenceID(const char * /*name*/, uint32_t referenceArchiveID)
{
	this->writeType(ptReferenceID);
	this->writeDelimiter();
	
	this->outputStream << static_cast<uint32_t>(referenceArchiveID);
}

void G_API_CC GTextStreamMetaWriter::writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass)
{
	this->writeType(ptClassType);
	this->writeDelimiter();

	this->outputStream << static_cast<uint32_t>(classTypeID);
	this->writeDelimiter();
	
	this->doWriteString(metaClass->getQualifiedName());
}

void G_API_CC GTextStreamMetaWriter::beginWriteArray(const char * /*name*/, uint32_t length)
{
	if(this->delimiter != dtNone) {
		this->outputStream << '\n';
		this->delimiter = dtNone;
	}

	this->writeType(ptArray);
	this->writeDelimiter();

	this->outputStream << static_cast<uint32_t>(length);

	this->typeSession.beginSession();
}

void G_API_CC GTextStreamMetaWriter::endWriteArray(const char * /*name*/, uint32_t /*length*/)
{
	this->typeSession.endSession();
}

void GTextStreamMetaWriter::writeDelimiter()
{
	if(this->delimiter == dtNone) {
		this->delimiter = dtSpace;
	}
	else {
		if(this->delimiter == dtSpace) {
			this->outputStream << ' ';
		}
		else {
			this->outputStream << "\n";
			this->delimiter = dtSpace;
		}
	}
}

void GTextStreamMetaWriter::writeType(int permanentType)
{
	this->writeDelimiter();
	this->outputStream << static_cast<uint16_t>(permanentType);
}

void GTextStreamMetaWriter::doWriteString(const char * s)
{
	int len = static_cast<int>(strlen(s));

	this->outputStream << static_cast<uint32_t>(len);

	this->writeDelimiter();

	this->outputStream.write(s, len);
}



GTextStreamMetaReader::GTextStreamMetaReader(std::istream & inputStream, serialization_internal::FuncStreamReadFundamental streamReadFundamental)
	: inputStream(inputStream), streamReadFundamental(streamReadFundamental), variantTypeMap(defaultVariantTypeMap)
{
}

GTextStreamMetaReader::~GTextStreamMetaReader()
{
}

PermanentType GTextStreamMetaReader::getTypeInSession(bool removeType)
{
	PermanentType type = ptBool;

	bool needType = true;

	if(this->typeSession.isInSession()) {
		needType = this->typeSession.needType();
		if(! needType) {
			type = static_cast<PermanentType>(this->typeSession.getType());
		}
	}

	if(needType) {
		if(removeType) {
			type = this->readType();
			
			if(this->typeSession.isInSession()) {
				this->typeSession.setType(type);
			}
		}
		else {
			StreamMarker marker(this);
			type = this->readType();
		}
	}

	return type;
}

uint32_t G_API_CC GTextStreamMetaReader::getArchiveType(const char * /*name*/)
{
	PermanentType type = this->getTypeInSession(false);

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

		case ptArray:
			return matCustomized;

		default:
			if(! permanentTypeIsFundamental(type)) {
				serializeError(Error_Serialization_TypeMismatch);
			}

			return matFundamental;
	}
}

uint32_t G_API_CC GTextStreamMetaReader::getClassTypeID(const char * /*name*/)
{
	StreamMarker marker(this);
	PermanentType type = this->readType();
	serializeCheckType(type, ptObject);

	uint32_t version;
	uint32_t classTypeID;
	this->inputStream >> version;
	this->skipDelimiter();
	this->inputStream >> classTypeID;
	this->skipDelimiter();

	return classTypeID;
}

void G_API_CC GTextStreamMetaReader::readFundamental(const char * /*name*/, GVariantData * outValue)
{
	PermanentType type = this->getTypeInSession(true);

	GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);
	if(vt == GVariantType::vtEmpty) {
		serializeError(Error_Serialization_TypeMismatch);
	}

	GVariant v(this->streamReadFundamental(this->inputStream, vt));
	this->skipDelimiter();
	*outValue = v.takeData();
}

char * G_API_CC GTextStreamMetaReader::readString(const char * /*name*/, IMemoryAllocator * allocator, uint32_t * outArchiveID)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptString);

	this->inputStream >> *outArchiveID;
	this->skipDelimiter();
	
	return this->doReadString(allocator);
}

void * G_API_CC GTextStreamMetaReader::readNullPointer(const char * /*name*/)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptNull);
	
	return nullptr;
}

uint32_t G_API_CC GTextStreamMetaReader::beginReadObject(const char * /*name*/, uint32_t * outVersion)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptObject);

	uint32_t classTypeID = 0;
	uint32_t archiveID = 0;
	uint32_t version = 0;

	this->inputStream >> version;
	this->skipDelimiter();
	
	this->inputStream >> classTypeID;
	this->skipDelimiter();
	
	this->inputStream >> archiveID;
	this->skipDelimiter();

	this->typeSession.beginNoneSession();

	*outVersion = version;
	
	return archiveID;
}

void G_API_CC GTextStreamMetaReader::endReadObject(const char * /*name*/, uint32_t /*version*/)
{
	this->typeSession.endNoneSession();
}

uint32_t G_API_CC GTextStreamMetaReader::readReferenceID(const char * /*name*/)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptReferenceID);

	uint32_t referenceArchiveID;
	this->inputStream >> referenceArchiveID;
	
	this->skipDelimiter();
	
	return referenceArchiveID;
}

IMetaClass * G_API_CC GTextStreamMetaReader::readMetaClassAndTypeID(IMetaService * service, uint32_t * outClassTypeID)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptClassType);

	uint32_t id;
	this->inputStream >> id;
	this->skipDelimiter();
	*outClassTypeID = id;
	
	GScopedArray<char> classType(this->doReadString(nullptr));
	return service->findClassByName(classType.get());
}

IMetaClass * G_API_CC GTextStreamMetaReader::readMetaClass(IMetaService * /*service*/, uint32_t /*classTypeID*/)
{
	return nullptr;
}

uint32_t G_API_CC GTextStreamMetaReader::beginReadArray(const char * /*name*/)
{
	PermanentType type = this->readType();
	serializeCheckType(type, ptArray);

	uint32_t length;
	this->inputStream >> length;

	this->typeSession.beginSession();

	return length;
}

void G_API_CC GTextStreamMetaReader::endReadArray(const char * /*name*/)
{
	this->typeSession.endSession();
}

void GTextStreamMetaReader::skipDelimiter()
{
	if(! this->inputStream.eof()) {
		int c = this->inputStream.get();
		if(! this->inputStream.eof()) {
			if(c != ' ' && c != '\n') {
				GASSERT(false);
			}
		}
	}
}

PermanentType GTextStreamMetaReader::readType()
{
	GASSERT(! this->inputStream.eof());

	uint16_t type;
	this->inputStream >> type;
	this->skipDelimiter();
	return static_cast<PermanentType>(type);
}

char * GTextStreamMetaReader::doReadString(IMemoryAllocator * allocator)
{
	GASSERT(! this->inputStream.eof());

	uint32_t len;
	this->inputStream >> len;
	this->skipDelimiter();
	char * s;
	if(allocator == nullptr) {
		s = new char[len + 1];
	}
	else {
		s = static_cast<char *>(allocator->allocate(len + 1));
	}
	this->inputStream.read(s, len);
	s[len] = 0;
	
	this->skipDelimiter();

	return s;
}


} // namespace serialization_internal


IMetaStorageWriter * createTextStreamStorageWriter(std::ostream & outputStream)
{
	return serialization_internal::doCreateTextStreamMetaWriter(outputStream, &streamWriteFundamental<PromotedPermenentTypeMap>);
}

IMetaStorageReader * createTextStreamStorageReader(std::istream & inputStream)
{
	return serialization_internal::doCreateTextStreamMetaReader(inputStream, &streamReadFundamental<PromotedPermenentTypeMap>);
}


} // namespace cpgf

