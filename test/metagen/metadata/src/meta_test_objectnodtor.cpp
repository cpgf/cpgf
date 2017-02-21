// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/objectnodtor.h"

#include "../include/meta_test_objectnodtor.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_ObjectNoDestructor()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<ObjectNoDestructor> _nd = GDefineMetaClass<ObjectNoDestructor>::Policy<MakePolicy<GMetaRuleDestructorAbsent> >::lazyDeclare("ObjectNoDestructor", &buildMetaClass_ObjectNoDestructor);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


