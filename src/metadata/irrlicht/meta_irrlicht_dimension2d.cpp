// Auto generated file, don't modify.

#include "irrlicht.h"
#include "dimension2d.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_dimension2d.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_dimension2d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_dimension2d(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Dimension2d()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<dimension2d<u32> > _nd = GDefineMetaClass<dimension2d<u32> >::declare("dimension2d_u32");
        buildMetaClass_Dimension2d<GDefineMetaClass<dimension2d<u32> >, u32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<dimension2d<f32> > _nd = GDefineMetaClass<dimension2d<f32> >::declare("dimension2d_f32");
        buildMetaClass_Dimension2d<GDefineMetaClass<dimension2d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<dimension2d<u32> > _nd = GDefineMetaClass<dimension2d<u32> >::declare("dimension2du");
        buildMetaClass_Dimension2d<GDefineMetaClass<dimension2d<u32> >, u32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<dimension2d<f32> > _nd = GDefineMetaClass<dimension2d<f32> >::declare("dimension2df");
        buildMetaClass_Dimension2d<GDefineMetaClass<dimension2d<f32> >, f32 >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<dimension2d<s32> > _nd = GDefineMetaClass<dimension2d<s32> >::declare("dimension2di");
        buildMetaClass_Dimension2d<GDefineMetaClass<dimension2d<s32> >, s32 >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


