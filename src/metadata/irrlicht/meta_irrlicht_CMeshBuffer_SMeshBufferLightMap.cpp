// Auto generated file, don't modify.

#include "irrlicht.h"
#include "CMeshBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_CMeshBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_CMeshBuffer_TemplateInstance_SMeshBufferLightMap()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<CMeshBuffer<video::S3DVertex2TCoords>, irr::scene::IMeshBuffer > _nd = GDefineMetaClass<CMeshBuffer<video::S3DVertex2TCoords>, irr::scene::IMeshBuffer >::declare("SMeshBufferLightMap");
        buildMetaClass_CMeshBuffer<GDefineMetaClass<CMeshBuffer<video::S3DVertex2TCoords>, irr::scene::IMeshBuffer >, video::S3DVertex2TCoords >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


