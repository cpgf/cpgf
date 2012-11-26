// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ILightManager.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ILightManager.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ILightManager()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ILightManager, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ILightManager, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ILightManager");
        buildMetaClass_ILightManager(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<ILightManagerWrapper, irr::scene::ILightManager> _nd = GDefineMetaClass<ILightManagerWrapper, irr::scene::ILightManager>::declare("ILightManagerWrapper");
        buildMetaClass_ILightManagerWrapper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


