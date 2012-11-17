// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IQ3SHADER_H
#define __META_IRRLICHT_IQ3SHADER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;
using namespace irr::scene::quake3;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iq3shader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("isEqual", (s16 (*) (const core::stringc &, u32 &, const c8 *[], u16))&isEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getAsFloat", (f32 (*) (const core::stringc &, u32 &))&getAsFloat, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getAsVector3df", (core::vector3df (*) (const core::stringc &, u32 &))&getAsVector3df, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getAsStringList", (void (*) (tStringList &, s32, const core::stringc &, u32 &))&getAsStringList, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method("getCullingFunction", (bool (*) (const core::stringc &))&getCullingFunction, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDepthFunction", (u8 (*) (const core::stringc &))&getDepthFunction, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getBlendFunc", (void (*) (const core::stringc &, SBlendFunc &))&getBlendFunc, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getMD3Normal", (core::vector3df (*) (u32, u32))&getMD3Normal);
    _d.CPGF_MD_TEMPLATE _method("getModifierFunc", (void (*) (SModifierFunction &, const core::stringc &, u32 &))&getModifierFunc, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("dumpVarGroup", (void (*) (core::stringc &, const SVarGroup *, s32))&dumpVarGroup);
    _d.CPGF_MD_TEMPLATE _method("dumpShader", (core::stringc & (*) (core::stringc &, const IShader *, bool))&dumpShader)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getTextures", (void (*) (tTexArray &, const core::stringc &, u32 &, io::IFileSystem *, video::IVideoDriver *))&getTextures, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _enum<eQ3MeshIndex>("eQ3MeshIndex")
        ._element("E_Q3_MESH_GEOMETRY", irr::scene::quake3::E_Q3_MESH_GEOMETRY)
        ._element("E_Q3_MESH_ITEMS", irr::scene::quake3::E_Q3_MESH_ITEMS)
        ._element("E_Q3_MESH_BILLBOARD", irr::scene::quake3::E_Q3_MESH_BILLBOARD)
        ._element("E_Q3_MESH_FOG", irr::scene::quake3::E_Q3_MESH_FOG)
        ._element("E_Q3_MESH_UNRESOLVED", irr::scene::quake3::E_Q3_MESH_UNRESOLVED)
        ._element("E_Q3_MESH_SIZE", irr::scene::quake3::E_Q3_MESH_SIZE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<eQ3ModifierFunction>("eQ3ModifierFunction")
        ._element("TCMOD", irr::scene::quake3::TCMOD)
        ._element("DEFORMVERTEXES", irr::scene::quake3::DEFORMVERTEXES)
        ._element("RGBGEN", irr::scene::quake3::RGBGEN)
        ._element("TCGEN", irr::scene::quake3::TCGEN)
        ._element("MAP", irr::scene::quake3::MAP)
        ._element("ALPHAGEN", irr::scene::quake3::ALPHAGEN)
        ._element("FUNCTION2", irr::scene::quake3::FUNCTION2)
        ._element("SCROLL", irr::scene::quake3::SCROLL)
        ._element("SCALE", irr::scene::quake3::SCALE)
        ._element("ROTATE", irr::scene::quake3::ROTATE)
        ._element("STRETCH", irr::scene::quake3::STRETCH)
        ._element("TURBULENCE", irr::scene::quake3::TURBULENCE)
        ._element("WAVE", irr::scene::quake3::WAVE)
        ._element("IDENTITY", irr::scene::quake3::IDENTITY)
        ._element("VERTEX", irr::scene::quake3::VERTEX)
        ._element("TEXTURE", irr::scene::quake3::TEXTURE)
        ._element("LIGHTMAP", irr::scene::quake3::LIGHTMAP)
        ._element("ENVIRONMENT", irr::scene::quake3::ENVIRONMENT)
        ._element("DOLLAR_LIGHTMAP", irr::scene::quake3::DOLLAR_LIGHTMAP)
        ._element("BULGE", irr::scene::quake3::BULGE)
        ._element("AUTOSPRITE", irr::scene::quake3::AUTOSPRITE)
        ._element("AUTOSPRITE2", irr::scene::quake3::AUTOSPRITE2)
        ._element("TRANSFORM", irr::scene::quake3::TRANSFORM)
        ._element("EXACTVERTEX", irr::scene::quake3::EXACTVERTEX)
        ._element("CONSTANT", irr::scene::quake3::CONSTANT)
        ._element("LIGHTINGSPECULAR", irr::scene::quake3::LIGHTINGSPECULAR)
        ._element("MOVE", irr::scene::quake3::MOVE)
        ._element("NORMAL", irr::scene::quake3::NORMAL)
        ._element("IDENTITYLIGHTING", irr::scene::quake3::IDENTITYLIGHTING)
        ._element("WAVE_MODIFIER_FUNCTION", irr::scene::quake3::WAVE_MODIFIER_FUNCTION)
        ._element("SINUS", irr::scene::quake3::SINUS)
        ._element("COSINUS", irr::scene::quake3::COSINUS)
        ._element("SQUARE", irr::scene::quake3::SQUARE)
        ._element("TRIANGLE", irr::scene::quake3::TRIANGLE)
        ._element("SAWTOOTH", irr::scene::quake3::SAWTOOTH)
        ._element("SAWTOOTH_INVERSE", irr::scene::quake3::SAWTOOTH_INVERSE)
        ._element("NOISE", irr::scene::quake3::NOISE)
        ._element("UNKNOWN", irr::scene::quake3::UNKNOWN)
    ;
}


inline void opErAToRWrapper_IShader__opAssign(IShader * self, const IShader & other) {
    (*self) = other;
}
inline bool opErAToRWrapper_IShader__opEqual(const IShader * self, const IShader & other) {
    return (*self) == other;
}
inline bool opErAToRWrapper_IShader__opLess(const IShader * self, const IShader & other) {
    return (*self) < other;
}


template <typename D>
void buildMetaClass_IShader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("ID", &D::ClassType::ID);
    _d.CPGF_MD_TEMPLATE _field("VarGroup", &D::ClassType::VarGroup);
    _d.CPGF_MD_TEMPLATE _field("name", &D::ClassType::name);
    _d.CPGF_MD_TEMPLATE _method("getGroupSize", &D::ClassType::getGroupSize);
    _d.CPGF_MD_TEMPLATE _method("getGroup", &D::ClassType::getGroup);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const IShader &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (void (*) (IShader *, const IShader &))&opErAToRWrapper_IShader__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IShader &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const IShader *, const IShader &))&opErAToRWrapper_IShader__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IShader &)>(mopHolder < mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const IShader *, const IShader &))&opErAToRWrapper_IShader__opLess, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


