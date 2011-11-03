#ifndef __GMETAPROPERTY_H
#define __GMETAPROPERTY_H

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

	virtual GVariant get(void * instance) const;
	virtual void set(void * instance, const GVariant & value) const;

	virtual size_t getSize() const;

	virtual const GMetaType & getItemType() const;

	virtual GMetaConverter * createConverter() const;

private:
	GScopedPointer<meta_internal::GMetaPropertyDataBase> baseData;
};


} // namespace cpgf


#endif
