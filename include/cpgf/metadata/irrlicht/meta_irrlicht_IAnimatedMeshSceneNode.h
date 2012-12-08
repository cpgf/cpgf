// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IANIMATEDMESHSCENENODE_H
#define __META_IRRLICHT_IANIMATEDMESHSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ianimatedmeshscenenode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_JOINT_UPDATE_ON_RENDER>("E_JOINT_UPDATE_ON_RENDER")
        ._element("EJUOR_NONE", irr::scene::EJUOR_NONE)
        ._element("EJUOR_READ", irr::scene::EJUOR_READ)
        ._element("EJUOR_CONTROL", irr::scene::EJUOR_CONTROL)
    ;
}


template <typename D>
void buildMetaClass_IAnimatedMeshSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setCurrentFrame", &D::ClassType::setCurrentFrame);
    _d.CPGF_MD_TEMPLATE _method("setFrameLoop", &D::ClassType::setFrameLoop);
    _d.CPGF_MD_TEMPLATE _method("setAnimationSpeed", &D::ClassType::setAnimationSpeed);
    _d.CPGF_MD_TEMPLATE _method("getAnimationSpeed", &D::ClassType::getAnimationSpeed);
    _d.CPGF_MD_TEMPLATE _method("addShadowVolumeSceneNode", &D::ClassType::addShadowVolumeSceneNode)
        ._default(copyVariantFromCopyable(1000.0f))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getJointNode", (IBoneSceneNode * (D::ClassType::*) (const c8 *))&D::ClassType::getJointNode);
    _d.CPGF_MD_TEMPLATE _method("getJointNode", (IBoneSceneNode * (D::ClassType::*) (u32))&D::ClassType::getJointNode);
    _d.CPGF_MD_TEMPLATE _method("getJointCount", &D::ClassType::getJointCount);
    _d.CPGF_MD_TEMPLATE _method("setMD2Animation", (bool (D::ClassType::*) (EMD2_ANIMATION_TYPE))&D::ClassType::setMD2Animation);
    _d.CPGF_MD_TEMPLATE _method("setMD2Animation", (bool (D::ClassType::*) (const c8 *))&D::ClassType::setMD2Animation);
    _d.CPGF_MD_TEMPLATE _method("getFrameNr", &D::ClassType::getFrameNr);
    _d.CPGF_MD_TEMPLATE _method("getStartFrame", &D::ClassType::getStartFrame);
    _d.CPGF_MD_TEMPLATE _method("getEndFrame", &D::ClassType::getEndFrame);
    _d.CPGF_MD_TEMPLATE _method("setLoopMode", &D::ClassType::setLoopMode);
    _d.CPGF_MD_TEMPLATE _method("getLoopMode", &D::ClassType::getLoopMode);
    _d.CPGF_MD_TEMPLATE _method("setAnimationEndCallback", &D::ClassType::setAnimationEndCallback)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("setReadOnlyMaterials", &D::ClassType::setReadOnlyMaterials);
    _d.CPGF_MD_TEMPLATE _method("isReadOnlyMaterials", &D::ClassType::isReadOnlyMaterials);
    _d.CPGF_MD_TEMPLATE _method("setMesh", &D::ClassType::setMesh);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("getMD3TagTransformation", &D::ClassType::getMD3TagTransformation, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setJointMode", &D::ClassType::setJointMode);
    _d.CPGF_MD_TEMPLATE _method("setTransitionTime", &D::ClassType::setTransitionTime);
    _d.CPGF_MD_TEMPLATE _method("animateJoints", &D::ClassType::animateJoints)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setRenderFromIdentity", &D::ClassType::setRenderFromIdentity);
    _d.CPGF_MD_TEMPLATE _method("clone", &D::ClassType::clone)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
}


template <typename D>
void buildMetaClass_IAnimationEndCallBack(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnAnimationEnd", &D::ClassType::OnAnimationEnd);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
