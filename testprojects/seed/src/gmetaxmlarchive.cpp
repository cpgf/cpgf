#include "gmetaxmlarchive.h"
#include "gmetaarchivecommon.h"
#include "gmetaarchivereader.h"
#include "gmetaarchivewriter.h"

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


namespace serialization_internal {


typedef xml_node<char> XmlNodeType;	
typedef xml_attribute<char> XmlAttributeType;

const char * const nameRootNode = "cpgf";
const char * const nameDataNode = "data";
const char * const nameClassTypesNode = "classtypes";

const char * const nameType = "t";
const char * const nameArchiveID = "id";
const char * const nameReferenceID = "rid";
const char * const nameClassTypeID = "cid";
const char * const nameLength = "len";
const char * const prefixClassType = "c";

void checkNode(void * node, const char * /*nodeName*/)
{
	if(node == NULL) {
		serializeError(Error_Serialization_InvalidStorage);
	}
}

XmlNodeType * getSiblingAt(XmlNodeType * node, const char * name, size_t index)
{
	while(index > 0 && node != NULL) {
		--index;
		node = node->next_sibling(name);
	}

	return node;
}


class GXmlMetaWriter : public IMetaWriter
{
	GMAKE_NONCOPYABLE(GXmlMetaWriter);

private:
	typedef stack<XmlNodeType *> ObjectNodeStack;

public:
	GXmlMetaWriter(std::ostream & outputStream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental);

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value);
	virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value);
	virtual void G_API_CC writeNullPointer(const char * name);
	
	virtual void G_API_CC beginWriteObject(GMetaArchiveWriterParam * param);
	virtual void G_API_CC endWriteObject(GMetaArchiveWriterParam * param);

	virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID);
	virtual void G_API_CC writeClassType(uint32_t classTypeID, IMetaClass * metaClass);

	virtual void G_API_CC beginWriteArray(const char * name, uint32_t length);
	virtual void G_API_CC endWriteArray(const char * name, uint32_t length);
	
	virtual void G_API_CC flush();

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
	std::ostream & outputStream;
	serialization_internal::FuncStreamWriteFundamental streamWriteFundamental;
	int * variantTypeMap;
	xml_document<> xml;
	ObjectNodeStack nodeStack;
	stringstream textStream;
	XmlNodeType * classTypeNode;
};

class GXmlMetaReaderNameTracker
{
private:
	typedef GStringMap<size_t, GStringMapReuseKey> MapType;

public:
	explicit GXmlMetaReaderNameTracker(XmlNodeType * node);
	void free();

	GXmlMetaReaderNameTracker(const GXmlMetaReaderNameTracker & other);
	GXmlMetaReaderNameTracker & operator = (const GXmlMetaReaderNameTracker & other);

	XmlNodeType * getNode() const;

	size_t getIndex(const char * name) const;
	void addIndex(const char * name);

private:
	XmlNodeType * node;
	MapType * countMap;
};

class GXmlMetaReader : public IMetaReader
{
	GMAKE_NONCOPYABLE(GXmlMetaReader);

private:
	typedef stack<GXmlMetaReaderNameTracker> ObjectNodeStack;

public:
	GXmlMetaReader(IMetaService * service, char * xmlContent, serialization_internal::FuncStreamReadFundamental streamReadFundamental);
	~GXmlMetaReader();

protected:
	G_INTERFACE_IMPL_OBJECT

	virtual uint32_t G_API_CC getArchiveType(const char * name);
	virtual uint32_t G_API_CC getClassType(const char * name);
	
	virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue);
	virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID);
	virtual void * G_API_CC readNullPointer(const char * name);

	virtual uint32_t G_API_CC beginReadObject(GMetaArchiveReaderParam * param);
	virtual void G_API_CC endReadObject(GMetaArchiveReaderParam * param);

	virtual uint32_t G_API_CC readReferenceID(const char * name);
	virtual IMetaClass * G_API_CC readClassAndTypeID(uint32_t * outClassTypeID);
	virtual IMetaClass * G_API_CC readClass(uint32_t classTypeID);

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
	GScopedInterface<IMetaService> service;
	serialization_internal::FuncStreamReadFundamental streamReadFundamental;
	int * variantTypeMap;
	xml_document<> xml;
	ObjectNodeStack nodeStack;
	stringstream textStream;
	XmlNodeType * classTypeNode;
};


