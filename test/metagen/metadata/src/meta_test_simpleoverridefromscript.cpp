// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/simpleoverridefromscript.h"

#include "../include/meta_test_simpleoverridefromscript.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleOverride()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverride, SimpleOverrideBase> _nd = GDefineMetaClass<SimpleOverride, SimpleOverrideBase>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::lazyDeclare("SimpleOverride", &buildMetaClass_SimpleOverride);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride> _nd = GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::lazyDeclare("SimpleOverrideWrapper", &buildMetaClass_SimpleOverrideWrapper);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleOverrideBase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverrideBase> _nd = GDefineMetaClass<SimpleOverrideBase>::lazyDeclare("SimpleOverrideBase", &buildMetaClass_SimpleOverrideBase);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleOverrideHelperData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverrideHelperData> _nd = GDefineMetaClass<SimpleOverrideHelperData>::lazyDeclare("SimpleOverrideHelperData", &buildMetaClass_SimpleOverrideHelperData);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


