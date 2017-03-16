#include "cpgf/serialization/gmetastorage_xml.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"
#include "cpgf/serialization/gmetaarchivetypemap.h"
#include "cpgf/gerrorcode.h"
#include "../pinclude/gmetaarchivecommonimpl.h"

#include "cpgf/thirdparty/rapidxml/rapidxml.hpp"
#include "cpgf/thirdparty/rapidxml/rapidxml_print.hpp"

#include <string.h>

#include <stack>
#include <sstream>

using namespace std;
using namespace rapidxml;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

namespace {

typedef xml_node<> XmlNodeType;	
typedef xml_attribute<> XmlAttributeType;


void checkNode(void * node, const char * /*nodeName*/)
{
	if(node == nullptr) {
		serializeError(Error_Serialization_InvalidStorage);
	}
}

XmlNodeType * getSiblingAt(XmlNodeType * node, const char * name, size_t index)
{
	while(index > 0 && node != nullptr) {
		--index;
		node = node->next_sibling(name);
	}

	return node;
}


} // unnamed namespace


class GMetaXmlStorageImplement
{
public:
	GMetaXmlStorageImplement();

	void initializeXml();

	void loadIntrusive(char * xmlContent);
	void load(const char * xmlContent);
	void saveToStream(std::ostream & outputStream);

	XmlNodeType * getDataNode() const;
	XmlNodeType * getClassTypeNode() const;
	xml_document<> * getXml() const;

private:
	void doLoad(char * xmlContent);

private:
	xml_document<> xml;
	string intrusiveXmlContent;
	XmlNodeType * dataNode;
	XmlNodeType * classTypeNode;
};

GMetaXmlStorageImplement::GMetaXmlStorageImplement()
	: dataNode(nullptr), classTypeNode(nullptr)
{
}

void GMetaXmlStorageImplement::initializeXml()
{
	XmlNodeType * rootNode = this->xml.first_node(nameRootNode);
	if(rootNode != 0) {
		this->doLoad(nullptr);
	}
	else {
		rootNode = this->xml.allocate_node(node_element, this->xml.allocate_string(nameRootNode));
		this->xml.append_node(rootNode);

		this->dataNode = this->xml.allocate_node(node_element, this->xml.allocate_string(nameDataNode));
		rootNode->append_node(this->dataNode);
	
		this->classTypeNode = this->xml.allocate_node(node_element, this->xml.allocate_string(nameClassTypesNode));
		rootNode->append_node(this->classTypeNode);
	}
}

void GMetaXmlStorageImplement::loadIntrusive(char * xmlContent)
{
	this->intrusiveXmlContent = "";
	this->doLoad(xmlContent);
}

void GMetaXmlStorageImplement::load(const char * xmlContent)
{
	this->intrusiveXmlContent = xmlContent;
	this->doLoad(&this->intrusiveXmlContent[0]);
}

void GMetaXmlStorageImplement::saveToStream(std::ostream & outputStream)
{
	outputStream << this->xml;
}

XmlNodeType * GMetaXmlStorageImplement::getDataNode() const
{
	GASSERT(this->dataNode != nullptr);

	return this->dataNode;
}

XmlNodeType * GMetaXmlStorageImplement::getClassTypeNode() const
{
	GASSERT(this->classTypeNode != nullptr);

	return this->classTypeNode;
}

xml_document<> * GMetaXmlStorageImplement::getXml() const
{
	return const_cast<xml_document<> *>(&this->xml);
}

void GMetaXmlStorageImplement::doLoad(char * xmlContent)
{
	if(xmlContent != nullptr) {
		this->xml.parse<0>(xmlContent);
	}

	XmlNodeType * rootNode = this->xml.first_node(nameRootNode);
	checkNode(rootNode, "root");

	this->dataNode = rootNode->first_node(nameDataNode);
	checkNode(dataNode, "dataNode");
	
	this->classTypeNode = rootNode->first_node(nameClassTypesNode);
	checkNode(this->classTypeNode, "classTypeNode");
}


GMetaXmlStorage::GMetaXmlStorage()
	: implement()
{
}

GMetaXmlStorage::~GMetaXmlStorage()
{
}

void GMetaXmlStorage::loadIntrusive(char * xmlContent) const
{
	this->implement.reset(new GMetaXmlStorageImplement);
	this->implement->loadIntrusive(xmlContent);
}

