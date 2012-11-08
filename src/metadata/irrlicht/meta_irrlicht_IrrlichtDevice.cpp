// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IrrlichtDevice.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IrrlichtDevice.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IrrlichtDevice()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::IrrlichtDevice, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::IrrlichtDevice, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IrrlichtDevice");
        buildMetaClass_IrrlichtDevice(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


