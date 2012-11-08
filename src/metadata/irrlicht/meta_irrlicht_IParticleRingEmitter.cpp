// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleRingEmitter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleRingEmitter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleRingEmitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleRingEmitter, irr::scene::IParticleEmitter> _nd = GDefineMetaClass<irr::scene::IParticleRingEmitter, irr::scene::IParticleEmitter>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleRingEmitter");
        buildMetaClass_IParticleRingEmitter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