void GMetaXmlStorage::load(const char * xmlContent) const
{
	this->implement.reset(new GMetaXmlStorageImplement);
	this->implement->load(xmlContent);
}

void GMetaXmlStorage::saveToStream(std::ostream & outputStream) const
{
	this->implement->saveToStream(outputStream);
}

GMetaXmlStorageImplement * GMetaXmlStorage::getImplement() const
{
	if(! this->implement) {
		this->implement.reset(new GMetaXmlStorageImplement);
	}
	return this->implement.get();
}


namespace serialization_internal {


class GXmlReaderNodeNameTracker
{
private:
	typedef GStringMap<size_t, GStringMapReuseKey> MapType;

public:
	explicit GXmlReaderNodeNameTracker(XmlNodeType * node);
	void free();

	GXmlReaderNodeNameTracker(const GXmlReaderNodeNameTracker & other);
	GXmlReaderNodeNameTracker & operator = (const GXmlReaderNodeNameTracker & other);

	XmlNodeType * getNode() const;

	size_t getIndex(const char * name) const;
	void addIndex(const char * name);

private:
	XmlNodeType * node;
	MapType * countMap;
};


GXmlReaderNodeNameTracker::GXmlReaderNodeNameTracker(XmlNodeType * node)
	: node(node), countMap(nullptr)
{
}

void GXmlReaderNodeNameTracker::free()
{
	delete this->countMap;
	this->countMap = nullptr;
}

GXmlReaderNodeNameTracker::GXmlReaderNodeNameTracker(const GXmlReaderNodeNameTracker & other)
	: node(other.node), countMap(other.countMap)
{
}

GXmlReaderNodeNameTracker & GXmlReaderNodeNameTracker::operator = (const GXmlReaderNodeNameTracker & other)
{
	this->node = other.node;
	this->countMap = other.countMap;

	return *this;
}

XmlNodeType * GXmlReaderNodeNameTracker::getNode() const
{
	return this->node;
}

size_t GXmlReaderNodeNameTracker::getIndex(const char * name) const
{
	if(this->countMap != nullptr) {
		MapType::const_iterator it = this->countMap->find(name);
		if(it != this->countMap->end()) {
			return it->second;
		}
	}

	return 0;
}

void GXmlReaderNodeNameTracker::addIndex(const char * name)
{
	if(this->countMap == nullptr) {
		this->countMap = new MapType;
	}

	this->countMap->set(name, this->getIndex(name) + 1);
}


class GXmlStorageWriter : public IMetaStorageWriter
{
	GMAKE_NONCOPYABLE(GXmlStorageWriter);

private:
	typedef stack<XmlNodeType *> ObjectNodeStack;

public:
	GXmlStorageWriter(xml_document<> * xml, XmlNodeType * dataNode, XmlNodeType * classTypeNode,
		serialization_internal::FuncStreamWriteFundamental streamWriteFundamental);
	virtual ~GXmlStorageWriter();

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
	void clearTextStream();
	const char * newString(const char * s);

	XmlNodeType * getCurrentNode() const;
	void pushNode(XmlNodeType * node);
	void popNode();

	XmlNodeType * addNode(const char * name, const char * value);
	XmlNodeType * addNode(const char * name);
	XmlAttributeType * addAttribute(XmlNodeType * node, const char * name, const char * value);
	XmlAttributeType * addAttribute(XmlNodeType * node, const char * name);
	void addIntAttribute(XmlNodeType * node, const char * name, long long value);
	void addType(XmlNodeType * node, int permanentType);
	
private:
	xml_document<> * xml;
	XmlNodeType * dataNode;
	XmlNodeType * classTypeNode;
	serialization_internal::FuncStreamWriteFundamental streamWriteFundamental;
	int * variantTypeMap;
	ObjectNodeStack nodeStack;
	stringstream textStream;
};

class GXmlStorageReader : public IMetaStorageReader
{
	GMAKE_NONCOPYABLE(GXmlStorageReader);

private:
	typedef stack<GXmlReaderNodeNameTracker> ObjectNodeStack;

public:
	GXmlStorageReader(XmlNodeType * dataNode, XmlNodeType * classTypeNode,
		serialization_internal::FuncStreamReadFundamental streamReadFundamental);
	virtual ~GXmlStorageReader();

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
	char * doReadString(XmlNodeType * node, IMemoryAllocator * allocator);

