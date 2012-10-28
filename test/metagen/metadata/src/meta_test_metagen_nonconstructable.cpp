// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_nonconstructable.h"

#include "../include/meta_test_metagen_nonconstructable.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_MetagenNonconstructable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MetagenNonconstructable> _nd = GDefineMetaClass<MetagenNonconstructable>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("MetagenNonconstructable");
        buildMetaClass_MetagenNonconstructable(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


