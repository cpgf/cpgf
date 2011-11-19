#ifndef __GMETACLASSTRAVELLER_H
#define __GMETACLASSTRAVELLER_H


#include "cpgf/gclassutil.h"

#include <deque>

#include <stddef.h>


namespace cpgf {

struct IMetaClass;

class GMetaClassTraveller : public GNoncopyable
{
private:
	struct Node {
		Node(IMetaClass * metaClass, void * instance) : metaClass(metaClass), instance(instance) {
		}

		IMetaClass * metaClass;
		void * instance;
	};
	
	typedef std::deque<Node> ListType;
	
public:
	GMetaClassTraveller(IMetaClass * metaClass, void * instance);
	~GMetaClassTraveller();
	
	IMetaClass * next(void ** outInstance);

private:
	ListType traversal;
};



} // namespace cpgf


#endif

