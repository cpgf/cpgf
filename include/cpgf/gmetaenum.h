#ifndef __GMETAENUM_H
#define __GMETAENUM_H

#include "cpgf/private/gmetaenum_p.h"


namespace cpgf {

class GMetaEnum;


GMAKE_FINAL(GMetaEnum)

class GMetaEnum : public GMetaTypedItem, GFINAL_BASE(GMetaEnum)
{
private:
	typedef GMetaTypedItem super;

public:
	GMetaEnum(const char * name, const GMetaType & itemType, meta_internal::GMetaEnumData * baseData);

	virtual size_t getTypeSize() const;
	size_t getCount() const;
	const char * getKey(size_t index) const;
	GVariant getValue(size_t index) const;
	int findKey(const char * key) const;

	GMetaEnum & operator () (const char * key, long long value);

	virtual void * createInstance() const;
	virtual void * createInplace(void * placement) const;
	virtual void * cloneInstance(void * instance) const;
	virtual void * cloneInplace(void * instance, void * placement) const;
	virtual void destroyInstance(void * instance) const;
	
private:
	GScopedPointer<meta_internal::GMetaEnumData> baseData;
};


} // namespace cpgf


#endif
