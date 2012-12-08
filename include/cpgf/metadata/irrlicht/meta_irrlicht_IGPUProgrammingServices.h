// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGPUPROGRAMMINGSERVICES_H
#define __META_IRRLICHT_IGPUPROGRAMMINGSERVICES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_igpuprogrammingservices(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_GPU_SHADING_LANGUAGE>("E_GPU_SHADING_LANGUAGE")
        ._element("EGSL_DEFAULT", irr::video::EGSL_DEFAULT)
        ._element("EGSL_CG", irr::video::EGSL_CG)
    ;
}


template <typename D>
void buildMetaClass_IGPUProgrammingServices(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterial", (s32 (D::ClassType::*) (const c8 *, const c8 *, E_VERTEX_SHADER_TYPE, const c8 *, const c8 *, E_PIXEL_SHADER_TYPE, const c8 *, const c8 *, E_GEOMETRY_SHADER_TYPE, scene::E_PRIMITIVE_TYPE, scene::E_PRIMITIVE_TYPE, u32, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterial)
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLE_STRIP))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(EGST_GS_4_0))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterial", (s32 (D::ClassType::*) (const c8 *, const c8 *, E_VERTEX_SHADER_TYPE, const c8 *, const c8 *, E_PIXEL_SHADER_TYPE, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterial)
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EPST_PS_1_1))
        ._default(copyVariantFromCopyable("main"))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EVST_VS_1_1))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterial", (s32 (D::ClassType::*) (const c8 *, const c8 *, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addHighLevelShaderMaterial)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterial", (s32 (D::ClassType::*) (const c8 *, const c8 *, const c8 *, scene::E_PRIMITIVE_TYPE, scene::E_PRIMITIVE_TYPE, u32, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addHighLevelShaderMaterial)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLE_STRIP))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (const io::path &, const c8 *, E_VERTEX_SHADER_TYPE, const io::path &, const c8 *, E_PIXEL_SHADER_TYPE, const io::path &, const c8 *, E_GEOMETRY_SHADER_TYPE, scene::E_PRIMITIVE_TYPE, scene::E_PRIMITIVE_TYPE, u32, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterialFromFiles, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<3>, cpgf::GMetaRuleCopyConstReference<6> >())
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLE_STRIP))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(EGST_GS_4_0))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (const io::path &, const c8 *, E_VERTEX_SHADER_TYPE, const io::path &, const c8 *, E_PIXEL_SHADER_TYPE, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterialFromFiles, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<3> >())
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EPST_PS_1_1))
        ._default(copyVariantFromCopyable("main"))
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(EVST_VS_1_1))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (const io::path &, const io::path &, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addHighLevelShaderMaterialFromFiles, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (const io::path &, const io::path &, const io::path &, scene::E_PRIMITIVE_TYPE, scene::E_PRIMITIVE_TYPE, u32, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addHighLevelShaderMaterialFromFiles, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLE_STRIP))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (io::IReadFile *, const c8 *, E_VERTEX_SHADER_TYPE, io::IReadFile *, const c8 *, E_PIXEL_SHADER_TYPE, io::IReadFile *, const c8 *, E_GEOMETRY_SHADER_TYPE, scene::E_PRIMITIVE_TYPE, scene::E_PRIMITIVE_TYPE, u32, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterialFromFiles)
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLE_STRIP))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(EGST_GS_4_0))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHighLevelShaderMaterialFromFiles", (s32 (D::ClassType::*) (io::IReadFile *, const c8 *, E_VERTEX_SHADER_TYPE, io::IReadFile *, const c8 *, E_PIXEL_SHADER_TYPE, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32, E_GPU_SHADING_LANGUAGE))&D::ClassType::addHighLevelShaderMaterialFromFiles)
        ._default(copyVariantFromCopyable(EGSL_DEFAULT))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EPST_PS_1_1))
        ._default(copyVariantFromCopyable("main"))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(EVST_VS_1_1))
        ._default(copyVariantFromCopyable("main"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addShaderMaterial", &D::ClassType::addShaderMaterial)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addShaderMaterialFromFiles", (s32 (D::ClassType::*) (io::IReadFile *, io::IReadFile *, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addShaderMaterialFromFiles)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addShaderMaterialFromFiles", (s32 (D::ClassType::*) (const io::path &, const io::path &, IShaderConstantSetCallBack *, E_MATERIAL_TYPE, s32))&D::ClassType::addShaderMaterialFromFiles, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::EMT_SOLID))
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
