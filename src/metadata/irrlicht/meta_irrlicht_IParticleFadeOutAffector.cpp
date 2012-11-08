// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleFadeOutAffector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleFadeOutAffector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleFadeOutAffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleFadeOutAffector, irr::scene::IParticleAffector> _nd = GDefineMetaClass<irr::scene::IParticleFadeOutAffector, irr::scene::IParticleAffector>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleFadeOutAffector");
        buildMetaClass_IParticleFadeOutAffector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


