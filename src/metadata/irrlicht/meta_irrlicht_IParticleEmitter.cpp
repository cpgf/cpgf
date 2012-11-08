// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleEmitter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleEmitter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iparticleemitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iparticleemitter(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IParticleEmitter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleEmitter, irr::io::IAttributeExchangingObject> _nd = GDefineMetaClass<irr::scene::IParticleEmitter, irr::io::IAttributeExchangingObject>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleEmitter");
        buildMetaClass_IParticleEmitter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


