// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IRandomizer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IRandomizer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IRandomizer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::IRandomizer, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::IRandomizer, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IRandomizer");
        buildMetaClass_IRandomizer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


