// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGPUProgrammingServices.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGPUProgrammingServices.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_igpuprogrammingservices()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_igpuprogrammingservices(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGPUProgrammingServices()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IGPUProgrammingServices> _nd = GDefineMetaClass<irr::video::IGPUProgrammingServices>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGPUProgrammingServices");
        buildMetaClass_IGPUProgrammingServices(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


