// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleSphereEmitter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleSphereEmitter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleSphereEmitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleSphereEmitter, irr::scene::IParticleEmitter> _nd = GDefineMetaClass<irr::scene::IParticleSphereEmitter, irr::scene::IParticleEmitter>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleSphereEmitter");
        buildMetaClass_IParticleSphereEmitter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


