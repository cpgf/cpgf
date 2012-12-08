// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IPARTICLESYSTEMSCENENODE_H
#define __META_IRRLICHT_IPARTICLESYSTEMSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IParticleSystemSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setParticleSize", &D::ClassType::setParticleSize)
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(5.0f, 5.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("setParticlesAreGlobal", &D::ClassType::setParticlesAreGlobal)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("clearParticles", &D::ClassType::clearParticles);
    _d.CPGF_MD_TEMPLATE _method("doParticleSystem", &D::ClassType::doParticleSystem);
    _d.CPGF_MD_TEMPLATE _method("getEmitter", &D::ClassType::getEmitter);
    _d.CPGF_MD_TEMPLATE _method("setEmitter", &D::ClassType::setEmitter);
    _d.CPGF_MD_TEMPLATE _method("addAffector", &D::ClassType::addAffector);
    _d.CPGF_MD_TEMPLATE _method("getAffectors", &D::ClassType::getAffectors);
    _d.CPGF_MD_TEMPLATE _method("removeAllAffectors", &D::ClassType::removeAllAffectors);
    _d.CPGF_MD_TEMPLATE _method("createAnimatedMeshSceneNodeEmitter", &D::ClassType::createAnimatedMeshSceneNodeEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(100.0f))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("createBoxEmitter", &D::ClassType::createBoxEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
        ._default(copyVariantFromCopyable(core::aabbox3df(-10, 28,-10, 10, 30, 10)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createCylinderEmitter", &D::ClassType::createCylinderEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createMeshEmitter", &D::ClassType::createMeshEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(100.0f))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("createPointEmitter", &D::ClassType::createPointEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createRingEmitter", &D::ClassType::createRingEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createSphereEmitter", &D::ClassType::createSphereEmitter)
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(core::dimension2df(5.0f, 5.0f)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(4000))
        ._default(copyVariantFromCopyable(2000))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.03f, 0.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createAttractionAffector", &D::ClassType::createAttractionAffector)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("createScaleParticleAffector", &D::ClassType::createScaleParticleAffector)
        ._default(copyVariantFromCopyable(core::dimension2df(1.0f, 1.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createFadeOutParticleAffector", &D::ClassType::createFadeOutParticleAffector)
        ._default(copyVariantFromCopyable(1000))
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createGravityAffector", &D::ClassType::createGravityAffector)
        ._default(copyVariantFromCopyable(1000))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f,-0.03f, 0.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createRotationAffector", &D::ClassType::createRotationAffector)
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(5.0f, 5.0f, 5.0f)))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
