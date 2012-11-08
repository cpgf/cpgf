// Auto generated file, don't modify.

#include "irrlicht.h"
#include "vector2d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_vector2d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_vector2d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_vector2d(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Vector2d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<vector2d<s32> > _nd = GDefineMetaClass<vector2d<s32> >::declare("position2d_s32");
        buildMetaClass_Vector2d<GDefineMetaClass<vector2d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<vector2d<s32> > _nd = GDefineMetaClass<vector2d<s32> >::declare("position2di");
        buildMetaClass_Vector2d<GDefineMetaClass<vector2d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<vector2d<f32> > _nd = GDefineMetaClass<vector2d<f32> >::declare("position2d_f32");
        buildMetaClass_Vector2d<GDefineMetaClass<vector2d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<vector2d<f32> > _nd = GDefineMetaClass<vector2d<f32> >::declare("position2df");
        buildMetaClass_Vector2d<GDefineMetaClass<vector2d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


