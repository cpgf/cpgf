#ifndef CPGF_GMETAFUNDAMENTAL_H
#define CPGF_GMETAFUNDAMENTAL_H

#include "cpgf/private/gmetafundamental_p.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

class GMetaFundamental final : public GMetaTypedItem
{
private:
	typedef GMetaTypedItem super;

public:
	template <typename T>
	GMetaFundamental(T *, const char * name)
		: super(name, createMetaType<T>(), mcatFundamental) {
		new (this->dataBuffer) meta_internal::GMetaFundamentalDataImplement<T>;
	}

	GVariantType getVariantType() const;
	GVariant getValue(const void * instance) const;

	virtual size_t getTypeSize() const override;
	virtual void * createInstance() const override;
	virtual void * createInplace(void * placement) const override;
	virtual void * cloneInstance(const void * instance) const override;
	virtual void * cloneInplace(const void * instance, void * placement) const override;
	virtual void destroyInstance(void * instance) const override;
	virtual void destroyInplace(void * instance) const override;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

private:
	const meta_internal::GMetaFundamentalData * getData() const;

private:
	char dataBuffer[sizeof(meta_internal::GMetaFundamentalDataImplement<int>)];
};




} // namespace cpgf




#endif
