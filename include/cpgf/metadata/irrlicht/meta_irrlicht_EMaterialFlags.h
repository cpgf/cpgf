// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EMATERIALFLAGS_H
#define __META_IRRLICHT_EMATERIALFLAGS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ematerialflags(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_MATERIAL_FLAG>("E_MATERIAL_FLAG")
        ._element("EMF_WIREFRAME", irr::video::EMF_WIREFRAME)
        ._element("EMF_POINTCLOUD", irr::video::EMF_POINTCLOUD)
        ._element("EMF_GOURAUD_SHADING", irr::video::EMF_GOURAUD_SHADING)
        ._element("EMF_LIGHTING", irr::video::EMF_LIGHTING)
        ._element("EMF_ZBUFFER", irr::video::EMF_ZBUFFER)
        ._element("EMF_ZWRITE_ENABLE", irr::video::EMF_ZWRITE_ENABLE)
        ._element("EMF_BACK_FACE_CULLING", irr::video::EMF_BACK_FACE_CULLING)
        ._element("EMF_FRONT_FACE_CULLING", irr::video::EMF_FRONT_FACE_CULLING)
        ._element("EMF_BILINEAR_FILTER", irr::video::EMF_BILINEAR_FILTER)
        ._element("EMF_TRILINEAR_FILTER", irr::video::EMF_TRILINEAR_FILTER)
        ._element("EMF_ANISOTROPIC_FILTER", irr::video::EMF_ANISOTROPIC_FILTER)
        ._element("EMF_FOG_ENABLE", irr::video::EMF_FOG_ENABLE)
        ._element("EMF_NORMALIZE_NORMALS", irr::video::EMF_NORMALIZE_NORMALS)
        ._element("EMF_TEXTURE_WRAP", irr::video::EMF_TEXTURE_WRAP)
        ._element("EMF_ANTI_ALIASING", irr::video::EMF_ANTI_ALIASING)
        ._element("EMF_COLOR_MASK", irr::video::EMF_COLOR_MASK)
        ._element("EMF_COLOR_MATERIAL", irr::video::EMF_COLOR_MATERIAL)
        ._element("EMF_USE_MIP_MAPS", irr::video::EMF_USE_MIP_MAPS)
        ._element("EMF_BLEND_OPERATION", irr::video::EMF_BLEND_OPERATION)
        ._element("EMF_POLYGON_OFFSET", irr::video::EMF_POLYGON_OFFSET)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
