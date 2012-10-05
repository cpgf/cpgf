// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/simpleobject.h"

#include "../include/meta_test_simpleobject.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_Global_simpleobject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_simpleobject(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleAtom()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleAtom> _nd = GDefineMetaClass<SimpleAtom>::declare("SimpleAtom");
        buildMetaClass_SimpleAtom(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleData> _nd = GDefineMetaClass<SimpleData>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleData");
        buildMetaClass_SimpleData(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<SimpleObject> _nd = GDefineMetaClass<SimpleObject>::declare("SimpleObject");
        buildMetaClass_SimpleObject(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


