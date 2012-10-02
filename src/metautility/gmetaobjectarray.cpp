#include "cpgf/metautility/gmetaobjectarray.h"


namespace cpgf {

class GMetaObjectArrayImplement
{
public:
	explicit GMetaObjectArrayImplement(IMetaClass * metaClass);
	
public:
	GSharedInterface<IMetaClass> metaClass;
};


GMetaObjectArray::GMetaObjectArray(IMetaClass * metaClass)
	: implement(new GMetaObjectArrayImplement(metaClass))
{
}


} // namespace cpgf

