#ifndef __GMETAOBJECTARRAY_H
#define __GMETAOBJECTARRAY_H

#include "cpgf/gmetaapi.h"


namespace cpgf {

class GMetaObjectArrayImplement;

class GMetaObjectArray
{
public:
	explicit GMetaObjectArray(IMetaClass * metaClass);
	~GMetaObjectArray();

	void * getMemory() const;

	GVariant getItem(uint32_t index) const;
	void setItem(uint32_t index, const GVariant & value);
	
	uint32_t getCount() const;

private:
	GScopedPointer<GMetaObjectArrayImplement> implement;
};


} // namespace cpgf



#endif
