// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENECOLLISIONMANAGER_H
#define __META_IRRLICHT_ISCENECOLLISIONMANAGER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneCollisionManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getCollisionPoint", &D::ClassType::getCollisionPoint);
    _d.CPGF_MD_TEMPLATE _method("getCollisionResultPosition", &D::ClassType::getCollisionResultPosition)
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(0.0005f))
    ;
    _d.CPGF_MD_TEMPLATE _method("getRayFromScreenCoordinates", &D::ClassType::getRayFromScreenCoordinates)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getScreenCoordinatesFrom3DPosition", &D::ClassType::getScreenCoordinatesFrom3DPosition)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromScreenCoordinatesBB", &D::ClassType::getSceneNodeFromScreenCoordinatesBB)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromRayBB", &D::ClassType::getSceneNodeFromRayBB)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromCameraBB", &D::ClassType::getSceneNodeFromCameraBB)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeAndCollisionPointFromRay", &D::ClassType::getSceneNodeAndCollisionPointFromRay)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
