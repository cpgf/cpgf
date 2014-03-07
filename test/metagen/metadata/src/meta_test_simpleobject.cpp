// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/simpleobject.h"

#include "../include/meta_test_simpleobject.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_Global_simpleobject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_simpleobject(_d);
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleAtom()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleAtom> _nd = GDefineMetaClass<SimpleAtom>::lazyDeclare("SimpleAtom", &buildMetaClass_SimpleAtom);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleData> _nd = GDefineMetaClass<SimpleData>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::lazyDeclare("SimpleData", &buildMetaClass_SimpleData);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_SimpleObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleObject> _nd = GDefineMetaClass<SimpleObject>::lazyDeclare("SimpleObject", &buildMetaClass_SimpleObject);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


