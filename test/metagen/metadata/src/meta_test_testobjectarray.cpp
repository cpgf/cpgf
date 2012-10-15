// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/testobjectarray.h"

#include "../include/meta_test_testobjectarray.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_Global_testobjectarray()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_testobjectarray(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_OAData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<OAData> _nd = GDefineMetaClass<OAData>::declare("OAData");
        buildMetaClass_OAData(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_OAObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<OAObject> _nd = GDefineMetaClass<OAObject>::declare("OAObject");
        buildMetaClass_OAObject(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


