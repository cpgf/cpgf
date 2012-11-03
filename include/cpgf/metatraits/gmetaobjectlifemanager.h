#ifndef __GMETAOBJECTLIFEMANAGER_H
#define __GMETAOBJECTLIFEMANAGER_H

#include "cpgf/gapi.h"


namespace cpgf {

struct GMetaTraitsParam;
struct IMetaClass;

struct IMetaObjectLifeManager : public IObject
{
	virtual void G_API_CC retainObject(void * object) = 0;
	virtual void G_API_CC releaseObject(void * object) = 0;
	virtual void G_API_CC freeObject(void * object, IMetaClass * metaClass) = 0;
	virtual void G_API_CC returnedFromMethod(void * object) = 0;
};


namespace metatraits_internal {
	IMetaObjectLifeManager * createDefaultObjectLifeManagerFromMetaTraits();
} // namespace metatraits_internal


} // namespace cpgf


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateObjectLifeManager
{
	static cpgf::IMetaObjectLifeManager * createObjectLifeManager(const cpgf::GMetaTraitsParam &) {
		return NULL;
	}
};

inline cpgf::IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const cpgf::GMetaTraitsParam & /*param*/, ...)
{
	return NULL;
}

template <typename T>
cpgf::IMetaObjectLifeManager * createObjectLifeManagerFromMetaTraits(const cpgf::GMetaTraitsParam & param, T * p)
{
	cpgf::IMetaObjectLifeManager * objectLifeManager = metaTraitsCreateObjectLifeManager(param, p);
	if(objectLifeManager == NULL) {
		objectLifeManager = GMetaTraitsCreateObjectLifeManager<T>::createObjectLifeManager(param);
	}
	if(objectLifeManager == NULL) {
		objectLifeManager = cpgf::metatraits_internal::createDefaultObjectLifeManagerFromMetaTraits();
	}
	return objectLifeManager;
}


} // namespace cpgf_metatraits


#endif