	void setTextStream(const char * text);
	
	XmlNodeType * getCurrentNode() const;
	void pushNode(XmlNodeType * node);
	void popNode();
	void doPopNode();

	XmlNodeType * getNode(const char * name);
	void gotoNextNode(XmlNodeType * currentNode, const char * name);

	XmlAttributeType * getAttribute(XmlNodeType * node, const char * name);
	uint32_t getIntAttribute(XmlNodeType * node, const char * name);
	PermanentType readType(XmlNodeType * node);

private:
	XmlNodeType * dataNode;
	XmlNodeType * classTypeNode;
	serialization_internal::FuncStreamReadFundamental streamReadFundamental;
	int * variantTypeMap;
	ObjectNodeStack nodeStack;
	stringstream textStream;
};


IMetaStorageWriter * doCreateXmlMetaWriter(const GMetaXmlStorage & xmlStorage, FuncStreamWriteFundamental func)
{
	xmlStorage.getImplement()->initializeXml();

	return new GXmlStorageWriter(xmlStorage.getImplement()->getXml(), xmlStorage.getImplement()->getDataNode(), xmlStorage.getImplement()->getClassTypeNode(), func);
}

IMetaStorageReader * doCreateXmlMetaReader(const GMetaXmlStorage & xmlStorage, FuncStreamReadFundamental func)
{
	return new GXmlStorageReader(xmlStorage.getImplement()->getDataNode(), xmlStorage.getImplement()->getClassTypeNode(), func);
}


GXmlStorageWriter::GXmlStorageWriter(xml_document<> * xml, XmlNodeType * dataNode, XmlNodeType * classTypeNode,
	serialization_internal::FuncStreamWriteFundamental streamWriteFundamental)
	: xml(xml), dataNode(dataNode), classTypeNode(classTypeNode), streamWriteFundamental(streamWriteFundamental),
		variantTypeMap(defaultVariantTypeMap)
{
	this->nodeStack.push(this->dataNode);
}

GXmlStorageWriter::~GXmlStorageWriter()
{
}

void G_API_CC GXmlStorageWriter::writeFundamental(const char * name, const GVariantData * value)
{
	GVariant v(createVariantFromData(*value));

	this->clearTextStream();
	this->streamWriteFundamental(this->textStream, v);
	
	XmlNodeType * newNode = this->addNode(name, this->textStream.str().c_str());
	this->addType(newNode, getMappedTypeFromMap(this->variantTypeMap, v.getType()));
}

void G_API_CC GXmlStorageWriter::writeString(const char * name, uint32_t archiveID, const char * value)
{
	XmlNodeType * newNode = this->addNode(name, value);
	this->addType(newNode, ptString);
	this->addIntAttribute(newNode, nameArchiveID, archiveID);
}

void G_API_CC GXmlStorageWriter::writeNullPointer(const char * name)
{
	XmlNodeType * newNode = this->addNode(name, "");
	this->addType(newNode, ptNull);
}

void G_API_CC GXmlStorageWriter::beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version)
{
	XmlNodeType * newNode = this->addNode(name);
	this->pushNode(newNode);
	this->addType(newNode, ptObject);
	this->addIntAttribute(newNode, nameVersion, version);
	this->addIntAttribute(newNode, nameArchiveID, archiveID);
	this->addIntAttribute(newNode, nameClassTypeID, classTypeID);
}

void G_API_CC GXmlStorageWriter::endWriteObject(const char * /*name*/, uint32_t /*archiveID*/, uint32_t /*classTypeID*/, uint32_t /*version*/)
{
	this->popNode();
}

void G_API_CC GXmlStorageWriter::writeReferenceID(const char * name, uint32_t referenceArchiveID)
{
	XmlNodeType * newNode = this->addNode(name);
	this->addType(newNode, ptReferenceID);
	this->addIntAttribute(newNode, nameReferenceID, referenceArchiveID);
}

void G_API_CC GXmlStorageWriter::writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass)
{
	this->clearTextStream();
	this->textStream << prefixClassType << classTypeID;

	XmlNodeType * newNode = this->xml->allocate_node(node_element, this->newString(this->textStream.str().c_str()), metaClass->getQualifiedName());
	this->classTypeNode->append_node(newNode);
	this->addType(newNode, ptClassType);
	this->addIntAttribute(newNode, nameArchiveID, classTypeID);
}