IMetaWriter * doCreateXmlMetaWriter(std::ostream & outputStream, FuncStreamWriteFundamental func)
{
	return new GXmlMetaWriter(outputStream, func);
}

IMetaReader * doCreateXmlMetaReader(IMetaService * service, char * xmlContent, FuncStreamReadFundamental func)
{
	return new GXmlMetaReader(service, xmlContent, func);
}


GXmlMetaWriter::GXmlMetaWriter(std::ostream & outputStream, serialization_internal::FuncStreamWriteFundamental streamWriteFundamental)
	: outputStream(outputStream), streamWriteFundamental(streamWriteFundamental), variantTypeMap(defaultVariantTypeMap)
{
	XmlNodeType * rootNode = this->xml.allocate_node(node_element, this->newString(nameRootNode));
	this->xml.append_node(rootNode);

	XmlNodeType * dataNode = this->xml.allocate_node(node_element, this->newString(nameDataNode));
	rootNode->append_node(dataNode);
	
	this->nodeStack.push(dataNode);

	this->classTypeNode = this->xml.allocate_node(node_element, this->newString(nameClassTypesNode));
	rootNode->append_node(this->classTypeNode);
}

void G_API_CC GXmlMetaWriter::writeFundamental(const char * name, const GVariantData * value)
{
	GVariant v(*value);

	this->clearTextStream();
	this->streamWriteFundamental(this->textStream, v);
	
	XmlNodeType * newNode = this->addNode(name, this->textStream.str().c_str());
	this->addType(newNode, getMappedTypeFromMap(this->variantTypeMap, v.getType()));
}

void G_API_CC GXmlMetaWriter::writeString(const char * name, uint32_t archiveID, const char * value)
{
	XmlNodeType * newNode = this->addNode(name, value);
	this->addType(newNode, ptString);
	this->addIntAttribute(newNode, nameArchiveID, archiveID);
}

void G_API_CC GXmlMetaWriter::writeNullPointer(const char * name)
{
	XmlNodeType * newNode = this->addNode(name, "");
	this->addType(newNode, ptNull);
}

void G_API_CC GXmlMetaWriter::beginWriteObject(GMetaArchiveWriterParam * param)
{
	XmlNodeType * newNode = this->addNode(param->name);
	this->pushNode(newNode);
	this->addType(newNode, ptObject);
	this->addIntAttribute(newNode, nameArchiveID, param->archiveID);
	this->addIntAttribute(newNode, nameClassTypeID, param->classTypeID);
}

void G_API_CC GXmlMetaWriter::endWriteObject(GMetaArchiveWriterParam * /*param*/)
{
	this->popNode();
}

void G_API_CC GXmlMetaWriter::writeReferenceID(const char * name, uint32_t referenceArchiveID)
{
	XmlNodeType * newNode = this->addNode(name);
	this->addType(newNode, ptReferenceID);
	this->addIntAttribute(newNode, nameReferenceID, referenceArchiveID);
}

void G_API_CC GXmlMetaWriter::writeClassType(uint32_t classTypeID, IMetaClass * metaClass)
{
	this->clearTextStream();
	this->textStream << prefixClassType << classTypeID;

	XmlNodeType * newNode = this->xml.allocate_node(node_element, this->newString(this->textStream.str().c_str()), metaClass->getTypeName());
	this->classTypeNode->append_node(newNode);
	this->addType(newNode, ptClassType);
	this->addIntAttribute(newNode, nameArchiveID, classTypeID);
}

void G_API_CC GXmlMetaWriter::beginWriteArray(const char * name, uint32_t length)
{
	XmlNodeType * newNode = this->addNode(name);
	this->pushNode(newNode);
	this->addType(newNode, ptArray);
	this->addIntAttribute(newNode, nameLength, length);
}

void G_API_CC GXmlMetaWriter::endWriteArray(const char * /*name*/, uint32_t /*length*/)
{
	this->popNode();
}

void G_API_CC GXmlMetaWriter::flush()
{
	this->outputStream << this->xml;
}

void GXmlMetaWriter::clearTextStream()
{
	this->textStream.str("");
	this->textStream.clear();
}

const char * GXmlMetaWriter::newString(const char * s)
{
	return s == NULL ? NULL : this->xml.allocate_string(s);
}

XmlNodeType * GXmlMetaWriter::getCurrentNode() const
{
	return this->nodeStack.top();
}

