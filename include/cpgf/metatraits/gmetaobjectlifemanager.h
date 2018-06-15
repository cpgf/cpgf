#ifndef CPGF_GMETAOBJECTLIFEMANAGER_H
#define CPGF_GMETAOBJECTLIFEMANAGER_H

#include "cpgf/gapi.h"

#include <type_traits>
#include <cstddef>

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


namespace cpgf {

template <typename T, typename Enabled = void>
struct GMetaTraitsCreateObjectLifeManager
{
	static IMetaObjectLifeManager * createObjectLifeManager(const GMetaTraitsParam &) {
		return nullptr;
	}
};

template <typename T>
IMetaObjectLifeManager * createObjectLifeManagerFromMetaTraits(const GMetaTraitsParam & param, T *)
{
	IMetaObjectLifeManager * objectLifeManager = GMetaTraitsCreateObjectLifeManager<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::createObjectLifeManager(param);
	if(objectLifeManager == nullptr) {
		objectLifeManager = metatraits_internal::createDefaultObjectLifeManagerFromMetaTraits();
	}
	return objectLifeManager;
}

} // namespace cpgf


#endif
