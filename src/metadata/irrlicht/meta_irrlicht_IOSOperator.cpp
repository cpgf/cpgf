// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IOSOperator.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IOSOperator.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IOSOperator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::IOSOperator, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::IOSOperator, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IOSOperator");
        buildMetaClass_IOSOperator(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


