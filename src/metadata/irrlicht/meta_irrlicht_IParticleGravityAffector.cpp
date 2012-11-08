// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleGravityAffector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleGravityAffector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleGravityAffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleGravityAffector, irr::scene::IParticleAffector> _nd = GDefineMetaClass<irr::scene::IParticleGravityAffector, irr::scene::IParticleAffector>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleGravityAffector");
        buildMetaClass_IParticleGravityAffector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


