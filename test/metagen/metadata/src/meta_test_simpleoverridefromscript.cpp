// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/simpleoverridefromscript.h"

#include "../include/meta_test_simpleoverridefromscript.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_SimpleOverride()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverride, SimpleOverrideBase> _nd = GDefineMetaClass<SimpleOverride, SimpleOverrideBase>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleOverride");
        buildMetaClass_SimpleOverride(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride> _nd = GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleOverrideWrapper");
        buildMetaClass_SimpleOverrideWrapper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleOverrideBase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverrideBase> _nd = GDefineMetaClass<SimpleOverrideBase>::declare("SimpleOverrideBase");
        buildMetaClass_SimpleOverrideBase(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


