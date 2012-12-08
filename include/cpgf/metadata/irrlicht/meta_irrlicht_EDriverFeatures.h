// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EDRIVERFEATURES_H
#define __META_IRRLICHT_EDRIVERFEATURES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_edriverfeatures(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_VIDEO_DRIVER_FEATURE>("E_VIDEO_DRIVER_FEATURE")
        ._element("EVDF_RENDER_TO_TARGET", irr::video::EVDF_RENDER_TO_TARGET)
        ._element("EVDF_HARDWARE_TL", irr::video::EVDF_HARDWARE_TL)
        ._element("EVDF_MULTITEXTURE", irr::video::EVDF_MULTITEXTURE)
        ._element("EVDF_BILINEAR_FILTER", irr::video::EVDF_BILINEAR_FILTER)
        ._element("EVDF_MIP_MAP", irr::video::EVDF_MIP_MAP)
        ._element("EVDF_MIP_MAP_AUTO_UPDATE", irr::video::EVDF_MIP_MAP_AUTO_UPDATE)
        ._element("EVDF_STENCIL_BUFFER", irr::video::EVDF_STENCIL_BUFFER)
        ._element("EVDF_VERTEX_SHADER_1_1", irr::video::EVDF_VERTEX_SHADER_1_1)
        ._element("EVDF_VERTEX_SHADER_2_0", irr::video::EVDF_VERTEX_SHADER_2_0)
        ._element("EVDF_VERTEX_SHADER_3_0", irr::video::EVDF_VERTEX_SHADER_3_0)
        ._element("EVDF_PIXEL_SHADER_1_1", irr::video::EVDF_PIXEL_SHADER_1_1)
        ._element("EVDF_PIXEL_SHADER_1_2", irr::video::EVDF_PIXEL_SHADER_1_2)
        ._element("EVDF_PIXEL_SHADER_1_3", irr::video::EVDF_PIXEL_SHADER_1_3)
        ._element("EVDF_PIXEL_SHADER_1_4", irr::video::EVDF_PIXEL_SHADER_1_4)
        ._element("EVDF_PIXEL_SHADER_2_0", irr::video::EVDF_PIXEL_SHADER_2_0)
        ._element("EVDF_PIXEL_SHADER_3_0", irr::video::EVDF_PIXEL_SHADER_3_0)
        ._element("EVDF_ARB_VERTEX_PROGRAM_1", irr::video::EVDF_ARB_VERTEX_PROGRAM_1)
        ._element("EVDF_ARB_FRAGMENT_PROGRAM_1", irr::video::EVDF_ARB_FRAGMENT_PROGRAM_1)
        ._element("EVDF_ARB_GLSL", irr::video::EVDF_ARB_GLSL)
        ._element("EVDF_HLSL", irr::video::EVDF_HLSL)
        ._element("EVDF_TEXTURE_NSQUARE", irr::video::EVDF_TEXTURE_NSQUARE)
        ._element("EVDF_TEXTURE_NPOT", irr::video::EVDF_TEXTURE_NPOT)
        ._element("EVDF_FRAMEBUFFER_OBJECT", irr::video::EVDF_FRAMEBUFFER_OBJECT)
        ._element("EVDF_VERTEX_BUFFER_OBJECT", irr::video::EVDF_VERTEX_BUFFER_OBJECT)
        ._element("EVDF_ALPHA_TO_COVERAGE", irr::video::EVDF_ALPHA_TO_COVERAGE)
        ._element("EVDF_COLOR_MASK", irr::video::EVDF_COLOR_MASK)
        ._element("EVDF_MULTIPLE_RENDER_TARGETS", irr::video::EVDF_MULTIPLE_RENDER_TARGETS)
        ._element("EVDF_MRT_BLEND", irr::video::EVDF_MRT_BLEND)
        ._element("EVDF_MRT_COLOR_MASK", irr::video::EVDF_MRT_COLOR_MASK)
        ._element("EVDF_MRT_BLEND_FUNC", irr::video::EVDF_MRT_BLEND_FUNC)
        ._element("EVDF_GEOMETRY_SHADER", irr::video::EVDF_GEOMETRY_SHADER)
        ._element("EVDF_OCCLUSION_QUERY", irr::video::EVDF_OCCLUSION_QUERY)
        ._element("EVDF_POLYGON_OFFSET", irr::video::EVDF_POLYGON_OFFSET)
        ._element("EVDF_BLEND_OPERATIONS", irr::video::EVDF_BLEND_OPERATIONS)
        ._element("EVDF_TEXTURE_MATRIX", irr::video::EVDF_TEXTURE_MATRIX)
        ._element("EVDF_CG", irr::video::EVDF_CG)
        ._element("EVDF_COUNT", irr::video::EVDF_COUNT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
