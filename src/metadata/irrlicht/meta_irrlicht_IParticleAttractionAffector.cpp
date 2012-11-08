// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleAttractionAffector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleAttractionAffector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleAttractionAffector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleAttractionAffector, irr::scene::IParticleAffector> _nd = GDefineMetaClass<irr::scene::IParticleAttractionAffector, irr::scene::IParticleAffector>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleAttractionAffector");
        buildMetaClass_IParticleAttractionAffector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


