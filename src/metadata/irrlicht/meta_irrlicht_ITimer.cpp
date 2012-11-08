// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ITimer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ITimer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ITimer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::ITimer, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::ITimer, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ITimer");
        buildMetaClass_ITimer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


