// Auto generated file, don't modify.

#include "irrlicht.h"
#include "S3DVertex.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_S3DVertex.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_s3dvertex()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_s3dvertex(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_S3DVertex()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::S3DVertex> _nd = GDefineMetaClass<irr::video::S3DVertex>::declare("S3DVertex");
        buildMetaClass_S3DVertex(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_S3DVertex2TCoords()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::S3DVertex2TCoords, irr::video::S3DVertex> _nd = GDefineMetaClass<irr::video::S3DVertex2TCoords, irr::video::S3DVertex>::declare("S3DVertex2TCoords");
        buildMetaClass_S3DVertex2TCoords(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_S3DVertexTangents()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::S3DVertexTangents, irr::video::S3DVertex> _nd = GDefineMetaClass<irr::video::S3DVertexTangents, irr::video::S3DVertex>::declare("S3DVertexTangents");
        buildMetaClass_S3DVertexTangents(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


