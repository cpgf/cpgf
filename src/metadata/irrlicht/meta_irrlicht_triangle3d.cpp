// Auto generated file, don't modify.

#include "irrlicht.h"
#include "triangle3d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_triangle3d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Triangle3d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<triangle3d<f32> > _nd = GDefineMetaClass<triangle3d<f32> >::declare("triangle3df");
        buildMetaClass_Triangle3d<GDefineMetaClass<triangle3d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<triangle3d<s32> > _nd = GDefineMetaClass<triangle3d<s32> >::declare("triangle3di");
        buildMetaClass_Triangle3d<GDefineMetaClass<triangle3d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


