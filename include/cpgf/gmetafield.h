#ifndef __GMETAFIELD_H
#define __GMETAFIELD_H

#include "cpgf/private/gmetafield_p.h"


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



} // namespace cpgf



#endif
