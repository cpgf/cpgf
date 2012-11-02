// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/interfaceobject.h"

#include "../include/meta_test_interfaceobject.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_MyInterfaceHolder()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MyInterfaceHolder> _nd = GDefineMetaClass<MyInterfaceHolder>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("MyInterfaceHolder");
        buildMetaClass_MyInterfaceHolder(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MyInterfaceObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MyInterfaceObject, IObject> _nd = GDefineMetaClass<MyInterfaceObject, IObject>::declare("MyInterfaceObject");
        buildMetaClass_MyInterfaceObject(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


