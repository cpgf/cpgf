// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IBONESCENENODE_H
#define __META_IRRLICHT_IBONESCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ibonescenenode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("BoneAnimationModeNames", &BoneAnimationModeNames);
    _d.CPGF_MD_TEMPLATE _enum<E_BONE_ANIMATION_MODE>("E_BONE_ANIMATION_MODE")
        ._element("EBAM_AUTOMATIC", irr::scene::EBAM_AUTOMATIC)
        ._element("EBAM_ANIMATED", irr::scene::EBAM_ANIMATED)
        ._element("EBAM_UNANIMATED", irr::scene::EBAM_UNANIMATED)
        ._element("EBAM_COUNT", irr::scene::EBAM_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_BONE_SKINNING_SPACE>("E_BONE_SKINNING_SPACE")
        ._element("EBSS_LOCAL", irr::scene::EBSS_LOCAL)
        ._element("EBSS_GLOBAL", irr::scene::EBSS_GLOBAL)
        ._element("EBSS_COUNT", irr::scene::EBSS_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IBoneSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("positionHint", &D::ClassType::positionHint);
    _d.CPGF_MD_TEMPLATE _field("scaleHint", &D::ClassType::scaleHint);
    _d.CPGF_MD_TEMPLATE _field("rotationHint", &D::ClassType::rotationHint);
    _d.CPGF_MD_TEMPLATE _method("getBoneName", &D::ClassType::getBoneName);
    _d.CPGF_MD_TEMPLATE _method("getBoneIndex", &D::ClassType::getBoneIndex);
    _d.CPGF_MD_TEMPLATE _method("setAnimationMode", &D::ClassType::setAnimationMode);
    _d.CPGF_MD_TEMPLATE _method("getAnimationMode", &D::ClassType::getAnimationMode);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("OnAnimate", &D::ClassType::OnAnimate);
    _d.CPGF_MD_TEMPLATE _method("render", &D::ClassType::render);
    _d.CPGF_MD_TEMPLATE _method("setSkinningSpace", &D::ClassType::setSkinningSpace);
    _d.CPGF_MD_TEMPLATE _method("getSkinningSpace", &D::ClassType::getSkinningSpace);
    _d.CPGF_MD_TEMPLATE _method("updateAbsolutePositionOfAllChildren", &D::ClassType::updateAbsolutePositionOfAllChildren);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
