// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGEOMETRYCREATOR_H
#define __META_IRRLICHT_IGEOMETRYCREATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGeometryCreator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("createCubeMesh", &D::ClassType::createCubeMesh)
        ._default(copyVariantFromCopyable(core::vector3df(5.f, 5.f, 5.f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createHillPlaneMesh", &D::ClassType::createHillPlaneMesh);
    _d.CPGF_MD_TEMPLATE _method("createPlaneMesh", &D::ClassType::createPlaneMesh)
        ._default(copyVariantFromCopyable(core::dimension2df(1.f, 1.f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(core::dimension2du(1, 1)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createTerrainMesh", &D::ClassType::createTerrainMesh)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createArrowMesh", &D::ClassType::createArrowMesh)
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0.3f))
        ._default(copyVariantFromCopyable(0.05f))
        ._default(copyVariantFromCopyable(0.6f))
        ._default(copyVariantFromCopyable(1.f))
        ._default(copyVariantFromCopyable(8))
        ._default(copyVariantFromCopyable(4))
    ;
    _d.CPGF_MD_TEMPLATE _method("createSphereMesh", &D::ClassType::createSphereMesh)
        ._default(copyVariantFromCopyable(16))
        ._default(copyVariantFromCopyable(16))
        ._default(copyVariantFromCopyable(5.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("createCylinderMesh", &D::ClassType::createCylinderMesh)
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(video::SColor(0xffffffff)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createConeMesh", &D::ClassType::createConeMesh)
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(video::SColor(0xffffffff)))
        ._default(copyVariantFromCopyable(video::SColor(0xffffffff)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createVolumeLightMesh", &D::ClassType::createVolumeLightMesh)
        ._default(copyVariantFromCopyable(core::vector3df(1.f, 1.2f, 1.f)))
        ._default(copyVariantFromCopyable(8.f))
        ._default(copyVariantFromCopyable(0xffffffff))
        ._default(copyVariantFromCopyable(0xffffffff))
        ._default(copyVariantFromCopyable(32))
        ._default(copyVariantFromCopyable(32))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
