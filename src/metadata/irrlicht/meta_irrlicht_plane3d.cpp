// Auto generated file, don't modify.

#include "irrlicht.h"
#include "plane3d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_plane3d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_plane3d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_plane3d(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Plane3d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<plane3d<f32> > _nd = GDefineMetaClass<plane3d<f32> >::declare("plane3df");
        buildMetaClass_Plane3d<GDefineMetaClass<plane3d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<plane3d<s32> > _nd = GDefineMetaClass<plane3d<s32> >::declare("plane3di");
        buildMetaClass_Plane3d<GDefineMetaClass<plane3d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


