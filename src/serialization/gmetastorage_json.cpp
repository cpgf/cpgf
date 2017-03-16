#include "cpgf/serialization/gmetastorage_json.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"
#include "cpgf/gerrorcode.h"
#include "../pinclude/gmetaarchivecommonimpl.h"

#include "cpgf/thirdparty/jsoncpp/value.h"
#include "cpgf/thirdparty/jsoncpp/reader.h"
#include "cpgf/thirdparty/jsoncpp/writer.h"

#include <stack>
#include <sstream>

using namespace std;
using namespace Json;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

namespace {

typedef Json::Value JsonNodeType;


void checkNode(JsonNodeType * node, const char * /*nodeName*/)
{
	if(node == nullptr) {
		serializeError(Error_Serialization_InvalidStorage);
	}
}


} // unnamed namespace


class GMetaJsonStorageImplement
{
public:
	GMetaJsonStorageImplement();

	void initializeJson();

	void load(const char * jsonContent);
	void saveToStream(std::ostream & outputStream);

	JsonNodeType * getDataNode();
	JsonNodeType * getClassTypeNode();

private:
	JsonNodeType root;
	JsonNodeType * dataNode;
	JsonNodeType * classTypeNode;
};


GMetaJsonStorageImplement::GMetaJsonStorageImplement()
	: dataNode(nullptr), classTypeNode(nullptr)
{
}

void GMetaJsonStorageImplement::initializeJson()
{
	this->dataNode = &this->root[nameDataNode];
	this->classTypeNode = &this->root[nameClassTypesNode];
}

void GMetaJsonStorageImplement::load(const char * jsonContent)
{
	Reader reader;
	bool ok = reader.parse(jsonContent, this->root);
	if(! ok) {
		serializeError(Error_Serialization_InvalidStorage);
	}

	this->initializeJson();
}

void GMetaJsonStorageImplement::saveToStream(std::ostream & outputStream)
{
	outputStream << this->root;
}

JsonNodeType * GMetaJsonStorageImplement::getDataNode()
{
	return this->dataNode;
}

JsonNodeType * GMetaJsonStorageImplement::getClassTypeNode()
{
	return this->classTypeNode;
}


GMetaJsonStorage::GMetaJsonStorage()
{
}

GMetaJsonStorage::~GMetaJsonStorage()
{
}

void GMetaJsonStorage::load(const char * jsonContent) const
{
	this->implement.reset(new GMetaJsonStorageImplement);
	this->implement->load(jsonContent);
}

void GMetaJsonStorage::saveToStream(std::ostream & outputStream) const
{
	this->implement->saveToStream(outputStream);
}

GMetaJsonStorageImplement * GMetaJsonStorage::getImplement() const
{
	if(! this->implement) {
		this->implement.reset(new GMetaJsonStorageImplement);
	}
	return this->implement.get();
}



namespace serialization_internal {


class GJsonNodeNameTracker
{
private:
	typedef GStringMap<size_t, GStringMapReuseKey> MapType;

public:
	explicit GJsonNodeNameTracker(JsonNodeType * node);
	GJsonNodeNameTracker(JsonNodeType * node, bool nodeIsArray);
	void free();

	GJsonNodeNameTracker(const GJsonNodeNameTracker & other);
	GJsonNodeNameTracker & operator = (const GJsonNodeNameTracker & other);

	JsonNodeType * getNode() const;
	bool isArray() const;

	size_t getNameCount(const char * name) const;
	void addName(const char * name);
	const char * makeName(const char * name, bool addNameCount);
	const char * makeName(const char * name);

