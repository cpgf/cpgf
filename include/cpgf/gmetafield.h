#ifndef __GMETAFIELD_H
#define __GMETAFIELD_H

#include "cpgf/private/gmetafield_p.h"


#define GMETA_FIELD(field) \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &MetaCurrentClassType::field)

#define GMETA_QUALIFIED_FIELD(field) \
	using namespace cpgf; \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &field)


namespace cpgf {


class GMetaField : public GMetaAccessible
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

	virtual GVariant get(void * instance) const;
	virtual void set(void * instance, const GVariant & v) const;

	virtual size_t getSize() const;

	void * getAddress(void * instance) const;
	
	virtual GMetaConverter * createConverter() const;

private:
	GScopedPointer<meta_internal::GMetaFieldDataBase> baseData;
};


void globalAddField(GMetaField * field);

template <typename FT>
void reflectField(const char * name, FT field)
{
	globalAddField(new cpgf::GMetaField(name, field, GMetaPolicyDefault()));
}

template <typename FT, typename Policy>
void reflectField(const char * name, FT field, const Policy & policy)
{
	globalAddField(new cpgf::GMetaField(name, field, policy));
}



} // namespace cpgf



#endif
