#ifndef CPGF_GMETAFIELD_H
#define CPGF_GMETAFIELD_H

#include "cpgf/private/gmetafield_p.h"
#include "cpgf/accessor/gaccessor.h"

namespace cpgf {

class GMetaField final : public GMetaAccessible
{
private:
	typedef GMetaAccessible super;

public:
	template <typename OT, typename FT, typename Policy>
	GMetaField(const char * name, FT OT::* field, const Policy & policy)
		:
			super(name, createMetaType<FT>(), mcatField),
			baseData(
				new meta_internal::GMetaFieldDataAccessor<decltype(createInstanceAccessor(field, field, policy))>(
					createInstanceAccessor(field, field, policy)
				)
			)
	{
	}

	template <typename FT, typename Policy>
	GMetaField(const char * name, FT * field, const Policy & policy)
		:
			super(name, createMetaType<FT>(), mcatField),
			baseData(
				new meta_internal::GMetaFieldDataAccessor<decltype(createInstanceAccessor(field, field, policy))>(
					createInstanceAccessor(field, field, policy)
				)
			)
	{
		this->addModifier(metaModifierStatic);
	}

	virtual bool canGet() const override;
	virtual bool canSet() const override;

	virtual GVariant get(const void * instance) const override;
	virtual void set(void * instance, const GVariant & v) const override;

	virtual void * getAddress(const void * instance) const override;

	virtual size_t getSize() const override;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

private:
	std::unique_ptr<meta_internal::GMetaFieldDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaFieldDataBase> >
		baseData;
};



} // namespace cpgf



#endif
