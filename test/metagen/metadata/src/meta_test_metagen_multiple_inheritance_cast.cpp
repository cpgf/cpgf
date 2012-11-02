// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_multiple_inheritance_cast.h"

#include "../include/meta_test_metagen_multiple_inheritance_cast.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_Global_metagen_multiple_inheritance_cast()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_metagen_multiple_inheritance_cast(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastA()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastA, MultipleInheritanceCastR> _nd = GDefineMetaClass<MultipleInheritanceCastA, MultipleInheritanceCastR>::declare("MultipleInheritanceCastA");
        buildMetaClass_MultipleInheritanceCastA(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastB()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastB, MultipleInheritanceCastA> _nd = GDefineMetaClass<MultipleInheritanceCastB, MultipleInheritanceCastA>::declare("MultipleInheritanceCastB");
        buildMetaClass_MultipleInheritanceCastB(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastC()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastC, MultipleInheritanceCastA> _nd = GDefineMetaClass<MultipleInheritanceCastC, MultipleInheritanceCastA>::declare("MultipleInheritanceCastC");
        buildMetaClass_MultipleInheritanceCastC(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastD()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastD, MultipleInheritanceCastB, MultipleInheritanceCastC> _nd = GDefineMetaClass<MultipleInheritanceCastD, MultipleInheritanceCastB, MultipleInheritanceCastC>::declare("MultipleInheritanceCastD");
        buildMetaClass_MultipleInheritanceCastD(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastFieldData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastFieldData> _nd = GDefineMetaClass<MultipleInheritanceCastFieldData>::declare("MultipleInheritanceCastFieldData");
        buildMetaClass_MultipleInheritanceCastFieldData(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_MultipleInheritanceCastR()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastR> _nd = GDefineMetaClass<MultipleInheritanceCastR>::declare("MultipleInheritanceCastR");
        buildMetaClass_MultipleInheritanceCastR(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


