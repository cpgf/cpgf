// Auto generated file, don't modify.

#include "irrlicht.h"
#include "matrix4.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_matrix4.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_CMatrix4_TemplateInstance_matrix4()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<CMatrix4<f32> > _nd = GDefineMetaClass<CMatrix4<f32> >::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("matrix4");
        buildMetaClass_CMatrix4<GDefineMetaClass<CMatrix4<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


