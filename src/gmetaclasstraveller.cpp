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

GMetaClassTraveller::Node & GMetaClassTraveller::Node::operator = (GMetaClassTraveller::Node other)
{
	this->swap(other);

	return *this;
}

GMetaClassTraveller::Node & GMetaClassTraveller::Node::operator = (GMetaClassTraveller::Node && other)
{
	this->swap(other);

	return *this;
}

GMetaClassTraveller::Node::~Node()
{
}

void GMetaClassTraveller::Node::swap(Node & other)
{
	using std::swap;

	swap(this->metaClass, other.metaClass);
	swap(this->instance, other.instance);
	swap(this->derived, other.derived);
}


GMetaClassTraveller::GMetaClassTraveller(IMetaClass * metaClass, void * instance)
{
	GASSERT(metaClass != nullptr);

	metaClass->addReference();

	this->traversal.emplace_back(metaClass, instance, nullptr);
}

GMetaClassTraveller::~GMetaClassTraveller()
{
	for(ListType::iterator it = this->traversal.begin(); it != this->traversal.end(); ++it) {
		it->metaClass->releaseReference();
	}
}
	
IMetaClass * GMetaClassTraveller::next(void ** outInstance, IMetaClass ** outDerived)
{
	if(outDerived != nullptr) {
		*outDerived = nullptr;
	}
	
	if(this->traversal.empty()) {
		return nullptr;
	}
	
	Node node = this->traversal.front();
	this->traversal.pop_front();
	for(uint32_t i = 0; i < node.metaClass->getBaseCount(); ++i) {
		GScopedInterface<IMetaClass> baseClass(node.metaClass->getBaseClass(i));
		if(baseClass) {
			baseClass->addReference();
			this->traversal.emplace_back(baseClass.get(), node.metaClass->castToBase(node.instance, i), node.metaClass.get());
		}
	}
	
	if(outInstance != nullptr) {
		*outInstance = node.instance;
	}

	if(outDerived != nullptr) {
		*outDerived = node.derived.get();
		if(node.derived) {
			node.derived->addReference();
		}
	}
	
	return node.metaClass.get();
}

IMetaClass * GMetaClassTraveller::next(void ** outInstance)
{
	return this->next(outInstance, nullptr);
}


} // namespace cpgf



