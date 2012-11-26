// Auto generated file, don't modify.

#include "../../../testmetatraits.h"

#include "../../include/metagen_template_classes.h"

#include "../include/meta_test_metagen_template_classes.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_TemplateClassA_TemplateInstance_TemplateClassA_int_char_5()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<TemplateClassA<int, char, 5> > _nd = GDefineMetaClass<TemplateClassA<int, char, 5> >::declare("TemplateClassA_int_char_5");
        buildMetaClass_TemplateClassA<GDefineMetaClass<TemplateClassA<int, char, 5> >, int, char, 5 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_test