	size_t getArrayIndex() const;
	void addArrayIndex();

private:
	JsonNodeType * node;
	bool nodeIsArray;
	size_t arrayIndex;
	MapType * countMap;
	string nameBuffer;
};


GJsonNodeNameTracker::GJsonNodeNameTracker(JsonNodeType * node)
	: node(node), nodeIsArray(false), arrayIndex(0), countMap(nullptr)
{
}

GJsonNodeNameTracker::GJsonNodeNameTracker(JsonNodeType * node, bool nodeIsArray)
	: node(node), nodeIsArray(nodeIsArray), arrayIndex(0), countMap(nullptr)
{
}

void GJsonNodeNameTracker::free()
{
	delete this->countMap;
	this->countMap = nullptr;
}

GJsonNodeNameTracker::GJsonNodeNameTracker(const GJsonNodeNameTracker & other)
	: node(other.node), nodeIsArray(other.nodeIsArray), arrayIndex(other.arrayIndex), countMap(other.countMap)
{
}

GJsonNodeNameTracker & GJsonNodeNameTracker::operator = (const GJsonNodeNameTracker & other)
{
	this->node = other.node;
	this->nodeIsArray = other.nodeIsArray;
	this->arrayIndex = other.arrayIndex;
	this->countMap = other.countMap;

	return *this;
}

JsonNodeType * GJsonNodeNameTracker::getNode() const
{
	return this->node;
}

bool GJsonNodeNameTracker::isArray() const
{
	return this->nodeIsArray;
}

size_t GJsonNodeNameTracker::getNameCount(const char * name) const
{
	if(this->countMap != nullptr) {
		MapType::const_iterator it = this->countMap->find(name);
		if(it != this->countMap->end()) {
			return it->second;
		}
	}

	return 0;
}

void GJsonNodeNameTracker::addName(const char * name)
{
	if(this->countMap == nullptr) {
		this->countMap = new MapType;
	}

	this->countMap->set(name, this->getNameCount(name) + 1);
}

const char * GJsonNodeNameTracker::makeName(const char * name, bool addNameCount)
{
	const char * result = name;

	size_t count = this->getNameCount(name);

	if(count > 0) {
		stringstream stream;
		stream << name << (unsigned int)count;
		this->nameBuffer = stream.str();
		result = this->nameBuffer.c_str();
	}

	if(addNameCount) {
		this->addName(name);
	}

	return result;
}

const char * GJsonNodeNameTracker::makeName(const char * name)
{
	return this->makeName(name, true);
}

size_t GJsonNodeNameTracker::getArrayIndex() const
{
	return this->arrayIndex;
}

void GJsonNodeNameTracker::addArrayIndex()
{
	++this->arrayIndex;
}


class GJsonStorageWriter : public IMetaStorageWriter
{
	GMAKE_NONCOPYABLE(GJsonStorageWriter);

private:
	typedef stack<GJsonNodeNameTracker> ObjectNodeStack;

public:
	GJsonStorageWriter(JsonNodeType * dataNode, JsonNodeType * classTypeNode);
	virtual ~GJsonStorageWriter();

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
	
private:
	JsonNodeType * getCurrentNode() const;
	void pushNode(JsonNodeType * node);
	void pushNode(JsonNodeType * node, bool isArray);
	void popNode();
	void doPopNode();

	JsonNodeType & addNode(const char * name);

private:
	JsonNodeType * dataNode;
	JsonNodeType * classTypeNode;
	ObjectNodeStack nodeStack;
};

class GJsonStorageReader : public IMetaStorageReader
{
	GMAKE_NONCOPYABLE(GJsonStorageReader);

private:
	typedef stack<GJsonNodeNameTracker> ObjectNodeStack;

public:
	GJsonStorageReader(JsonNodeType * dataNode, JsonNodeType * classTypeNode);
	virtual ~GJsonStorageReader();

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

private:
	char * doReadString(JsonNodeType * node, IMemoryAllocator * allocator);

	JsonNodeType * getCurrentNode() const;
	void pushNode(JsonNodeType * node);
	void pushNode(JsonNodeType * node, bool isArray);
	void popNode();
	void doPopNode();
	
	JsonNodeType * getNode(const char * name, bool moveToNext);
	JsonNodeType * getNode(const char * name);

private:
	JsonNodeType * dataNode;
	JsonNodeType * classTypeNode;
	ObjectNodeStack nodeStack;
};


GJsonStorageWriter::GJsonStorageWriter(JsonNodeType * dataNode, JsonNodeType * classTypeNode)
	: dataNode(dataNode), classTypeNode(classTypeNode)
{
	this->nodeStack.push(GJsonNodeNameTracker(this->dataNode));
}

GJsonStorageWriter::~GJsonStorageWriter()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

void G_API_CC GJsonStorageWriter::writeFundamental(const char * name, const GVariantData * value)
{
	GVariant v(createVariantFromData(*value));

	GVariantType vt = v.getType();

	JsonNodeType & newNode = this->addNode(name);

	if(vtIsReal(vt)) {
		newNode = fromVariant<double>(v);
	}
	else {
		if(vt == GVariantType::vtSignedLongLong || vt == GVariantType::vtUnsignedLongLong) {
			newNode = fromVariant<double>(v);
		}
		else {
			if(vtIsUnsignedInteger(vt)) {
				newNode= fromVariant<unsigned long long>(v);
			}
			else {
				newNode = fromVariant<long long>(v);
			}
		}
	}
}

void G_API_CC GJsonStorageWriter::writeString(const char * name, uint32_t archiveID, const char * value)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameString] = value;
	newNode[nameArchiveID] = archiveID;
}

