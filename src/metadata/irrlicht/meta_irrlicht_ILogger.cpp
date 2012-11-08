// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ILogger.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ILogger.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ilogger()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ilogger(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_ILogger()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::ILogger, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::ILogger, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ILogger");
        buildMetaClass_ILogger(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


