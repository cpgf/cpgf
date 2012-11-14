#ifndef __GMETAOBJECTLIFEMANAGER_IRRLICHT_IREFERENCECOUNTED_H
#define __GMETAOBJECTLIFEMANAGER_IRRLICHT_IREFERENCECOUNTED_H


#include "IReferenceCounted.h"

#include "cpgf/metatraits/gmetaobjectlifemanager.h"
#include "cpgf/genableif.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypeutil.h"


namespace cpgf {

struct IMetaObjectLifeManager;
struct GMetaTraitsParam;

namespace irrlicht_internal {

IMetaObjectLifeManager * doCreateObjectLifeManagerForIrrReferenceCounted(const GTypeConverterCallback & caster);

} // namespace irrlicht_internal

template <typename T>
struct GMetaTraitsCreateObjectLifeManager <T, typename GEnableIfResult<IsConvertible<T *, irr::IReferenceCounted *> >::Result>
{
	static IMetaObjectLifeManager * createObjectLifeManager(const GMetaTraitsParam &) {
		return irrlicht_internal::doCreateObjectLifeManagerForIrrReferenceCounted(GTypeConverter<T *, irr::IReferenceCounted *>());
	}
};

} // namespace cpgf



#endif

