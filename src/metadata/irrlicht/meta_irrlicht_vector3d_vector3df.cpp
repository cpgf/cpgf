// Auto generated file, don't modify.

#include "irrlicht.h"
#include "vector3d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_vector3d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Vector3d_TemplateInstance_vector3df()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<vector3d<f32> > _nd = GDefineMetaClass<vector3d<f32> >::declare("vector3df");
        buildMetaClass_Vector3d<GDefineMetaClass<vector3d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


