#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {


GMetaClassTraveller::GMetaClassTraveller(IMetaClass * metaClass, void * instance)
{
	metaClass->addReference();

	this->traversal.push_back(Node(metaClass, instance));
}

GMetaClassTraveller::~GMetaClassTraveller()
{
	for(ListType::iterator it = this->traversal.begin(); it != this->traversal.end(); ++it) {
		it->metaClass->releaseReference();
	}
}
	
IMetaClass * GMetaClassTraveller::next(void ** outInstance)
{
	if(this->traversal.empty()) {
		return NULL;
	}
	
	Node node = this->traversal.front();
	this->traversal.pop_front();
	for(uint32_t i = 0; i < node.metaClass->getBaseCount(); ++i) {
		this->traversal.push_back(Node(node.metaClass->getBaseClass(i), node.metaClass->castToBase(node.instance, i)));
	}
	
	if(outInstance != NULL) {
		*outInstance = node.instance;
	}
	
	return node.metaClass;
}

} // namespace cpgf