void G_API_CC GXmlStorageWriter::beginWriteArray(const char * name, uint32_t length)
{
	XmlNodeType * newNode = this->addNode(name);
	this->pushNode(newNode);
	this->addType(newNode, ptArray);
	this->addIntAttribute(newNode, nameLength, length);
}

void G_API_CC GXmlStorageWriter::endWriteArray(const char * /*name*/, uint32_t /*length*/)
{
	this->popNode();
}

void GXmlStorageWriter::clearTextStream()
{
	this->textStream.str("");
	this->textStream.clear();
}

const char * GXmlStorageWriter::newString(const char * s)
{
	return s == nullptr ? nullptr : this->xml->allocate_string(s);
}

XmlNodeType * GXmlStorageWriter::getCurrentNode() const
{
	return this->nodeStack.top();
}

void GXmlStorageWriter::pushNode(XmlNodeType * node)
{
	this->nodeStack.push(node);
}

void GXmlStorageWriter::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->nodeStack.pop();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

XmlNodeType * GXmlStorageWriter::addNode(const char * name, const char * value)
{
	GASSERT(name != nullptr);
	GASSERT(strlen(name) > 0);

	XmlNodeType * newNode = this->xml->allocate_node(node_element, this->newString(name), this->newString(value));
	this->getCurrentNode()->append_node(newNode);
	return newNode;
}

XmlNodeType * GXmlStorageWriter::addNode(const char * name)
{
	return this->addNode(name, nullptr);
}

XmlAttributeType * GXmlStorageWriter::addAttribute(XmlNodeType * node, const char * name, const char * value)
{
	XmlAttributeType * newAttribute = this->xml->allocate_attribute(this->newString(name), this->newString(value));
	node->append_attribute(newAttribute);
	return newAttribute;
}

XmlAttributeType * GXmlStorageWriter::addAttribute(XmlNodeType * node, const char * name)
{
	return this->addAttribute(node, name, nullptr);
}

void GXmlStorageWriter::addIntAttribute(XmlNodeType * node, const char * name, long long value)
{
	this->clearTextStream();
	this->textStream << value;
	this->addAttribute(node, this->newString(name), this->textStream.str().c_str());
}

void GXmlStorageWriter::addType(XmlNodeType * node, int permanentType)
{
	this->addIntAttribute(node, nameType, permanentType);
}



GXmlStorageReader::GXmlStorageReader(XmlNodeType * dataNode, XmlNodeType * classTypeNode, serialization_internal::FuncStreamReadFundamental streamReadFundamental)
	: dataNode(dataNode), classTypeNode(classTypeNode), streamReadFundamental(streamReadFundamental), variantTypeMap(defaultVariantTypeMap)
{
	this->nodeStack.push(GXmlReaderNodeNameTracker(this->dataNode));
}

GXmlStorageReader::~GXmlStorageReader()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

uint32_t G_API_CC GXmlStorageReader::getArchiveType(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	
	if(node == nullptr) {
		return matMissed;
	}

	PermanentType type = this->readType(node);
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

uint32_t G_API_CC GXmlStorageReader::getClassTypeID(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptObject);

	uint32_t classTypeID = this->getIntAttribute(node, nameClassTypeID);

	return classTypeID;
}

void G_API_CC GXmlStorageReader::readFundamental(const char * name, GVariantData * outValue)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	
	GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);
	if(vt == GVariantType::vtEmpty) {
		serializeError(Error_Serialization_TypeMismatch);
	}

	this->setTextStream(node->value());

	GVariant v(this->streamReadFundamental(this->textStream, vt));
	*outValue = v.takeData();

	this->gotoNextNode(node, name);
}

char * G_API_CC GXmlStorageReader::readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptString);

	*outArchiveID = this->getIntAttribute(node, nameArchiveID);
	
	char * s = this->doReadString(node, allocator);

	this->gotoNextNode(node, name);

	return s;
}

void * G_API_CC GXmlStorageReader::readNullPointer(const char * name)
{
	XmlNodeType * node = this->getNode(name);

	if(node != nullptr) {
		this->gotoNextNode(node, name);
	}

	return nullptr;
}

