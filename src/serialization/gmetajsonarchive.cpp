#include "cpgf/serialization/gmetajsonarchive.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"
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


namespace {

typedef Json::Value JsonNodeType;


void checkNode(JsonNodeType * node, const char * /*nodeName*/)
{
	if(node == NULL) {
		serializeError(Error_Serialization_InvalidStorage);
	}
}


} // unnamed namespace


class GMetaJsonArchiveImplement
{
public:
	GMetaJsonArchiveImplement();

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


GMetaJsonArchiveImplement::GMetaJsonArchiveImplement()
	: dataNode(NULL), classTypeNode(NULL)
{
}

void GMetaJsonArchiveImplement::initializeJson()
{
	this->dataNode = &this->root[nameDataNode];
	this->classTypeNode = &this->root[nameClassTypesNode];
}

void GMetaJsonArchiveImplement::load(const char * jsonContent)
{
	Reader reader;
	bool ok = reader.parse(jsonContent, this->root);
	if(! ok) {
		serializeError(Error_Serialization_InvalidStorage);
	}

	this->initializeJson();
}

void GMetaJsonArchiveImplement::saveToStream(std::ostream & outputStream)
{
	outputStream << this->root;
}

JsonNodeType * GMetaJsonArchiveImplement::getDataNode()
{
	return this->dataNode;
}

JsonNodeType * GMetaJsonArchiveImplement::getClassTypeNode()
{
	return this->classTypeNode;
}


GMetaJsonArchive::GMetaJsonArchive()
{
}

GMetaJsonArchive::~GMetaJsonArchive()
{
}

void GMetaJsonArchive::load(const char * jsonContent) const
{
	this->implement.reset(new GMetaJsonArchiveImplement);
	this->implement->load(jsonContent);
}

void GMetaJsonArchive::saveToStream(std::ostream & outputStream) const
{
	this->implement->saveToStream(outputStream);
}

GMetaJsonArchiveImplement * GMetaJsonArchive::getImplement() const
{
	if(! this->implement) {
		this->implement.reset(new GMetaJsonArchiveImplement);
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
	: node(node), nodeIsArray(false), arrayIndex(0), countMap(NULL)
{
}

GJsonNodeNameTracker::GJsonNodeNameTracker(JsonNodeType * node, bool nodeIsArray)
	: node(node), nodeIsArray(nodeIsArray), arrayIndex(0), countMap(NULL)
{
}

void GJsonNodeNameTracker::free()
{
	delete this->countMap;
	this->countMap = NULL;
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
	if(this->countMap != NULL) {
		MapType::const_iterator it = this->countMap->find(name);
		if(it != this->countMap->end()) {
			return it->second;
		}
	}

	return 0;
}

void GJsonNodeNameTracker::addName(const char * name)
{
	if(this->countMap == NULL) {
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
		stream << name << count;
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


class GJsonMetaWriter : public IMetaWriter
{
	GMAKE_NONCOPYABLE(GJsonMetaWriter);

private:
	typedef stack<GJsonNodeNameTracker> ObjectNodeStack;

public:
	GJsonMetaWriter(JsonNodeType * dataNode, JsonNodeType * classTypeNode);
	~GJsonMetaWriter();

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value);
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value);
	virtual void G_API_CC writeNullPointer(const char * name);
	
	virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version);
	virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version);

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID);
	virtual void G_API_CC writeClassType(uint32_t classTypeID, IMetaClass * metaClass);

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

class GJsonMetaReader : public IMetaReader
{
	GMAKE_NONCOPYABLE(GJsonMetaReader);

private:
	typedef stack<GJsonNodeNameTracker> ObjectNodeStack;

public:
	GJsonMetaReader(IMetaService * service, JsonNodeType * dataNode, JsonNodeType * classTypeNode);
	~GJsonMetaReader();

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
	virtual IMetaClass * G_API_CC readClassAndTypeID(uint32_t * outClassTypeID);
	virtual IMetaClass * G_API_CC readClass(uint32_t classTypeID);

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
	GScopedInterface<IMetaService> service;
	JsonNodeType * dataNode;
	JsonNodeType * classTypeNode;
	ObjectNodeStack nodeStack;
};


GJsonMetaWriter::GJsonMetaWriter(JsonNodeType * dataNode, JsonNodeType * classTypeNode)
	: dataNode(dataNode), classTypeNode(classTypeNode)
{
	this->nodeStack.push(GJsonNodeNameTracker(this->dataNode));
}

GJsonMetaWriter::~GJsonMetaWriter()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

void G_API_CC GJsonMetaWriter::writeFundamental(const char * name, const GVariantData * value)
{
	GVariant v(*value);

	GVariantType vt = v.getType();

	JsonNodeType & newNode = this->addNode(name);

	if(vtIsReal(vt)) {
		newNode = fromVariant<double>(v);
	}
	else {
		if(vt == vtSignedLongLong || vt == vtUnsignedLongLong) {
			newNode = fromVariant<double>(v);
		}
		else {
			if(vtIsUnsignedInteger(vt)) {
				newNode= fromVariant<unsigned int>(v);
			}
			else {
				newNode = fromVariant<int>(v);
			}
		}
	}
}

void G_API_CC GJsonMetaWriter::writeString(const char * name, uint32_t archiveID, const char * value)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameString] = value;
	newNode[nameArchiveID] = archiveID;
}

