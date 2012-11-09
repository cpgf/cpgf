// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_S3DVERTEX_H
#define __META_IRRLICHT_S3DVERTEX_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_s3dvertex(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("sBuiltInVertexTypeNames", &sBuiltInVertexTypeNames);
    _d.CPGF_MD_TEMPLATE _method("getVertexPitchFromType", (u32 (*) (E_VERTEX_TYPE))&getVertexPitchFromType);
    _d.CPGF_MD_TEMPLATE _enum<E_VERTEX_TYPE>("E_VERTEX_TYPE")
        ._element("EVT_STANDARD", irr::video::EVT_STANDARD)
        ._element("EVT_2TCOORDS", irr::video::EVT_2TCOORDS)
        ._element("EVT_TANGENTS", irr::video::EVT_TANGENTS)
    ;
}


template <typename D>
void buildMetaClass_S3DVertex(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32, f32, f32, SColor, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, const core::vector3df &, SColor, const core::vector2d< f32 > &)>();
    _d.CPGF_MD_TEMPLATE _field("Pos", &D::ClassType::Pos);
    _d.CPGF_MD_TEMPLATE _field("Normal", &D::ClassType::Normal);
    _d.CPGF_MD_TEMPLATE _field("Color", &D::ClassType::Color);
    _d.CPGF_MD_TEMPLATE _field("TCoords", &D::ClassType::TCoords);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex &)>(mopHolder < mopHolder);
}


template <typename D>
void buildMetaClass_S3DVertex2TCoords(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, SColor, f32, f32, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, SColor, const core::vector2d< f32 > &, const core::vector2d< f32 > &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, const core::vector3df &, const SColor &, const core::vector2d< f32 > &, const core::vector2d< f32 > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32, f32, f32, SColor, f32, f32, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32, f32, f32, SColor, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, const core::vector3df &, SColor, const core::vector2d< f32 > &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (S3DVertex &)>();
    _d.CPGF_MD_TEMPLATE _field("TCoords2", &D::ClassType::TCoords2);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex2TCoords &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex2TCoords &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertex2TCoords &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


template <typename D>
void buildMetaClass_S3DVertexTangents(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32, f32, f32, SColor, f32, f32, f32, f32, f32, f32, f32, f32)>()
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0.0f))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, SColor, const core::vector2df &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, const core::vector3df &, SColor, const core::vector2df &, const core::vector3df &, const core::vector3df &)>()
        ._default(copyVariantFromCopyable(core::vector3df()))
        ._default(copyVariantFromCopyable(core::vector3df()))
    ;
    _d.CPGF_MD_TEMPLATE _field("Tangent", &D::ClassType::Tangent);
    _d.CPGF_MD_TEMPLATE _field("Binormal", &D::ClassType::Binormal);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertexTangents &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertexTangents &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const S3DVertexTangents &)>(mopHolder < mopHolder);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
