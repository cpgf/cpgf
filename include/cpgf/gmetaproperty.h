#ifndef CPGF_GMETAPROPERTY_H
#define CPGF_GMETAPROPERTY_H

#include "cpgf/private/gmetaproperty_p.h"


namespace cpgf {


class GMetaProperty final : public GMetaAccessible
{
private:
	typedef GMetaAccessible super;

public:
	template <typename Getter, typename Setter, typename Policy>
	GMetaProperty(const char * name, const Getter & getter, const Setter & setter, const Policy &)
		: super(name, meta_internal::createPropertyType<Getter, Setter, Policy>(), mcatProperty), baseData(new meta_internal::GMetaPropertyData<Getter, Setter, Policy>(getter, setter)) {
	}

	virtual bool canGet() const override;
	virtual bool canSet() const override;

	virtual GVariant get(const void * instance) const override;
	virtual void set(void * instance, const GVariant & value) const override;

	virtual void * getAddress(const void * instance) const override;

	virtual size_t getSize() const override;

	virtual const GMetaType & getItemType() const override;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

private:
	std::unique_ptr<meta_internal::GMetaPropertyDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaPropertyDataBase> >
		baseData;
};


} // namespace cpgf


#endif
