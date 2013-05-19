#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gscopedinterface.h"


namespace cpgf {


GMetaClassTraveller::Node::Node(IMetaClass * metaClass, void * instance, IMetaClass * derived)
	: metaClass(metaClass), instance(instance), derived(derived)
{
}

GMetaClassTraveller::Node::Node(const GMetaClassTraveller::Node & other)
	: metaClass(other.metaClass), instance(other.instance), derived(other.derived)
{
}

GMetaClassTraveller::Node & GMetaClassTraveller::Node::operator = (const GMetaClassTraveller::Node & other)
{
	this->metaClass = other.metaClass;
	this->instance = other.instance;
	this->derived = other.derived;

	return *this;
}

GMetaClassTraveller::Node::~Node()
{
}


GMetaClassTraveller::GMetaClassTraveller(IMetaClass * metaClass, void * instance)
{
	GASSERT(metaClass != NULL);

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
	if(outDerived != NULL) {
		*outDerived = NULL;
	}
	
	if(this->traversal.empty()) {
		return NULL;
	}
	
	Node node = this->traversal.front();
	this->traversal.pop_front();
	for(uint32_t i = 0; i < node.metaClass->getBaseCount(); ++i) {
		GScopedInterface<IMetaClass> baseClass(node.metaClass->getBaseClass(i));
		if(baseClass) {
			baseClass->addReference();
			this->traversal.push_back(Node(baseClass.get(), node.metaClass->castToBase(node.instance, i), node.metaClass.get()));
		}
	}
	
	if(outInstance != NULL) {
		*outInstance = node.instance;
	}

	if(outDerived != NULL) {
		*outDerived = node.derived.get();
		if(node.derived) {
			node.derived->addReference();
		}
	}
	
	return node.metaClass.get();
}

IMetaClass * GMetaClassTraveller::next(void ** outInstance)
{
	return this->next(outInstance, NULL);
}


} // namespace cpgf



