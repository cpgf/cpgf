#ifndef __GMETAOBJECTARRAY_H
#define __GMETAOBJECTARRAY_H

#include "cpgf/gmetaapi.h"


namespace cpgf {

class GMetaObjectArrayImplement;

class GMetaObjectArray
{
public:
	explicit GMetaObjectArray(IMetaClass * metaClass);

private:
	GScopedPointer<GMetaObjectArrayImplement> implement;
};


} // namespace cpgf



#endif