template <typename D>
void buildMetaClass_IShaderManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <typename D>
void buildMetaClass_Noiser(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("get", &D::ClassType::get);
}


template <typename D>
void buildMetaClass_Q3LevelLoadParameter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("defaultLightMapMaterial", &D::ClassType::defaultLightMapMaterial);
    _d.CPGF_MD_TEMPLATE _field("defaultModulate", &D::ClassType::defaultModulate);
    _d.CPGF_MD_TEMPLATE _field("defaultFilter", &D::ClassType::defaultFilter);
    _d.CPGF_MD_TEMPLATE _field("patchTesselation", &D::ClassType::patchTesselation);
    _d.CPGF_MD_TEMPLATE _field("verbose", &D::ClassType::verbose);
    _d.CPGF_MD_TEMPLATE _field("startTime", &D::ClassType::startTime);
    _d.CPGF_MD_TEMPLATE _field("endTime", &D::ClassType::endTime);
    _d.CPGF_MD_TEMPLATE _field("mergeShaderBuffer", &D::ClassType::mergeShaderBuffer);
    _d.CPGF_MD_TEMPLATE _field("cleanUnResolvedMeshes", &D::ClassType::cleanUnResolvedMeshes);
    _d.CPGF_MD_TEMPLATE _field("loadAllShaders", &D::ClassType::loadAllShaders);
    _d.CPGF_MD_TEMPLATE _field("loadSkyShader", &D::ClassType::loadSkyShader);
    _d.CPGF_MD_TEMPLATE _field("alpharef", &D::ClassType::alpharef);
    _d.CPGF_MD_TEMPLATE _field("swapLump", &D::ClassType::swapLump);
    _d.CPGF_MD_TEMPLATE _field("swapHeader", &D::ClassType::swapHeader);
    _d.CPGF_MD_TEMPLATE _field("scriptDir", &D::ClassType::scriptDir);
}


