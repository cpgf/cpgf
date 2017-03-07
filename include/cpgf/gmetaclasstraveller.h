#ifndef CPGF_GMETACLASSTRAVELLER_H
#define CPGF_GMETACLASSTRAVELLER_H


#include "cpgf/gclassutil.h"
#include "cpgf/gapi.h"
#include "cpgf/gsharedinterface.h"

#include <deque>

#include <stddef.h>


namespace cpgf {

struct IMetaClass;

class GMetaClassTraveller : public GNoncopyable
{
private:
	class Node {
	public:
		Node(IMetaClass * metaClass, void * instance, IMetaClass * derived);
		~Node();

		Node(const Node & other);
		Node & operator = (Node other);
		Node & operator = (Node && other);

		void swap(Node & other);

	public:
		GSharedInterface<IMetaClass> metaClass;
		void * instance;
		GSharedInterface<IMetaClass> derived;
	};
	
	typedef std::deque<Node> ListType;
	
public:
	GMetaClassTraveller(IMetaClass * metaClass, void * instance);
	~GMetaClassTraveller();
	
	IMetaClass * next(void ** outInstance, IMetaClass ** outDerived);
	IMetaClass * next(void ** outInstance);

private:
	ListType traversal;
};



} // namespace cpgf


#endif

