// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IANIMATEDMESH_H
#define __META_IRRLICHT_IANIMATEDMESH_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ianimatedmesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_ANIMATED_MESH_TYPE>("E_ANIMATED_MESH_TYPE")
        ._element("EAMT_UNKNOWN", irr::scene::EAMT_UNKNOWN)
        ._element("EAMT_MD2", irr::scene::EAMT_MD2)
        ._element("EAMT_MD3", irr::scene::EAMT_MD3)
        ._element("EAMT_OBJ", irr::scene::EAMT_OBJ)
        ._element("EAMT_BSP", irr::scene::EAMT_BSP)
        ._element("EAMT_3DS", irr::scene::EAMT_3DS)
        ._element("EAMT_MY3D", irr::scene::EAMT_MY3D)
        ._element("EAMT_LMTS", irr::scene::EAMT_LMTS)
        ._element("EAMT_CSM", irr::scene::EAMT_CSM)
        ._element("EAMT_OCT", irr::scene::EAMT_OCT)
        ._element("EAMT_MDL_HALFLIFE", irr::scene::EAMT_MDL_HALFLIFE)
        ._element("EAMT_SKINNED", irr::scene::EAMT_SKINNED)
    ;
}


template <typename D>
void buildMetaClass_IAnimatedMesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getFrameCount", &D::ClassType::getFrameCount);
    _d.CPGF_MD_TEMPLATE _method("getAnimationSpeed", &D::ClassType::getAnimationSpeed);
    _d.CPGF_MD_TEMPLATE _method("setAnimationSpeed", &D::ClassType::setAnimationSpeed);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(255))
    ;
    _d.CPGF_MD_TEMPLATE _method("getMeshType", &D::ClassType::getMeshType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
