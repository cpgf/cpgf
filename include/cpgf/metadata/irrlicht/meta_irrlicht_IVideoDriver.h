// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IVIDEODRIVER_H
#define __META_IRRLICHT_IVIDEODRIVER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::scene;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ivideodriver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("FogTypeNames", &FogTypeNames);
    _d.CPGF_MD_TEMPLATE _enum<E_TRANSFORMATION_STATE>("E_TRANSFORMATION_STATE")
        ._element("ETS_VIEW", irr::video::ETS_VIEW)
        ._element("ETS_WORLD", irr::video::ETS_WORLD)
        ._element("ETS_PROJECTION", irr::video::ETS_PROJECTION)
        ._element("ETS_TEXTURE_0", irr::video::ETS_TEXTURE_0)
        ._element("ETS_TEXTURE_1", irr::video::ETS_TEXTURE_1)
        ._element("ETS_TEXTURE_2", irr::video::ETS_TEXTURE_2)
        ._element("ETS_TEXTURE_3", irr::video::ETS_TEXTURE_3)
        ._element("ETS_COUNT", irr::video::ETS_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_LOST_RESOURCE>("E_LOST_RESOURCE")
        ._element("ELR_DEVICE", irr::video::ELR_DEVICE)
        ._element("ELR_TEXTURES", irr::video::ELR_TEXTURES)
        ._element("ELR_RTTS", irr::video::ELR_RTTS)
        ._element("ELR_HW_BUFFERS", irr::video::ELR_HW_BUFFERS)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_RENDER_TARGET>("E_RENDER_TARGET")
        ._element("ERT_FRAME_BUFFER", irr::video::ERT_FRAME_BUFFER)
        ._element("ERT_RENDER_TEXTURE", irr::video::ERT_RENDER_TEXTURE)
        ._element("ERT_MULTI_RENDER_TEXTURES", irr::video::ERT_MULTI_RENDER_TEXTURES)
        ._element("ERT_STEREO_LEFT_BUFFER", irr::video::ERT_STEREO_LEFT_BUFFER)
        ._element("ERT_STEREO_RIGHT_BUFFER", irr::video::ERT_STEREO_RIGHT_BUFFER)
        ._element("ERT_STEREO_BOTH_BUFFERS", irr::video::ERT_STEREO_BOTH_BUFFERS)
        ._element("ERT_AUX_BUFFER0", irr::video::ERT_AUX_BUFFER0)
        ._element("ERT_AUX_BUFFER1", irr::video::ERT_AUX_BUFFER1)
        ._element("ERT_AUX_BUFFER2", irr::video::ERT_AUX_BUFFER2)
        ._element("ERT_AUX_BUFFER3", irr::video::ERT_AUX_BUFFER3)
        ._element("ERT_AUX_BUFFER4", irr::video::ERT_AUX_BUFFER4)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_FOG_TYPE>("E_FOG_TYPE")
        ._element("EFT_FOG_EXP", irr::video::EFT_FOG_EXP)
        ._element("EFT_FOG_LINEAR", irr::video::EFT_FOG_LINEAR)
        ._element("EFT_FOG_EXP2", irr::video::EFT_FOG_EXP2)
    ;
}


inline E_RENDER_TARGET bItFiEldWrapper_IRenderTarget_TargetType_getter(IRenderTarget * self) {
    return self->TargetType;
}

inline void bItFiEldWrapper_IRenderTarget_TargetType_setter(IRenderTarget * self, E_RENDER_TARGET value) {
    self->TargetType = value;
}
inline E_COLOR_PLANE bItFiEldWrapper_IRenderTarget_ColorMask_getter(IRenderTarget * self) {
    return self->ColorMask;
}

inline void bItFiEldWrapper_IRenderTarget_ColorMask_setter(IRenderTarget * self, E_COLOR_PLANE value) {
    self->ColorMask = value;
}
inline E_BLEND_FACTOR bItFiEldWrapper_IRenderTarget_BlendFuncSrc_getter(IRenderTarget * self) {
    return self->BlendFuncSrc;
}

