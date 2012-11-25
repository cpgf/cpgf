// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IVideoDriver.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IVideoDriver_IVideoDriver.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IVideoDriver()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IVideoDriver, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::IVideoDriver, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IVideoDriver");
        buildMetaClass_IVideoDriver(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


