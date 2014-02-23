#ifndef CPGF_GMETAFIELD_H
#define CPGF_GMETAFIELD_H

#include "cpgf/private/gmetafield_p.h"


namespace cpgf {


GMAKE_FINAL(GMetaField)

class GMetaField : public GMetaAccessible, GFINAL_BASE(GMetaField)
{
private:
	typedef GMetaAccessible super;

public:
	template <typename OT, typename FT, typename Policy>
	GMetaField(const char * name, FT OT::* field, const Policy &)
		: super(name, createMetaType<FT>(), mcatField), baseData(new meta_internal::GMetaFieldDataMember<OT, FT, Policy>(field)) {
	}

	template <typename FT, typename Policy>
	GMetaField(const char * name, FT * field, const Policy &)
		: super(name, createMetaType<FT>(), mcatField), baseData(new meta_internal::GMetaFieldDataGlobal<FT, Policy>(field)) {
		this->addModifier(metaModifierStatic);
	}

	virtual bool canGet() const;
	virtual bool canSet() const;

	virtual GVariant get(const void * instance) const;
	virtual void set(void * instance, const GVariant & v) const;

	virtual void * getAddress(const void * instance) const;

	virtual size_t getSize() const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

private:
	GScopedPointer<meta_internal::GMetaFieldDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaFieldDataBase> >
		baseData;
};



} // namespace cpgf



#endif