void GXmlMetaWriter::pushNode(XmlNodeType * node)
{
	this->nodeStack.push(node);
}

void GXmlMetaWriter::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->nodeStack.pop();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

XmlNodeType * GXmlMetaWriter::addNode(const char * name, const char * value)
{
	GASSERT(name != NULL);
	GASSERT(strlen(name) > 0);

	XmlNodeType * newNode = this->xml.allocate_node(node_element, this->newString(name), this->newString(value));
	this->getCurrentNode()->append_node(newNode);
	return newNode;
}

XmlNodeType * GXmlMetaWriter::addNode(const char * name)
{
	return this->addNode(name, NULL);
}

XmlAttributeType * GXmlMetaWriter::addAttribute(XmlNodeType * node, const char * name, const char * value)
{
	XmlAttributeType * newAttribute = this->xml.allocate_attribute(this->newString(name), this->newString(value));
	node->append_attribute(newAttribute);
	return newAttribute;
}

XmlAttributeType * GXmlMetaWriter::addAttribute(XmlNodeType * node, const char * name)
{
	return this->addAttribute(node, name, NULL);
}

void GXmlMetaWriter::addIntAttribute(XmlNodeType * node, const char * name, long long value)
{
	this->clearTextStream();
	this->textStream << value;
	this->addAttribute(node, this->newString(name), this->textStream.str().c_str());
}

void GXmlMetaWriter::addType(XmlNodeType * node, int permanentType)
{
	this->addIntAttribute(node, nameType, permanentType);
}



GXmlMetaReaderNameTracker::GXmlMetaReaderNameTracker(XmlNodeType * node)
	: node(node), countMap(NULL)
{
}

void GXmlMetaReaderNameTracker::free()
{
	delete this->countMap;
	this->countMap = NULL;
}

GXmlMetaReaderNameTracker::GXmlMetaReaderNameTracker(const GXmlMetaReaderNameTracker & other)
	: node(other.node), countMap(other.countMap)
{
}

GXmlMetaReaderNameTracker & GXmlMetaReaderNameTracker::operator = (const GXmlMetaReaderNameTracker & other)
{
	this->node = other.node;
	this->countMap = other.countMap;

	return *this;
}

XmlNodeType * GXmlMetaReaderNameTracker::getNode() const
{
	return this->node;
}

size_t GXmlMetaReaderNameTracker::getIndex(const char * name) const
{
	if(this->countMap != NULL) {
		MapType::const_iterator it = this->countMap->find(name);
		if(it != this->countMap->end()) {
			return it->second;
		}
	}

	return 0;
}

void GXmlMetaReaderNameTracker::addIndex(const char * name)
{
	if(this->countMap == NULL) {
		this->countMap = new MapType;
	}

	this->countMap->set(name, this->getIndex(name) + 1);
}



GXmlMetaReader::GXmlMetaReader(IMetaService * service, char * xmlContent, serialization_internal::FuncStreamReadFundamental streamReadFundamental)
	: service(service), streamReadFundamental(streamReadFundamental), variantTypeMap(defaultVariantTypeMap)
{
	this->xml.parse<0>(xmlContent);

	XmlNodeType * rootNode = this->xml.first_node(nameRootNode);
	checkNode(rootNode, "root");

	XmlNodeType * dataNode = rootNode->first_node(nameDataNode);
	checkNode(dataNode, "dataNode");
	
	this->nodeStack.push(GXmlMetaReaderNameTracker(dataNode));

	this->classTypeNode = rootNode->first_node(nameClassTypesNode);
	checkNode(this->classTypeNode, "classTypeNode");

	if(this->service) {
		this->service->addReference();
	}
}

GXmlMetaReader::~GXmlMetaReader()
{
	while(! this->nodeStack.empty()) {
		this->doPopNode();
	}
}

uint32_t G_API_CC GXmlMetaReader::getArchiveType(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

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

uint32_t G_API_CC GXmlMetaReader::getClassType(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptObject);

	uint32_t classTypeID = this->getIntAttribute(node, nameClassTypeID);

	return classTypeID;
}

void G_API_CC GXmlMetaReader::readFundamental(const char * name, GVariantData * outValue)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	
	GVariantType vt = getVariantTypeFromMap(this->variantTypeMap, type);
	if(vt == vtEmpty) {
		serializeError(Error_Serialization_TypeMismatch);
	}

	this->setTextStream(node->value());

	GVariant v(this->streamReadFundamental(this->textStream, vt));
	*outValue = v.takeData();

	this->gotoNextNode(node, name);
}

