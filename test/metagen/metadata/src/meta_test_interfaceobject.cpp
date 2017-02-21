// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/interfaceobject.h"

#include "../include/meta_test_interfaceobject.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MyInterfaceHolder()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MyInterfaceHolder> _nd = GDefineMetaClass<MyInterfaceHolder>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::lazyDeclare("MyInterfaceHolder", &buildMetaClass_MyInterfaceHolder);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MyInterfaceObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MyInterfaceObject, IObject> _nd = GDefineMetaClass<MyInterfaceObject, IObject>::lazyDeclare("MyInterfaceObject", &buildMetaClass_MyInterfaceObject);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


