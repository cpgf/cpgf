// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleMeshEmitter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleMeshEmitter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleMeshEmitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleMeshEmitter, irr::scene::IParticleEmitter> _nd = GDefineMetaClass<irr::scene::IParticleMeshEmitter, irr::scene::IParticleEmitter>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleMeshEmitter");
        buildMetaClass_IParticleMeshEmitter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