uint32_t G_API_CC GXmlStorageReader::beginReadObject(const char * name, uint32_t * outVersion)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptObject);

	*outVersion = this->getIntAttribute(node, nameVersion);

	uint32_t archiveID = this->getIntAttribute(node, nameArchiveID);

	this->gotoNextNode(node, name);

	this->pushNode(node);

	return archiveID;
}

void G_API_CC GXmlStorageReader::endReadObject(const char * /*name*/, uint32_t /*version*/)
{
	this->popNode();
}

uint32_t G_API_CC GXmlStorageReader::readReferenceID(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptReferenceID);

	uint32_t referenceArchiveID = this->getIntAttribute(node, nameReferenceID);
	
	this->gotoNextNode(node, name);

	return referenceArchiveID;
}

IMetaClass * G_API_CC GXmlStorageReader::readMetaClassAndTypeID(IMetaService * /*service*/, uint32_t * /*outClassTypeID*/)
{
	return nullptr;
}

IMetaClass * G_API_CC GXmlStorageReader::readMetaClass(IMetaService * service, uint32_t classTypeID)
{
	this->setTextStream("");
	this->textStream << prefixClassType << classTypeID;
	XmlNodeType * node = this->classTypeNode->first_node(this->textStream.str().c_str());
	if(node != nullptr) {
		return service->findClassByName(node->value());
	}
	else {
		return nullptr;
	}
}

uint32_t G_API_CC GXmlStorageReader::beginReadArray(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptArray);

	uint32_t len = this->getIntAttribute(node, nameLength);

	this->gotoNextNode(node, name);

	this->pushNode(node);

	return len;
}

void G_API_CC GXmlStorageReader::endReadArray(const char * /*name*/)
{
	this->popNode();
}

char * GXmlStorageReader::doReadString(XmlNodeType * node, IMemoryAllocator * allocator)
{
	char * s = node->value();
	char * result = static_cast<char *>(allocator->allocate(static_cast<uint32_t>(strlen(s) + 1)));
	strcpy(result, s);
	return result;
}

void GXmlStorageReader::setTextStream(const char * text)
{
	this->textStream.str(text == nullptr ? "" : text);
	this->textStream.clear();
}

XmlNodeType * GXmlStorageReader::getCurrentNode() const
{
	return this->nodeStack.top().getNode();
}

void GXmlStorageReader::pushNode(XmlNodeType * node)
{
	this->nodeStack.push(GXmlReaderNodeNameTracker(node));
}

void GXmlStorageReader::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GXmlStorageReader::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

XmlNodeType * GXmlStorageReader::getNode(const char * name)
{
	XmlNodeType * firstNode = this->getCurrentNode()->first_node(name);
	size_t index = this->nodeStack.top().getIndex(name);

	return getSiblingAt(firstNode, name, index);
}

void GXmlStorageReader::gotoNextNode(XmlNodeType * currentNode, const char * name)
{
	if(currentNode->next_sibling(name) != 0) {
		this->nodeStack.top().addIndex(name);
	}
}

XmlAttributeType * GXmlStorageReader::getAttribute(XmlNodeType * node, const char * name)
{
	return node->first_attribute(name);
}

uint32_t GXmlStorageReader::getIntAttribute(XmlNodeType * node, const char * name)
{
	XmlAttributeType * attribute = this->getAttribute(node, name);
	
	checkNode(attribute, name);
	this->setTextStream(attribute->value());

	uint32_t value = 0;
	this->textStream >> value;

	return value;
}

PermanentType GXmlStorageReader::readType(XmlNodeType * node)
{
	return static_cast<PermanentType>(this->getIntAttribute(node, nameType));
}



} // namespace serialization_internal



IMetaStorageWriter * createXmlStorageWriter(const GMetaXmlStorage & xmlStorage)
{
	return serialization_internal::doCreateXmlMetaWriter(xmlStorage, &streamWriteFundamental<PromotedPermenentTypeMap>);
}

IMetaStorageReader * createXmlStorageReader(const GMetaXmlStorage & xmlStorage)
{
	return serialization_internal::doCreateXmlMetaReader(xmlStorage, &streamReadFundamental<PromotedPermenentTypeMap>);
}

	
} // namespace cpgf

