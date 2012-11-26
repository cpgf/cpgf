// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_template_classes.h"

#include "../include/meta_test_metagen_template_classes.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_TemplateClassB_TemplateInstance_TemplateClassB_TemplateBase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TemplateClassB<TemplateBase>, TemplateBase > _nd = GDefineMetaClass<TemplateClassB<TemplateBase>, TemplateBase >::declare("TemplateClassB_TemplateBase");
        buildMetaClass_TemplateClassB<GDefineMetaClass<TemplateClassB<TemplateBase>, TemplateBase >, TemplateBase >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_test


