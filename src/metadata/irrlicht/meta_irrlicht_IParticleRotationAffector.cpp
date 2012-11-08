// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleRotationAffector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleRotationAffector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleRotationAffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleRotationAffector, irr::scene::IParticleAffector> _nd = GDefineMetaClass<irr::scene::IParticleRotationAffector, irr::scene::IParticleAffector>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleRotationAffector");
        buildMetaClass_IParticleRotationAffector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


