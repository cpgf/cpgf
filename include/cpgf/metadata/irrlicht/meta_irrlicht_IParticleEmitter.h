// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IPARTICLEEMITTER_H
#define __META_IRRLICHT_IPARTICLEEMITTER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iparticleemitter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("ParticleEmitterTypeNames", &ParticleEmitterTypeNames);
    _d.CPGF_MD_TEMPLATE _enum<E_PARTICLE_EMITTER_TYPE>("E_PARTICLE_EMITTER_TYPE")
        ._element("EPET_POINT", irr::scene::EPET_POINT)
        ._element("EPET_ANIMATED_MESH", irr::scene::EPET_ANIMATED_MESH)
        ._element("EPET_BOX", irr::scene::EPET_BOX)
        ._element("EPET_CYLINDER", irr::scene::EPET_CYLINDER)
        ._element("EPET_MESH", irr::scene::EPET_MESH)
        ._element("EPET_RING", irr::scene::EPET_RING)
        ._element("EPET_SPHERE", irr::scene::EPET_SPHERE)
        ._element("EPET_COUNT", irr::scene::EPET_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IParticleEmitter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("emitt", &D::ClassType::emitt);
    _d.CPGF_MD_TEMPLATE _method("setDirection", &D::ClassType::setDirection);
    _d.CPGF_MD_TEMPLATE _method("setMinParticlesPerSecond", &D::ClassType::setMinParticlesPerSecond);
    _d.CPGF_MD_TEMPLATE _method("setMaxParticlesPerSecond", &D::ClassType::setMaxParticlesPerSecond);
    _d.CPGF_MD_TEMPLATE _method("setMinStartColor", &D::ClassType::setMinStartColor);
    _d.CPGF_MD_TEMPLATE _method("setMaxStartColor", &D::ClassType::setMaxStartColor);
    _d.CPGF_MD_TEMPLATE _method("setMaxStartSize", &D::ClassType::setMaxStartSize);
    _d.CPGF_MD_TEMPLATE _method("setMinStartSize", &D::ClassType::setMinStartSize);
    _d.CPGF_MD_TEMPLATE _method("setMinLifeTime", &D::ClassType::setMinLifeTime);
    _d.CPGF_MD_TEMPLATE _method("setMaxLifeTime", &D::ClassType::setMaxLifeTime);
    _d.CPGF_MD_TEMPLATE _method("setMaxAngleDegrees", &D::ClassType::setMaxAngleDegrees);
    _d.CPGF_MD_TEMPLATE _method("getDirection", &D::ClassType::getDirection);
    _d.CPGF_MD_TEMPLATE _method("getMinParticlesPerSecond", &D::ClassType::getMinParticlesPerSecond);
    _d.CPGF_MD_TEMPLATE _method("getMaxParticlesPerSecond", &D::ClassType::getMaxParticlesPerSecond);
    _d.CPGF_MD_TEMPLATE _method("getMinStartColor", &D::ClassType::getMinStartColor);
    _d.CPGF_MD_TEMPLATE _method("getMaxStartColor", &D::ClassType::getMaxStartColor);
    _d.CPGF_MD_TEMPLATE _method("getMaxStartSize", &D::ClassType::getMaxStartSize);
    _d.CPGF_MD_TEMPLATE _method("getMinStartSize", &D::ClassType::getMinStartSize);
    _d.CPGF_MD_TEMPLATE _method("getMinLifeTime", &D::ClassType::getMinLifeTime);
    _d.CPGF_MD_TEMPLATE _method("getMaxLifeTime", &D::ClassType::getMaxLifeTime);
    _d.CPGF_MD_TEMPLATE _method("getMaxAngleDegrees", &D::ClassType::getMaxAngleDegrees);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