void G_API_CC GJsonStorageWriter::writeNullPointer(const char * name)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameNull] = 1;
}

void G_API_CC GJsonStorageWriter::beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameVersion] = version;
	newNode[nameArchiveID] = archiveID;
	newNode[nameClassTypeID] = classTypeID;
	
	JsonNodeType & contentNode = newNode[nameObject];
	this->pushNode(&contentNode);
}

void G_API_CC GJsonStorageWriter::endWriteObject(const char * /*name*/, uint32_t /*archiveID*/, uint32_t /*classTypeID*/, uint32_t /*version*/)
{
	this->popNode();
}

void G_API_CC GJsonStorageWriter::writeReferenceID(const char * name, uint32_t referenceArchiveID)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameReferenceID] = referenceArchiveID;
}

void G_API_CC GJsonStorageWriter::writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass)
{
	stringstream stream;
	stream << prefixClassType << classTypeID;

	JsonNodeType & currentNode = *(this->classTypeNode);
	currentNode[stream.str()] = metaClass->getQualifiedName();
}

void G_API_CC GJsonStorageWriter::beginWriteArray(const char * name, uint32_t length)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameLength] = length;
	
	JsonNodeType & contentNode = newNode[nameArray];
	this->pushNode(&contentNode, true);
}

void G_API_CC GJsonStorageWriter::endWriteArray(const char * /*name*/, uint32_t /*length*/)
{
	this->popNode();
}

JsonNodeType * GJsonStorageWriter::getCurrentNode() const
{
	return this->nodeStack.top().getNode();
}

void GJsonStorageWriter::pushNode(JsonNodeType * node)
{
	this->nodeStack.push(GJsonNodeNameTracker(node));
}

void GJsonStorageWriter::pushNode(JsonNodeType * node, bool isArray)
{
	this->nodeStack.push(GJsonNodeNameTracker(node, isArray));
}

void GJsonStorageWriter::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GJsonStorageWriter::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

JsonNodeType & GJsonStorageWriter::addNode(const char * name)
{
	JsonNodeType & currentNode = *(this->getCurrentNode());
	if(this->nodeStack.top().isArray()) {
		size_t index = this->nodeStack.top().getArrayIndex();
		this->nodeStack.top().addArrayIndex();
		return currentNode[(JsonNodeType::UInt)index];
	}
	else {
		return currentNode[this->nodeStack.top().makeName(name)];
	}
}


GJsonStorageReader::GJsonStorageReader(JsonNodeType * dataNode, JsonNodeType * classTypeNode)
	: dataNode(dataNode), classTypeNode(classTypeNode)
{
	this->nodeStack.push(GJsonNodeNameTracker(this->dataNode));
}

GJsonStorageReader::~GJsonStorageReader()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

uint32_t G_API_CC GJsonStorageReader::getArchiveType(const char * name)
{
	JsonNodeType * node = this->getNode(name, false);
	
	if(node == nullptr) {
		return matMissed;
	}

	if(node->isObject()) {
		if(node->isMember(nameReferenceID)) {
			return matReferenceObject;
		}
		else {
			if(node->isMember(nameString) || node->isMember(nameArray)) {
				return matCustomized;
			}
			else {
				if(node->isMember(nameNull)) {
					return matNull;
				}
				else {
					return matObject;
				}
			}
		}
	}

	if(node->isArray()) {
		return matCustomized;
	}

	if(node->isString()) {
		return matCustomized;
	}

	return matFundamental;
}

uint32_t G_API_CC GJsonStorageReader::getClassTypeID(const char * name)
{
	JsonNodeType * node = this->getNode(name, false);
	checkNode(node, name);

	return (*node)[nameClassTypeID].asUInt();
}

void G_API_CC GJsonStorageReader::readFundamental(const char * name, GVariantData * outValue)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);

	GVariant v;
	if(node->isDouble()) {
		v = node->asDouble();
	}
	else {
		if(node->isInt()) {
			v = node->asLargestInt();
		}
		else {
			v = node->asLargestUInt();
		}
	}
	*outValue = v.takeData();
}