void G_API_CC GJsonMetaWriter::writeNullPointer(const char * name)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameNull] = 1;
}

void G_API_CC GJsonMetaWriter::beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameVersion] = version;
	newNode[nameArchiveID] = archiveID;
	newNode[nameClassTypeID] = classTypeID;
	
	JsonNodeType & contentNode = newNode[nameObject];
	this->pushNode(&contentNode);
}

void G_API_CC GJsonMetaWriter::endWriteObject(const char * /*name*/, uint32_t /*archiveID*/, uint32_t /*classTypeID*/, uint32_t /*version*/)
{
	this->popNode();
}

void G_API_CC GJsonMetaWriter::writeReferenceID(const char * name, uint32_t referenceArchiveID)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameReferenceID] = referenceArchiveID;
}

void G_API_CC GJsonMetaWriter::writeClassType(uint32_t classTypeID, IMetaClass * metaClass)
{
	stringstream stream;
	stream << prefixClassType << classTypeID;

	JsonNodeType & currentNode = *(this->classTypeNode);
	currentNode[stream.str()] = metaClass->getTypeName();
}

void G_API_CC GJsonMetaWriter::beginWriteArray(const char * name, uint32_t length)
{
	JsonNodeType & newNode = this->addNode(name);
	newNode[nameLength] = length;
	
	JsonNodeType & contentNode = newNode[nameArray];
	this->pushNode(&contentNode, true);
}

void G_API_CC GJsonMetaWriter::endWriteArray(const char * /*name*/, uint32_t /*length*/)
{
	this->popNode();
}

JsonNodeType * GJsonMetaWriter::getCurrentNode() const
{
	return this->nodeStack.top().getNode();
}

void GJsonMetaWriter::pushNode(JsonNodeType * node)
{
	this->nodeStack.push(GJsonNodeNameTracker(node));
}

void GJsonMetaWriter::pushNode(JsonNodeType * node, bool isArray)
{
	this->nodeStack.push(GJsonNodeNameTracker(node, isArray));
}

void GJsonMetaWriter::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GJsonMetaWriter::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

JsonNodeType & GJsonMetaWriter::addNode(const char * name)
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


GJsonMetaReader::GJsonMetaReader(IMetaService * service, JsonNodeType * dataNode, JsonNodeType * classTypeNode)
	: service(service), dataNode(dataNode), classTypeNode(classTypeNode)
{
	this->nodeStack.push(GJsonNodeNameTracker(this->dataNode));

	if(this->service) {
		this->service->addReference();
	}
}

GJsonMetaReader::~GJsonMetaReader()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

