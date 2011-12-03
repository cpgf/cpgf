#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {


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



