// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_userconverter.h"

#include "../include/meta_test_metagen_userconverter.h"

using namespace cpgf;

namespace meta_test { 


#ifdef DLL_PUBLIC
DLL_PUBLIC
#endif
GDefineMetaInfo createMetaClass_Global_metagen_userconverter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_metagen_userconverter(0, _d);
    return _d.getMetaInfo();
}


#ifdef DLL_PUBLIC
DLL_PUBLIC
#endif
GDefineMetaInfo createMetaClass_TestUserConverterData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TestUserConverterData> _nd = GDefineMetaClass<TestUserConverterData>::declare("TestUserConverterData");
        buildMetaClass_TestUserConverterData(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


