#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {


GMetaClassTraveller::Node::Node(IMetaClass * metaClass, void * instance, IMetaClass * derived)
	: metaClass(metaClass), instance(instance), derived(derived)
{
	this->retain();
}

GMetaClassTraveller::Node::Node(const GMetaClassTraveller::Node & other)
	: metaClass(other.metaClass), instance(other.instance), derived(other.derived)
{
	this->retain();
}

GMetaClassTraveller::Node & GMetaClassTraveller::Node::operator = (const GMetaClassTraveller::Node & other)
{
	this->metaClass = other.metaClass;
	this->instance = other.instance;
	this->derived = other.derived;

	this->retain();

	return *this;
}

GMetaClassTraveller::Node::~Node()
{
	if(this->metaClass != NULL) {
		this->metaClass->releaseReference();
	}
	if(this->derived != NULL) {
		this->derived->releaseReference();
	}
}

void GMetaClassTraveller::Node::retain()
{
	if(this->metaClass != NULL) {
		this->metaClass->addReference();
	}
	if(this->derived != NULL) {
		this->derived->addReference();
	}
}


GMetaClassTraveller::GMetaClassTraveller(IMetaClass * metaClass, void * instance)
{
	metaClass->addReference();

	this->traversal.push_back(Node(metaClass, instance, NULL));
}

GMetaClassTraveller::~GMetaClassTraveller()
{
	for(ListType::iterator it = this->traversal.begin(); it != this->traversal.end(); ++it) {
		it->metaClass->releaseReference();
	}
}
	
IMetaClass * GMetaClassTraveller::next(void ** outInstance, IMetaClass ** outDerived)
{
	if(this->traversal.empty()) {
		return NULL;
	}
	
	Node node = this->traversal.front();
	this->traversal.pop_front();
	for(uint32_t i = 0; i < node.metaClass->getBaseCount(); ++i) {
		this->traversal.push_back(Node(node.metaClass->getBaseClass(i), node.metaClass->castToBase(node.instance, i), node.metaClass));
	}
	
	if(outInstance != NULL) {
		*outInstance = node.instance;
	}

	if(outDerived != NULL) {
		*outDerived = node.derived;
		if(node.derived != NULL) {
			node.derived->addReference();
		}
	}
	
	return node.metaClass;
}

IMetaClass * GMetaClassTraveller::next(void ** outInstance)
{
	return this->next(outInstance, NULL);
}


} // namespace cpgf



