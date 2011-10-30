#ifndef __GMETAPROPERTY_H
#define __GMETAPROPERTY_H

#include "cpgf/private/gmetaproperty_p.h"


#define GMETA_PROPERTY(prop, getter, setter) \
	reflectProperty(cpgf::normalizeReflectName(GPP_STRINGIZE(prop)).c_str(), &MetaCurrentClassType::getter, &MetaCurrentClassType::setter)

#define GMETA_QUALIFIED_PROPERTY(prop, getter, setter) \
	using namespace cpgf; \
	reflectProperty(cpgf::normalizeReflectName(GPP_STRINGIZE(prop)).c_str(), &getter, &setter)


namespace cpgf {


class GMetaProperty : public GMetaAccessible
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


void globalAddProperty(GMetaProperty * field);

template <typename Getter, typename Setter>
void reflectProperty(const char * name, const Getter & getter, const Setter & setter)
{
	globalAddProperty(new cpgf::GMetaProperty(name, getter, setter, GMetaPolicyDefault()));
}

template <typename Getter, typename Setter, typename Policy>
void reflectProperty(const char * name, const Getter & getter, const Setter & setter, const Policy & policy)
{
	globalAddProperty(new cpgf::GMetaProperty(name, getter, setter, policy));
}


} // namespace cpgf


#endif
