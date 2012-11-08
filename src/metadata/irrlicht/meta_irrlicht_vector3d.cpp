// Auto generated file, don't modify.

#include "irrlicht.h"
#include "vector3d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_vector3d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_vector3d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_vector3d(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Vector3d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<vector3d<f32> > _nd = GDefineMetaClass<vector3d<f32> >::declare("vector3df");
        buildMetaClass_Vector3d<GDefineMetaClass<vector3d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<vector3d<s32> > _nd = GDefineMetaClass<vector3d<s32> >::declare("vector3di");
        buildMetaClass_Vector3d<GDefineMetaClass<vector3d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


