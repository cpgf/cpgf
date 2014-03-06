// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/testobjectarray.h"

#include "../include/meta_test_testobjectarray.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_Global_testobjectarray()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_testobjectarray(_d);
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_OAData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<OAData> _nd = GDefineMetaClass<OAData>::lazyDeclare("OAData", &buildMetaClass_OAData);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_OAObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<OAObject> _nd = GDefineMetaClass<OAObject>::lazyDeclare("OAObject", &buildMetaClass_OAObject);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


