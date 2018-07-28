#ifndef CPGF_GMETAENUM_H
#define CPGF_GMETAENUM_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

class GMetaEnum;

class GMetaEnumDataImplement;

class GMetaEnum final : public GMetaTypedItem
{
private:
	typedef GMetaTypedItem super;

public:
	GMetaEnum(const char * name, const GMetaType & itemType);
	virtual ~GMetaEnum();

	virtual size_t getTypeSize() const override;
	size_t getCount() const;
	const char * getKey(size_t index) const;
	GVariant getValue(size_t index) const;
	int findKey(const char * key) const;

	GMetaEnum & operator () (const char * key, const GVariant & value);

	virtual void * createInstance() const override;
	virtual void * createInplace(void * placement) const override;
	virtual void * cloneInstance(const void * instance) const override;
	virtual void * cloneInplace(const void * instance, void * placement) const override;
	virtual void destroyInstance(void * instance) const override;
	virtual void destroyInplace(void * instance) const override;
	
	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

private:
	void addEnum(const char * key, const GVariant & value);

private:
	std::unique_ptr<GMetaEnumDataImplement> implement;
};


} // namespace cpgf


#endif
