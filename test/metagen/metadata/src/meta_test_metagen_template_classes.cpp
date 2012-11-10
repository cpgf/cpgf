// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_template_classes.h"

#include "../include/meta_test_metagen_template_classes.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_TemplateBase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TemplateBase> _nd = GDefineMetaClass<TemplateBase>::declare("TemplateBase");
        buildMetaClass_TemplateBase(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_TemplateClassA()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TemplateClassA<int, char, 5> > _nd = GDefineMetaClass<TemplateClassA<int, char, 5> >::declare("TemplateClassA_int,char,5");
        buildMetaClass_TemplateClassA<GDefineMetaClass<TemplateClassA<int, char, 5> >, int, char, 5 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_TemplateClassB()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TemplateClassB<TemplateBase> > _nd = GDefineMetaClass<TemplateClassB<TemplateBase> >::declare("TemplateClassB_TemplateBase");
        buildMetaClass_TemplateClassB<GDefineMetaClass<TemplateClassB<TemplateBase> >, TemplateBase >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_test