uint32_t G_API_CC GJsonMetaReader::getArchiveType(const char * name)
{
	JsonNodeType * node = this->getNode(name, false);
	
	if(node == NULL) {
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

uint32_t G_API_CC GJsonMetaReader::getClassTypeID(const char * name)
{
	JsonNodeType * node = this->getNode(name, false);
	checkNode(node, name);

	return (*node)[nameClassTypeID].asUInt();
}

void G_API_CC GJsonMetaReader::readFundamental(const char * name, GVariantData * outValue)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);

	GVariant v;
	if(node->isDouble()) {
		v = node->asDouble();
	}
	else {
		if(node->isInt()) {
			v = node->asInt();
		}
		else {
			v = node->asUInt();
		}
	}
	*outValue = v.takeData();
}

char * G_API_CC GJsonMetaReader::readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID)
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

void * G_API_CC GJsonMetaReader::readNullPointer(const char * name)
{
	this->getNode(name);
	return NULL;
}

uint32_t G_API_CC GJsonMetaReader::beginReadObject(const char * name, uint32_t * outVersion)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);
	
	*outVersion = (*node)[nameVersion].asUInt();

	uint32_t archiveID = (*node)[nameArchiveID].asUInt();
	
	JsonNodeType & contentNode = (*node)[nameObject];
	this->pushNode(&contentNode);

	return archiveID;
}

void G_API_CC GJsonMetaReader::endReadObject(const char * /*name*/, uint32_t /*version*/)
{
	this->popNode();
}

uint32_t G_API_CC GJsonMetaReader::readReferenceID(const char * name)
{
	JsonNodeType * node = this->getNode(name);
	checkNode(node, name);
	
	return (*node)[nameReferenceID].asUInt();
}

IMetaClass * G_API_CC GJsonMetaReader::readClassAndTypeID(uint32_t * /*outClassTypeID*/)
{
	return NULL;
}

IMetaClass * G_API_CC GJsonMetaReader::readClass(uint32_t classTypeID)
{
	stringstream stream;
	stream << prefixClassType << classTypeID;

	JsonNodeType & currentNode = *(this->classTypeNode);
	JsonNodeType & node = currentNode[stream.str()];
	
	if(! node.isNull()) {
		string s = node.asString();
		return this->service->findClassByName(s.c_str());
	}
	else {
		return NULL;
	}
}

uint32_t G_API_CC GJsonMetaReader::beginReadArray(const char * name)
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

void G_API_CC GJsonMetaReader::endReadArray(const char * /*name*/)
{
	this->popNode();
}

JsonNodeType * GJsonMetaReader::getCurrentNode() const
{
	return this->nodeStack.top().getNode();
}

void GJsonMetaReader::pushNode(JsonNodeType * node)
{
	this->nodeStack.push(GJsonNodeNameTracker(node));
}

void GJsonMetaReader::pushNode(JsonNodeType * node, bool isArray)
{
	this->nodeStack.push(GJsonNodeNameTracker(node, isArray));
}

void GJsonMetaReader::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GJsonMetaReader::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

JsonNodeType * GJsonMetaReader::getNode(const char * name, bool moveToNext)
{
	JsonNodeType * currentNode = this->getCurrentNode();
	if(this->nodeStack.top().isArray()) {
		size_t index = this->nodeStack.top().getArrayIndex();
		if(index >= currentNode->size()) {
			return NULL;
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
			return NULL;
		}
	}
}

JsonNodeType * GJsonMetaReader::getNode(const char * name)
{
	return this->getNode(name, true);
}

} // namespace serialization_internal


IMetaWriter * createJsonMetaWriter(const GMetaJsonArchive & jsonArchive)
{
	jsonArchive.getImplement()->initializeJson();

	return new serialization_internal::GJsonMetaWriter(jsonArchive.getImplement()->getDataNode(), jsonArchive.getImplement()->getClassTypeNode());
}

IMetaReader * createJsonMetaReader(IMetaService * service, const GMetaJsonArchive & jsonArchive)
{
	return new serialization_internal::GJsonMetaReader(service, jsonArchive.getImplement()->getDataNode(), jsonArchive.getImplement()->getClassTypeNode());
}


} // namespace cpgf

