// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/simpleoverridableincrementer.h"

#include "../include/meta_test_simpleoverridableincrementer.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleOverridableIncrementer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleOverridableIncrementer> _nd = GDefineMetaClass<SimpleOverridableIncrementer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::lazyDeclare("SimpleOverridableIncrementer", &buildMetaClass_SimpleOverridableIncrementer);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<SimpleOverridableIncrementerWrapper, SimpleOverridableIncrementer> _nd = GDefineMetaClass<SimpleOverridableIncrementerWrapper, SimpleOverridableIncrementer>::lazyDeclare("SimpleOverridableIncrementerWrapper", &buildMetaClass_SimpleOverridableIncrementerWrapper);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


