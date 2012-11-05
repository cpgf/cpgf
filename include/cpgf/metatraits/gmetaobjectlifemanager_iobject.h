#ifndef __GMETAOBJECTLIFEMANAGER_IOBJECT_H
#define __GMETAOBJECTLIFEMANAGER_IOBJECT_H

#include "cpgf/metatraits/gmetaobjectlifemanager.h"
#include "cpgf/gtypeutil.h"
#include "cpgf/genableif.h"


namespace cpgf {

struct IObject;
struct GMetaTraitsParam;
struct IMetaClass;
struct IMetaObjectLifeManager;

} // namespace cpgf


namespace cpgf {

namespace metatraits_internal {

	IMetaObjectLifeManager * doCreateObjectLifeManagerForIObject(const GTypeConverterCallback & callback);

} // namespace metatraits_internal

template <typename T>
struct GMetaTraitsCreateObjectLifeManager <T, typename GEnableIfResult<IsConvertible<T *, IObject *> >::Result>
{
	static IMetaObjectLifeManager * createObjectLifeManager(const GMetaTraitsParam &) {
		return metatraits_internal::doCreateObjectLifeManagerForIObject(GTypeConverter<T *, IObject *>());
	}
};

} // namespace cpgf



#endif
