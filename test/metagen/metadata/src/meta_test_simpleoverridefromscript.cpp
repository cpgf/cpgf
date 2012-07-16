// Auto generated file, don't modify.

#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/metatraits/gmetaserializer_string.h"

#include "../../include/simpleoverridefromscript.h"

#include "../include/meta_test_simpleoverridefromscript.h"

using namespace cpgf;

namespace meta_test { 


bool SimpleOverrideWrapper::_cpgf_override_method_is_in_script[1] = { false };

GDefineMetaInfo createMetaClass_SimpleOverride()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverride> _nd = GDefineMetaClass<SimpleOverride>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleOverride");
        buildMetaClass_SimpleOverride(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride> _nd = GDefineMetaClass<SimpleOverrideWrapper, SimpleOverride>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleOverrideWrapper");
        buildMetaClass_SimpleOverride(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


