// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleBoxEmitter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleBoxEmitter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleBoxEmitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleBoxEmitter, irr::scene::IParticleEmitter> _nd = GDefineMetaClass<irr::scene::IParticleBoxEmitter, irr::scene::IParticleEmitter>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleBoxEmitter");
        buildMetaClass_IParticleBoxEmitter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