template <typename D>
void buildMetaClass_SBlendFunc(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::E_MODULATE_FUNC)>();
    _d.CPGF_MD_TEMPLATE _field("type", &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field("modulate", &D::ClassType::modulate);
    _d.CPGF_MD_TEMPLATE _field("param0", &D::ClassType::param0);
    _d.CPGF_MD_TEMPLATE _field("isTransparent", &D::ClassType::isTransparent);
}


template <typename D>
void buildMetaClass_SModifierFunction(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("masterfunc0", &D::ClassType::masterfunc0);
    _d.CPGF_MD_TEMPLATE _field("masterfunc1", &D::ClassType::masterfunc1);
    _d.CPGF_MD_TEMPLATE _field("func", &D::ClassType::func);
    _d.CPGF_MD_TEMPLATE _field("tcgen", &D::ClassType::tcgen);
    _d.CPGF_MD_TEMPLATE _field("rgbgen", &D::ClassType::rgbgen);
    _d.CPGF_MD_TEMPLATE _field("alphagen", &D::ClassType::alphagen);
    _d.CPGF_MD_TEMPLATE _field("base", &D::ClassType::base);
    _d.CPGF_MD_TEMPLATE _field("bulgewidth", &D::ClassType::bulgewidth);
    _d.CPGF_MD_TEMPLATE _field("amp", &D::ClassType::amp);
    _d.CPGF_MD_TEMPLATE _field("bulgeheight", &D::ClassType::bulgeheight);
    _d.CPGF_MD_TEMPLATE _field("phase", &D::ClassType::phase);
    _d.CPGF_MD_TEMPLATE _field("frequency", &D::ClassType::frequency);
    _d.CPGF_MD_TEMPLATE _field("bulgespeed", &D::ClassType::bulgespeed);
    _d.CPGF_MD_TEMPLATE _field("wave", &D::ClassType::wave);
    _d.CPGF_MD_TEMPLATE _field("div", &D::ClassType::div);
    _d.CPGF_MD_TEMPLATE _field("x", &D::ClassType::x);
    _d.CPGF_MD_TEMPLATE _field("y", &D::ClassType::y);
    _d.CPGF_MD_TEMPLATE _field("z", &D::ClassType::z);
    _d.CPGF_MD_TEMPLATE _field("count", &D::ClassType::count);
    _d.CPGF_MD_TEMPLATE _method("evaluate", &D::ClassType::evaluate);
}


template <typename D>
void buildMetaClass_SVarGroup(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Variable", &D::ClassType::Variable);
    _d.CPGF_MD_TEMPLATE _method("isDefined", &D::ClassType::isDefined)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("get", &D::ClassType::get, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("set", &D::ClassType::set)
        ._default(copyVariantFromCopyable(0))
    ;
}


template <typename D>
void buildMetaClass_SVarGroupList(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("VariableGroup", &D::ClassType::VariableGroup);
}


inline bool opErAToRWrapper_SVariable__opEqual(const SVariable * self, const SVariable & other) {
    return (*self) == other;
}
inline bool opErAToRWrapper_SVariable__opLess(const SVariable * self, const SVariable & other) {
    return (*self) < other;
}


template <typename D>
void buildMetaClass_SVariable(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const c8 *, const c8 *)>()
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _field("name", &D::ClassType::name);
    _d.CPGF_MD_TEMPLATE _field("content", &D::ClassType::content);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("isValid", &D::ClassType::isValid);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SVariable &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SVariable *, const SVariable &))&opErAToRWrapper_SVariable__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SVariable &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const SVariable *, const SVariable &))&opErAToRWrapper_SVariable__opLess, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
