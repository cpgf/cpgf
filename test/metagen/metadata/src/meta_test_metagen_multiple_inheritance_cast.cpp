// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_multiple_inheritance_cast.h"

#include "../include/meta_test_metagen_multiple_inheritance_cast.h"

using namespace cpgf;

namespace meta_test { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_Global_metagen_multiple_inheritance_cast()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_metagen_multiple_inheritance_cast(_d);
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastA()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastA, MultipleInheritanceCastR> _nd = GDefineMetaClass<MultipleInheritanceCastA, MultipleInheritanceCastR>::lazyDeclare("MultipleInheritanceCastA", &buildMetaClass_MultipleInheritanceCastA);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastB()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastB, MultipleInheritanceCastA> _nd = GDefineMetaClass<MultipleInheritanceCastB, MultipleInheritanceCastA>::lazyDeclare("MultipleInheritanceCastB", &buildMetaClass_MultipleInheritanceCastB);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastC()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastC, MultipleInheritanceCastA> _nd = GDefineMetaClass<MultipleInheritanceCastC, MultipleInheritanceCastA>::lazyDeclare("MultipleInheritanceCastC", &buildMetaClass_MultipleInheritanceCastC);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastD()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastD, MultipleInheritanceCastB, MultipleInheritanceCastC> _nd = GDefineMetaClass<MultipleInheritanceCastD, MultipleInheritanceCastB, MultipleInheritanceCastC>::lazyDeclare("MultipleInheritanceCastD", &buildMetaClass_MultipleInheritanceCastD);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastFieldData()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastFieldData> _nd = GDefineMetaClass<MultipleInheritanceCastFieldData>::lazyDeclare("MultipleInheritanceCastFieldData", &buildMetaClass_MultipleInheritanceCastFieldData);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_MultipleInheritanceCastR()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<MultipleInheritanceCastR> _nd = GDefineMetaClass<MultipleInheritanceCastR>::lazyDeclare("MultipleInheritanceCastR", &buildMetaClass_MultipleInheritanceCastR);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


