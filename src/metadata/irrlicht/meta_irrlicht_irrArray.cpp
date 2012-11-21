// Auto generated file, don't modify.

#include "irrlicht.h"
#include "irrArray.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_irrArray.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Array()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > > _nd = GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > >::declare("tQ3EntityList");
        buildMetaClass_Array<GDefineMetaClass<array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> > >, irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity>  >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<array<video::S3DVertex, irrAllocator<video::S3DVertex> > > _nd = GDefineMetaClass<array<video::S3DVertex, irrAllocator<video::S3DVertex> > >::declare("array_S3DVertex");
        buildMetaClass_Array<GDefineMetaClass<array<video::S3DVertex, irrAllocator<video::S3DVertex> > >, video::S3DVertex, irrAllocator<video::S3DVertex>  >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<array<video::S3DVertex2TCoords, irrAllocator<video::S3DVertex2TCoords> > > _nd = GDefineMetaClass<array<video::S3DVertex2TCoords, irrAllocator<video::S3DVertex2TCoords> > >::declare("array_S3DVertex2TCoords");
        buildMetaClass_Array<GDefineMetaClass<array<video::S3DVertex2TCoords, irrAllocator<video::S3DVertex2TCoords> > >, video::S3DVertex2TCoords, irrAllocator<video::S3DVertex2TCoords>  >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<array<video::S3DVertexTangents, irrAllocator<video::S3DVertexTangents> > > _nd = GDefineMetaClass<array<video::S3DVertexTangents, irrAllocator<video::S3DVertexTangents> > >::declare("array_S3DVertexTangents");
        buildMetaClass_Array<GDefineMetaClass<array<video::S3DVertexTangents, irrAllocator<video::S3DVertexTangents> > >, video::S3DVertexTangents, irrAllocator<video::S3DVertexTangents>  >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<array<f32, irrAllocator<f32> > > _nd = GDefineMetaClass<array<f32, irrAllocator<f32> > >::declare("array_f32");
        buildMetaClass_Array<GDefineMetaClass<array<f32, irrAllocator<f32> > >, f32, irrAllocator<f32>  >(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<array<u16, irrAllocator<u16> > > _nd = GDefineMetaClass<array<u16, irrAllocator<u16> > >::declare("array_u16");
        buildMetaClass_Array<GDefineMetaClass<array<u16, irrAllocator<u16> > >, u16, irrAllocator<u16>  >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


