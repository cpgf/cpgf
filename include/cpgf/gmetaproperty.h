#ifndef CPGF_GMETAPROPERTY_H
#define CPGF_GMETAPROPERTY_H

#include "cpgf/private/gmetaproperty_p.h"


namespace cpgf {


GMAKE_FINAL(GMetaProperty)

class GMetaProperty : public GMetaAccessible, GFINAL_BASE(GMetaProperty)
{
private:
	typedef GMetaAccessible super;

public:
	template <typename Getter, typename Setter, typename Policy>
	GMetaProperty(const char * name, const Getter & getter, const Setter & setter, const Policy &)
		: super(name, meta_internal::createPropertyType<Getter, Setter, Policy>(), mcatProperty), baseData(new meta_internal::GMetaPropertyData<Getter, Setter, Policy>(getter, setter)) {
	}

	virtual bool canGet() const;
	virtual bool canSet() const;

	virtual GVariant get(const void * instance) const;
	virtual void set(void * instance, const GVariant & value) const;

	virtual void * getAddress(const void * instance) const;

	virtual size_t getSize() const;

	virtual const GMetaType & getItemType() const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

private:
	GScopedPointer<meta_internal::GMetaPropertyDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaPropertyDataBase> >
		baseData;
};


} // namespace cpgf


#endif