char * G_API_CC GXmlMetaReader::readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID)
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

void * G_API_CC GXmlMetaReader::readNullPointer(const char * name)
{
	XmlNodeType * node = this->getNode(name);

	if(node != NULL) {
		this->gotoNextNode(node, name);
	}

	return NULL;
}

uint32_t G_API_CC GXmlMetaReader::beginReadObject(GMetaArchiveReaderParam * param)
{
	XmlNodeType * node = this->getNode(param->name);
	checkNode(node, param->name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptObject);

	uint32_t archiveID = this->getIntAttribute(node, nameArchiveID);

	this->gotoNextNode(node, param->name);

	this->pushNode(node);

	return archiveID;
}

void G_API_CC GXmlMetaReader::endReadObject(GMetaArchiveReaderParam * /*param*/)
{
	this->popNode();
}

uint32_t G_API_CC GXmlMetaReader::readReferenceID(const char * name)
{
	XmlNodeType * node = this->getNode(name);
	checkNode(node, name);

	PermanentType type = this->readType(node);
	serializeCheckType(type, ptReferenceID);

	uint32_t referenceArchiveID = this->getIntAttribute(node, nameReferenceID);
	
	this->gotoNextNode(node, name);

	return referenceArchiveID;
}

IMetaClass * G_API_CC GXmlMetaReader::readClassAndTypeID(uint32_t * /*outClassTypeID*/)
{
	return NULL;
}

IMetaClass * G_API_CC GXmlMetaReader::readClass(uint32_t classTypeID)
{
	this->setTextStream("");
	this->textStream << prefixClassType << classTypeID;
	XmlNodeType * node = this->classTypeNode->first_node(this->textStream.str().c_str());
	if(node != NULL) {
		return this->service->findClassByName(node->value());
	}
	else {
		return NULL;
	}
}

uint32_t G_API_CC GXmlMetaReader::beginReadArray(const char * name)
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

void G_API_CC GXmlMetaReader::endReadArray(const char * /*name*/)
{
	this->popNode();
}

char * GXmlMetaReader::doReadString(XmlNodeType * node, IMemoryAllocator * allocator)
{
	char * s = node->value();
	char * result = static_cast<char *>(allocator->allocate(static_cast<uint32_t>(strlen(s) + 1)));
	strcpy(result, s);
	return result;
}

void GXmlMetaReader::setTextStream(const char * text)
{
	this->textStream.str(text == NULL ? "" : text);
	this->textStream.clear();
}

XmlNodeType * GXmlMetaReader::getCurrentNode() const
{
	return this->nodeStack.top().getNode();;
}

void GXmlMetaReader::pushNode(XmlNodeType * node)
{
	this->nodeStack.push(GXmlMetaReaderNameTracker(node));
}

void GXmlMetaReader::popNode()
{
	GASSERT(! this->nodeStack.empty());

	this->doPopNode();

	// still can't be empty, because there at least be the root node.
	GASSERT(! this->nodeStack.empty());
}

void GXmlMetaReader::doPopNode()
{
	this->nodeStack.top().free();
	this->nodeStack.pop();
}

XmlNodeType * GXmlMetaReader::getNode(const char * name)
{
	XmlNodeType * firstNode = this->getCurrentNode()->first_node(name);
	size_t index = this->nodeStack.top().getIndex(name);

	return getSiblingAt(firstNode, name, index);
}

void GXmlMetaReader::gotoNextNode(XmlNodeType * currentNode, const char * name)
{
	if(currentNode->next_sibling(name) != 0) {
		this->nodeStack.top().addIndex(name);
	}
}

XmlAttributeType * GXmlMetaReader::getAttribute(XmlNodeType * node, const char * name)
{
	return node->first_attribute(name);
}

uint32_t GXmlMetaReader::getIntAttribute(XmlNodeType * node, const char * name)
{
	XmlAttributeType * attribute = this->getAttribute(node, name);
	
	checkNode(attribute, name);
	this->setTextStream(attribute->value());

	uint32_t value = 0;
	this->textStream >> value;

	return value;
}

PermanentType GXmlMetaReader::readType(XmlNodeType * node)
{
	return static_cast<PermanentType>(this->getIntAttribute(node, nameType));
}



} // namespace serialization_internal


} // namespace cpgf

