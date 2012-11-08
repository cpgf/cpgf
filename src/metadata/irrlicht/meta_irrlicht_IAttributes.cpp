// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IAttributes.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IAttributes.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IAttributes()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IAttributes, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IAttributes, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IAttributes");
        buildMetaClass_IAttributes(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