char * G_API_CC GJsonStorageReader::readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);

	string s;

	*outArchiveID = archiveIDNone;

	if(node->isString()) {
		s = node->asString();
	}
	else {
		if(node->isObject() && node->isMember(nameString)) {
			s = (*node)[nameString].asString();
			*outArchiveID = (*node)[nameArchiveID].asUInt();
		}
	}

	char * result = static_cast<char *>(allocator->allocate(static_cast<uint32_t>(s.length() + 1)));
	strcpy(result, s.c_str());
	return result;
}

void * G_API_CC GJsonStorageReader::readNullPointer(const char * name)
{
	this->getNode(name);
	return nullptr;
}

uint32_t G_API_CC GJsonStorageReader::beginReadObject(const char * name, uint32_t * outVersion)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);
	
	*outVersion = (*node)[nameVersion].asUInt();

	uint32_t archiveID = (*node)[nameArchiveID].asUInt();
	
	JsonNodeType & contentNode = (*node)[nameObject];
	this->pushNode(&contentNode);

	return archiveID;
}

void G_API_CC GJsonStorageReader::endReadObject(const char * /*name*/, uint32_t /*version*/)
{
	this->popNode();
}

uint32_t G_API_CC GJsonStorageReader::readReferenceID(const char * name)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);
	
	return (*node)[nameReferenceID].asUInt();
}

IMetaClass * G_API_CC GJsonStorageReader::readMetaClassAndTypeID(IMetaService * /*service*/, uint32_t * /*outClassTypeID*/)
{
	return nullptr;
}

IMetaClass * G_API_CC GJsonStorageReader::readMetaClass(IMetaService * service, uint32_t classTypeID)
{
	stringstream stream;
	stream << prefixClassType << classTypeID;

	JsonNodeType & currentNode = *(this->classTypeNode);
	JsonNodeType & node = currentNode[stream.str()];
	
	if(! node.isNull()) {
		string s = node.asString();
		return service->findClassByName(s.c_str());
	}
	else {
		return nullptr;
	}
}

uint32_t G_API_CC GJsonStorageReader::beginReadArray(const char * name)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);

	if(node->isArray()) {
		this->pushNode(node, true);
		return node->size();
	}
	else {
		this->pushNode(&(*node)[nameArray], true);
		return (*node)[nameLength].asUInt();
	}
}

void G_API_CC GJsonStorageReader::endReadArray(const char * /*name*/)
{
	this->popNode();
}

JsonNodeType * GJsonStorageReader::getCurrentNode() const
{
	return this->nodeStack.top().getNode();
}

void GJsonStorageReader::pushNode(JsonNodeType * node)
{
	this->nodeStack.push(GJsonNodeNameTracker(node));
}

void GJsonStorageReader::pushNode(JsonNodeType * node, bool isArray)
{
	this->nodeStack.push(GJsonNodeNameTracker(node, isArray));
}

void GJsonStorageReader::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GJsonStorageReader::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

JsonNodeType * GJsonStorageReader::getNode(const char * name, bool moveToNext)
{
	JsonNodeType * currentNode = this->getCurrentNode();
	if(this->nodeStack.top().isArray()) {
		size_t index = this->nodeStack.top().getArrayIndex();
		if(index >= currentNode->size()) {
			return nullptr;
		}
		if(moveToNext) {
			this->nodeStack.top().addArrayIndex();
		}
		return &(*currentNode)[(JsonNodeType::UInt)index];
	}
	else {
		const char * n = this->nodeStack.top().makeName(name, false);
		if(currentNode->isMember(n)) {
			if(moveToNext) {
				this->nodeStack.top().addName(name);
			}
			return &(*currentNode)[n];
		}
		else {
			return nullptr;
		}
	}
}

JsonNodeType * GJsonStorageReader::getNode(const char * name)
{
	return this->getNode(name, true);
}

} // namespace serialization_internal


IMetaStorageWriter * createJsonStorageWriter(const GMetaJsonStorage & jsonStorage)
{
	jsonStorage.getImplement()->initializeJson();

	return new serialization_internal::GJsonStorageWriter(jsonStorage.getImplement()->getDataNode(), jsonStorage.getImplement()->getClassTypeNode());
}

IMetaStorageReader * createJsonStorageReader(const GMetaJsonStorage & jsonStorage)
{
	return new serialization_internal::GJsonStorageReader(jsonStorage.getImplement()->getDataNode(), jsonStorage.getImplement()->getClassTypeNode());
}


} // namespace cpgf

