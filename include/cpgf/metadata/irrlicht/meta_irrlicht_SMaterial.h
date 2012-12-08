// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SMATERIAL_H
#define __META_IRRLICHT_SMATERIAL_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_smaterial(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("PolygonOffsetDirectionNames", &PolygonOffsetDirectionNames);
    _d.CPGF_MD_TEMPLATE _field("MATERIAL_MAX_TEXTURES", &MATERIAL_MAX_TEXTURES);
    _d.CPGF_MD_TEMPLATE _field("IdentityMaterial", &IdentityMaterial);
    _d.CPGF_MD_TEMPLATE _method("pack_textureBlendFunc", (f32 (*) (const E_BLEND_FACTOR, const E_BLEND_FACTOR, const E_MODULATE_FUNC, const u32))&pack_textureBlendFunc)
        ._default(copyVariantFromCopyable(EAS_TEXTURE))
        ._default(copyVariantFromCopyable(EMFN_MODULATE_1X))
    ;
    _d.CPGF_MD_TEMPLATE _method("unpack_textureBlendFunc", (void (*) (E_BLEND_FACTOR &, E_BLEND_FACTOR &, E_MODULATE_FUNC &, u32 &, const f32))&unpack_textureBlendFunc);
    _d.CPGF_MD_TEMPLATE _method("textureBlendFunc_hasAlpha", (bool (*) (const E_BLEND_FACTOR))&textureBlendFunc_hasAlpha);
    _d.CPGF_MD_TEMPLATE _enum<E_BLEND_FACTOR>("E_BLEND_FACTOR")
        ._element("EBF_ZERO", irr::video::EBF_ZERO)
        ._element("EBF_ONE", irr::video::EBF_ONE)
        ._element("EBF_DST_COLOR", irr::video::EBF_DST_COLOR)
        ._element("EBF_ONE_MINUS_DST_COLOR", irr::video::EBF_ONE_MINUS_DST_COLOR)
        ._element("EBF_SRC_COLOR", irr::video::EBF_SRC_COLOR)
        ._element("EBF_ONE_MINUS_SRC_COLOR", irr::video::EBF_ONE_MINUS_SRC_COLOR)
        ._element("EBF_SRC_ALPHA", irr::video::EBF_SRC_ALPHA)
        ._element("EBF_ONE_MINUS_SRC_ALPHA", irr::video::EBF_ONE_MINUS_SRC_ALPHA)
        ._element("EBF_DST_ALPHA", irr::video::EBF_DST_ALPHA)
        ._element("EBF_ONE_MINUS_DST_ALPHA", irr::video::EBF_ONE_MINUS_DST_ALPHA)
        ._element("EBF_SRC_ALPHA_SATURATE", irr::video::EBF_SRC_ALPHA_SATURATE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_BLEND_OPERATION>("E_BLEND_OPERATION")
        ._element("EBO_NONE", irr::video::EBO_NONE)
        ._element("EBO_ADD", irr::video::EBO_ADD)
        ._element("EBO_SUBTRACT", irr::video::EBO_SUBTRACT)
        ._element("EBO_REVSUBTRACT", irr::video::EBO_REVSUBTRACT)
        ._element("EBO_MIN", irr::video::EBO_MIN)
        ._element("EBO_MAX", irr::video::EBO_MAX)
        ._element("EBO_MIN_FACTOR", irr::video::EBO_MIN_FACTOR)
        ._element("EBO_MAX_FACTOR", irr::video::EBO_MAX_FACTOR)
        ._element("EBO_MIN_ALPHA", irr::video::EBO_MIN_ALPHA)
        ._element("EBO_MAX_ALPHA", irr::video::EBO_MAX_ALPHA)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_MODULATE_FUNC>("E_MODULATE_FUNC")
        ._element("EMFN_MODULATE_1X", irr::video::EMFN_MODULATE_1X)
        ._element("EMFN_MODULATE_2X", irr::video::EMFN_MODULATE_2X)
        ._element("EMFN_MODULATE_4X", irr::video::EMFN_MODULATE_4X)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COMPARISON_FUNC>("E_COMPARISON_FUNC")
        ._element("ECFN_NEVER", irr::video::ECFN_NEVER)
        ._element("ECFN_LESSEQUAL", irr::video::ECFN_LESSEQUAL)
        ._element("ECFN_EQUAL", irr::video::ECFN_EQUAL)
        ._element("ECFN_LESS", irr::video::ECFN_LESS)
        ._element("ECFN_NOTEQUAL", irr::video::ECFN_NOTEQUAL)
        ._element("ECFN_GREATEREQUAL", irr::video::ECFN_GREATEREQUAL)
        ._element("ECFN_GREATER", irr::video::ECFN_GREATER)
        ._element("ECFN_ALWAYS", irr::video::ECFN_ALWAYS)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLOR_PLANE>("E_COLOR_PLANE")
        ._element("ECP_NONE", irr::video::ECP_NONE)
        ._element("ECP_ALPHA", irr::video::ECP_ALPHA)
        ._element("ECP_RED", irr::video::ECP_RED)
        ._element("ECP_GREEN", irr::video::ECP_GREEN)
        ._element("ECP_BLUE", irr::video::ECP_BLUE)
        ._element("ECP_RGB", irr::video::ECP_RGB)
        ._element("ECP_ALL", irr::video::ECP_ALL)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_ALPHA_SOURCE>("E_ALPHA_SOURCE")
        ._element("EAS_NONE", irr::video::EAS_NONE)
        ._element("EAS_VERTEX_COLOR", irr::video::EAS_VERTEX_COLOR)
        ._element("EAS_TEXTURE", irr::video::EAS_TEXTURE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_ANTI_ALIASING_MODE>("E_ANTI_ALIASING_MODE")
        ._element("EAAM_OFF", irr::video::EAAM_OFF)
        ._element("EAAM_SIMPLE", irr::video::EAAM_SIMPLE)
        ._element("EAAM_QUALITY", irr::video::EAAM_QUALITY)
        ._element("EAAM_LINE_SMOOTH", irr::video::EAAM_LINE_SMOOTH)
        ._element("EAAM_POINT_SMOOTH", irr::video::EAAM_POINT_SMOOTH)
        ._element("EAAM_FULL_BASIC", irr::video::EAAM_FULL_BASIC)
        ._element("EAAM_ALPHA_TO_COVERAGE", irr::video::EAAM_ALPHA_TO_COVERAGE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLOR_MATERIAL>("E_COLOR_MATERIAL")
        ._element("ECM_NONE", irr::video::ECM_NONE)
        ._element("ECM_DIFFUSE", irr::video::ECM_DIFFUSE)
        ._element("ECM_AMBIENT", irr::video::ECM_AMBIENT)
        ._element("ECM_EMISSIVE", irr::video::ECM_EMISSIVE)
        ._element("ECM_SPECULAR", irr::video::ECM_SPECULAR)
        ._element("ECM_DIFFUSE_AND_AMBIENT", irr::video::ECM_DIFFUSE_AND_AMBIENT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_POLYGON_OFFSET>("E_POLYGON_OFFSET")
        ._element("EPO_BACK", irr::video::EPO_BACK)
        ._element("EPO_FRONT", irr::video::EPO_FRONT)
    ;
}


inline u8 bItFiEldWrapper_SMaterial_ColorMask_getter(SMaterial * self) {
    return self->ColorMask;
}

inline void bItFiEldWrapper_SMaterial_ColorMask_setter(SMaterial * self, u8 value) {
    self->ColorMask = value;
}
inline u8 bItFiEldWrapper_SMaterial_ColorMaterial_getter(SMaterial * self) {
    return self->ColorMaterial;
}

inline void bItFiEldWrapper_SMaterial_ColorMaterial_setter(SMaterial * self, u8 value) {
    self->ColorMaterial = value;
}
inline E_BLEND_OPERATION bItFiEldWrapper_SMaterial_BlendOperation_getter(SMaterial * self) {
    return self->BlendOperation;
}

inline void bItFiEldWrapper_SMaterial_BlendOperation_setter(SMaterial * self, E_BLEND_OPERATION value) {
    self->BlendOperation = value;
}
inline u8 bItFiEldWrapper_SMaterial_PolygonOffsetFactor_getter(SMaterial * self) {
    return self->PolygonOffsetFactor;
}

inline void bItFiEldWrapper_SMaterial_PolygonOffsetFactor_setter(SMaterial * self, u8 value) {
    self->PolygonOffsetFactor = value;
}
inline E_POLYGON_OFFSET bItFiEldWrapper_SMaterial_PolygonOffsetDirection_getter(SMaterial * self) {
    return self->PolygonOffsetDirection;
}

inline void bItFiEldWrapper_SMaterial_PolygonOffsetDirection_setter(SMaterial * self, E_POLYGON_OFFSET value) {
    self->PolygonOffsetDirection = value;
}
inline bool bItFiEldWrapper_SMaterial_Wireframe_getter(SMaterial * self) {
    return self->Wireframe;
}

inline void bItFiEldWrapper_SMaterial_Wireframe_setter(SMaterial * self, bool value) {
    self->Wireframe = value;
}
inline bool bItFiEldWrapper_SMaterial_PointCloud_getter(SMaterial * self) {
    return self->PointCloud;
}

inline void bItFiEldWrapper_SMaterial_PointCloud_setter(SMaterial * self, bool value) {
    self->PointCloud = value;
}
inline bool bItFiEldWrapper_SMaterial_GouraudShading_getter(SMaterial * self) {
    return self->GouraudShading;
}

inline void bItFiEldWrapper_SMaterial_GouraudShading_setter(SMaterial * self, bool value) {
    self->GouraudShading = value;
}
inline bool bItFiEldWrapper_SMaterial_Lighting_getter(SMaterial * self) {
    return self->Lighting;
}

inline void bItFiEldWrapper_SMaterial_Lighting_setter(SMaterial * self, bool value) {
    self->Lighting = value;
}
inline bool bItFiEldWrapper_SMaterial_ZWriteEnable_getter(SMaterial * self) {
    return self->ZWriteEnable;
}

inline void bItFiEldWrapper_SMaterial_ZWriteEnable_setter(SMaterial * self, bool value) {
    self->ZWriteEnable = value;
}
inline bool bItFiEldWrapper_SMaterial_BackfaceCulling_getter(SMaterial * self) {
    return self->BackfaceCulling;
}

inline void bItFiEldWrapper_SMaterial_BackfaceCulling_setter(SMaterial * self, bool value) {
    self->BackfaceCulling = value;
}
inline bool bItFiEldWrapper_SMaterial_FrontfaceCulling_getter(SMaterial * self) {
    return self->FrontfaceCulling;
}

inline void bItFiEldWrapper_SMaterial_FrontfaceCulling_setter(SMaterial * self, bool value) {
    self->FrontfaceCulling = value;
}
inline bool bItFiEldWrapper_SMaterial_FogEnable_getter(SMaterial * self) {
    return self->FogEnable;
}

inline void bItFiEldWrapper_SMaterial_FogEnable_setter(SMaterial * self, bool value) {
    self->FogEnable = value;
}
inline bool bItFiEldWrapper_SMaterial_NormalizeNormals_getter(SMaterial * self) {
    return self->NormalizeNormals;
}

inline void bItFiEldWrapper_SMaterial_NormalizeNormals_setter(SMaterial * self, bool value) {
    self->NormalizeNormals = value;
}
inline bool bItFiEldWrapper_SMaterial_UseMipMaps_getter(SMaterial * self) {
    return self->UseMipMaps;
}

inline void bItFiEldWrapper_SMaterial_UseMipMaps_setter(SMaterial * self, bool value) {
    self->UseMipMaps = value;
}


inline SMaterial & opErAToRWrapper_SMaterial__opAssign(SMaterial * self, const SMaterial & other) {
    return (*self) = other;
}
inline bool opErAToRWrapper_SMaterial__opNotEqual(const SMaterial * self, const SMaterial & b) {
    return (*self) != b;
}
inline bool opErAToRWrapper_SMaterial__opEqual(const SMaterial * self, const SMaterial & b) {
    return (*self) == b;
}


template <typename D>
void buildMetaClass_SMaterial(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SMaterial &)>();
    _d.CPGF_MD_TEMPLATE _field("TextureLayer", &D::ClassType::TextureLayer);
    _d.CPGF_MD_TEMPLATE _field("MaterialType", &D::ClassType::MaterialType);
    _d.CPGF_MD_TEMPLATE _field("AmbientColor", &D::ClassType::AmbientColor);
    _d.CPGF_MD_TEMPLATE _field("DiffuseColor", &D::ClassType::DiffuseColor);
    _d.CPGF_MD_TEMPLATE _field("EmissiveColor", &D::ClassType::EmissiveColor);
    _d.CPGF_MD_TEMPLATE _field("SpecularColor", &D::ClassType::SpecularColor);
    _d.CPGF_MD_TEMPLATE _field("Shininess", &D::ClassType::Shininess);
    _d.CPGF_MD_TEMPLATE _field("MaterialTypeParam", &D::ClassType::MaterialTypeParam);
    _d.CPGF_MD_TEMPLATE _field("MaterialTypeParam2", &D::ClassType::MaterialTypeParam2);
    _d.CPGF_MD_TEMPLATE _field("Thickness", &D::ClassType::Thickness);
    _d.CPGF_MD_TEMPLATE _field("ZBuffer", &D::ClassType::ZBuffer);
    _d.CPGF_MD_TEMPLATE _field("AntiAliasing", &D::ClassType::AntiAliasing);
    _d.CPGF_MD_TEMPLATE _property("ColorMask", &bItFiEldWrapper_SMaterial_ColorMask_getter, &bItFiEldWrapper_SMaterial_ColorMask_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("ColorMaterial", &bItFiEldWrapper_SMaterial_ColorMaterial_getter, &bItFiEldWrapper_SMaterial_ColorMaterial_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BlendOperation", &bItFiEldWrapper_SMaterial_BlendOperation_getter, &bItFiEldWrapper_SMaterial_BlendOperation_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("PolygonOffsetFactor", &bItFiEldWrapper_SMaterial_PolygonOffsetFactor_getter, &bItFiEldWrapper_SMaterial_PolygonOffsetFactor_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("PolygonOffsetDirection", &bItFiEldWrapper_SMaterial_PolygonOffsetDirection_getter, &bItFiEldWrapper_SMaterial_PolygonOffsetDirection_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("Wireframe", &bItFiEldWrapper_SMaterial_Wireframe_getter, &bItFiEldWrapper_SMaterial_Wireframe_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("PointCloud", &bItFiEldWrapper_SMaterial_PointCloud_getter, &bItFiEldWrapper_SMaterial_PointCloud_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("GouraudShading", &bItFiEldWrapper_SMaterial_GouraudShading_getter, &bItFiEldWrapper_SMaterial_GouraudShading_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("Lighting", &bItFiEldWrapper_SMaterial_Lighting_getter, &bItFiEldWrapper_SMaterial_Lighting_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("ZWriteEnable", &bItFiEldWrapper_SMaterial_ZWriteEnable_getter, &bItFiEldWrapper_SMaterial_ZWriteEnable_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BackfaceCulling", &bItFiEldWrapper_SMaterial_BackfaceCulling_getter, &bItFiEldWrapper_SMaterial_BackfaceCulling_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("FrontfaceCulling", &bItFiEldWrapper_SMaterial_FrontfaceCulling_getter, &bItFiEldWrapper_SMaterial_FrontfaceCulling_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("FogEnable", &bItFiEldWrapper_SMaterial_FogEnable_getter, &bItFiEldWrapper_SMaterial_FogEnable_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("NormalizeNormals", &bItFiEldWrapper_SMaterial_NormalizeNormals_getter, &bItFiEldWrapper_SMaterial_NormalizeNormals_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("UseMipMaps", &bItFiEldWrapper_SMaterial_UseMipMaps_getter, &bItFiEldWrapper_SMaterial_UseMipMaps_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _method("getTextureMatrix", (core::matrix4 & (D::ClassType::*) (u32))&D::ClassType::getTextureMatrix);
    _d.CPGF_MD_TEMPLATE _method("getTextureMatrix", (const core::matrix4 & (D::ClassType::*) (u32) const)&D::ClassType::getTextureMatrix);
    _d.CPGF_MD_TEMPLATE _method("setTextureMatrix", &D::ClassType::setTextureMatrix);
    _d.CPGF_MD_TEMPLATE _method("getTexture", &D::ClassType::getTexture);
    _d.CPGF_MD_TEMPLATE _method("setTexture", &D::ClassType::setTexture);
    _d.CPGF_MD_TEMPLATE _method("setFlag", &D::ClassType::setFlag);
    _d.CPGF_MD_TEMPLATE _method("getFlag", &D::ClassType::getFlag);
    _d.CPGF_MD_TEMPLATE _method("isTransparent", &D::ClassType::isTransparent);
    _d.CPGF_MD_TEMPLATE _operator<SMaterial & (*)(cpgf::GMetaSelf, const SMaterial &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (SMaterial & (*) (SMaterial *, const SMaterial &))&opErAToRWrapper_SMaterial__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SMaterial &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const SMaterial *, const SMaterial &))&opErAToRWrapper_SMaterial__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SMaterial &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SMaterial *, const SMaterial &))&opErAToRWrapper_SMaterial__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