inline void bItFiEldWrapper_IRenderTarget_BlendFuncSrc_setter(IRenderTarget * self, E_BLEND_FACTOR value) {
    self->BlendFuncSrc = value;
}
inline E_BLEND_FACTOR bItFiEldWrapper_IRenderTarget_BlendFuncDst_getter(IRenderTarget * self) {
    return self->BlendFuncDst;
}

inline void bItFiEldWrapper_IRenderTarget_BlendFuncDst_setter(IRenderTarget * self, E_BLEND_FACTOR value) {
    self->BlendFuncDst = value;
}
inline E_BLEND_OPERATION bItFiEldWrapper_IRenderTarget_BlendOp_getter(IRenderTarget * self) {
    return self->BlendOp;
}

inline void bItFiEldWrapper_IRenderTarget_BlendOp_setter(IRenderTarget * self, E_BLEND_OPERATION value) {
    self->BlendOp = value;
}


inline bool opErAToRWrapper_IRenderTarget__opNotEqual(const IRenderTarget * self, const IRenderTarget & other) {
    return (*self) != other;
}


template <typename D>
void buildMetaClass_IRenderTarget(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (ITexture *, E_COLOR_PLANE, E_BLEND_FACTOR, E_BLEND_FACTOR, E_BLEND_OPERATION)>()
        ._default(copyVariantFromCopyable(EBO_NONE))
        ._default(copyVariantFromCopyable(EBF_ONE_MINUS_SRC_ALPHA))
        ._default(copyVariantFromCopyable(EBF_ONE))
        ._default(copyVariantFromCopyable(ECP_ALL))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (E_RENDER_TARGET, E_COLOR_PLANE, E_BLEND_FACTOR, E_BLEND_FACTOR, E_BLEND_OPERATION)>()
        ._default(copyVariantFromCopyable(EBO_NONE))
        ._default(copyVariantFromCopyable(EBF_ONE_MINUS_SRC_ALPHA))
        ._default(copyVariantFromCopyable(EBF_ONE))
        ._default(copyVariantFromCopyable(ECP_ALL))
    ;
    _d.CPGF_MD_TEMPLATE _field("RenderTexture", &D::ClassType::RenderTexture);
    _d.CPGF_MD_TEMPLATE _property("TargetType", &bItFiEldWrapper_IRenderTarget_TargetType_getter, &bItFiEldWrapper_IRenderTarget_TargetType_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("ColorMask", &bItFiEldWrapper_IRenderTarget_ColorMask_getter, &bItFiEldWrapper_IRenderTarget_ColorMask_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BlendFuncSrc", &bItFiEldWrapper_IRenderTarget_BlendFuncSrc_getter, &bItFiEldWrapper_IRenderTarget_BlendFuncSrc_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BlendFuncDst", &bItFiEldWrapper_IRenderTarget_BlendFuncDst_getter, &bItFiEldWrapper_IRenderTarget_BlendFuncDst_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BlendOp", &bItFiEldWrapper_IRenderTarget_BlendOp_getter, &bItFiEldWrapper_IRenderTarget_BlendOp_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IRenderTarget &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const IRenderTarget *, const IRenderTarget &))&opErAToRWrapper_IRenderTarget__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


template <typename D>
void buildMetaClass_SOverrideMaterial(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Material", &D::ClassType::Material);
    _d.CPGF_MD_TEMPLATE _field("EnableFlags", &D::ClassType::EnableFlags);
    _d.CPGF_MD_TEMPLATE _field("EnablePasses", &D::ClassType::EnablePasses);
    _d.CPGF_MD_TEMPLATE _field("Enabled", &D::ClassType::Enabled);
    _d.CPGF_MD_TEMPLATE _method("apply", &D::ClassType::apply);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
