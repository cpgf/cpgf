#ifndef CPGF_GMETAOBJECTARRAY_H
#define CPGF_GMETAOBJECTARRAY_H

#include "cpgf/gmetaapi.h"


namespace cpgf {

struct GMetaVariadicParam;

class GMetaObjectArrayImplement;

class GMetaObjectArray
{
public:
	explicit GMetaObjectArray(IMetaClass * metaClass);
	~GMetaObjectArray();

	GVariant getItem(uint32_t index) const;
	void setItem(uint32_t index, const GVariant & value, const GMetaVariadicParam * moreValues = nullptr);
	
	uint32_t getCount() const;

private:
	GScopedPointer<GMetaObjectArrayImplement> implement;
};


} // namespace cpgf



#endif
