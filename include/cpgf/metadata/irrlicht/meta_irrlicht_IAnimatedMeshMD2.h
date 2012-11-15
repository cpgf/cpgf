// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IANIMATEDMESHMD2_H
#define __META_IRRLICHT_IANIMATEDMESHMD2_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ianimatedmeshmd2(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EMD2_ANIMATION_TYPE>("EMD2_ANIMATION_TYPE")
        ._element("EMAT_STAND", irr::scene::EMAT_STAND)
        ._element("EMAT_RUN", irr::scene::EMAT_RUN)
        ._element("EMAT_ATTACK", irr::scene::EMAT_ATTACK)
        ._element("EMAT_PAIN_A", irr::scene::EMAT_PAIN_A)
        ._element("EMAT_PAIN_B", irr::scene::EMAT_PAIN_B)
        ._element("EMAT_PAIN_C", irr::scene::EMAT_PAIN_C)
        ._element("EMAT_JUMP", irr::scene::EMAT_JUMP)
        ._element("EMAT_FLIP", irr::scene::EMAT_FLIP)
        ._element("EMAT_SALUTE", irr::scene::EMAT_SALUTE)
        ._element("EMAT_FALLBACK", irr::scene::EMAT_FALLBACK)
        ._element("EMAT_WAVE", irr::scene::EMAT_WAVE)
        ._element("EMAT_POINT", irr::scene::EMAT_POINT)
        ._element("EMAT_CROUCH_STAND", irr::scene::EMAT_CROUCH_STAND)
        ._element("EMAT_CROUCH_WALK", irr::scene::EMAT_CROUCH_WALK)
        ._element("EMAT_CROUCH_ATTACK", irr::scene::EMAT_CROUCH_ATTACK)
        ._element("EMAT_CROUCH_PAIN", irr::scene::EMAT_CROUCH_PAIN)
        ._element("EMAT_CROUCH_DEATH", irr::scene::EMAT_CROUCH_DEATH)
        ._element("EMAT_DEATH_FALLBACK", irr::scene::EMAT_DEATH_FALLBACK)
        ._element("EMAT_DEATH_FALLFORWARD", irr::scene::EMAT_DEATH_FALLFORWARD)
        ._element("EMAT_DEATH_FALLBACKSLOW", irr::scene::EMAT_DEATH_FALLBACKSLOW)
        ._element("EMAT_BOOM", irr::scene::EMAT_BOOM)
        ._element("EMAT_COUNT", irr::scene::EMAT_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IAnimatedMeshMD2(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getFrameLoop", (void (D::ClassType::*) (EMD2_ANIMATION_TYPE, s32 &, s32 &, s32 &) const)&D::ClassType::getFrameLoop);
    _d.CPGF_MD_TEMPLATE _method("getFrameLoop", (bool (D::ClassType::*) (const c8 *, s32 &, s32 &, s32 &) const)&D::ClassType::getFrameLoop);
    _d.CPGF_MD_TEMPLATE _method("getAnimationCount", &D::ClassType::getAnimationCount);
    _d.CPGF_MD_TEMPLATE _method("getAnimationName", &D::ClassType::getAnimationName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
