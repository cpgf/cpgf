#ifndef CPGF_GMETAENUM_H
#define CPGF_GMETAENUM_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

class GMetaEnum;

class GMetaEnumDataImplement;

GMAKE_FINAL(GMetaEnum)

class GMetaEnum : public GMetaTypedItem, GFINAL_BASE(GMetaEnum)
{
private:
	typedef GMetaTypedItem super;

public:
	GMetaEnum(const char * name, const GMetaType & itemType);
	virtual ~GMetaEnum();

	virtual size_t getTypeSize() const;
	size_t getCount() const;
	const char * getKey(size_t index) const;
	GVariant getValue(size_t index) const;
	int findKey(const char * key) const;

	GMetaEnum & operator () (const char * key, const GVariant & value);

	virtual void * createInstance() const;
	virtual void * createInplace(void * placement) const;
	virtual void * cloneInstance(const void * instance) const;
	virtual void * cloneInplace(const void * instance, void * placement) const;
	virtual void destroyInstance(void * instance) const;
	virtual void destroyInplace(void * instance) const;
	
	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

private:
	void addEnum(const char * key, const GVariant & value);

private:
	GScopedPointer<GMetaEnumDataImplement> implement;
};


} // namespace cpgf


#endif
