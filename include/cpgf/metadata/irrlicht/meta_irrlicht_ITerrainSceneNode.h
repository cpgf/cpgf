// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ITERRAINSCENENODE_H
#define __META_IRRLICHT_ITERRAINSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ITerrainSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", (const core::aabbox3d< f32 > & (D::ClassType::*) () const)&D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", (const core::aabbox3d< f32 > & (D::ClassType::*) (s32, s32) const)&D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getIndexCount", &D::ClassType::getIndexCount);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("getRenderBuffer", &D::ClassType::getRenderBuffer);
    _d.CPGF_MD_TEMPLATE _method("getMeshBufferForLOD", &D::ClassType::getMeshBufferForLOD)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getIndicesForPatch", &D::ClassType::getIndicesForPatch)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getCurrentLODOfPatches", &D::ClassType::getCurrentLODOfPatches);
    _d.CPGF_MD_TEMPLATE _method("setLODOfPatch", &D::ClassType::setLODOfPatch)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getTerrainCenter", &D::ClassType::getTerrainCenter);
    _d.CPGF_MD_TEMPLATE _method("getHeight", &D::ClassType::getHeight);
    _d.CPGF_MD_TEMPLATE _method("setCameraMovementDelta", &D::ClassType::setCameraMovementDelta);
    _d.CPGF_MD_TEMPLATE _method("setCameraRotationDelta", &D::ClassType::setCameraRotationDelta);
    _d.CPGF_MD_TEMPLATE _method("setDynamicSelectorUpdate", &D::ClassType::setDynamicSelectorUpdate);
    _d.CPGF_MD_TEMPLATE _method("overrideLODDistance", &D::ClassType::overrideLODDistance);
    _d.CPGF_MD_TEMPLATE _method("scaleTexture", &D::ClassType::scaleTexture)
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadHeightMap", &D::ClassType::loadHeightMap)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadHeightMapRAW", &D::ClassType::loadHeightMapRAW)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(16))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
