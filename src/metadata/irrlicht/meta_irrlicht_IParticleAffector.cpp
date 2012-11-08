// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleAffector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleAffector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iparticleaffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iparticleaffector(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IParticleAffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleAffector, irr::io::IAttributeExchangingObject> _nd = GDefineMetaClass<irr::scene::IParticleAffector, irr::io::IAttributeExchangingObject>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleAffector");
        buildMetaClass_IParticleAffector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


