// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SCOLOR_H
#define __META_IRRLICHT_SCOLOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_scolor(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("RGBA16", (u16 (*) (u32, u32, u32, u32))&RGBA16)
        ._default(copyVariantFromCopyable(0xFF))
    ;
    _d.CPGF_MD_TEMPLATE _method("RGB16", (u16 (*) (u32, u32, u32))&RGB16);
    _d.CPGF_MD_TEMPLATE _method("RGB16from16", (u16 (*) (u16, u16, u16))&RGB16from16);
    _d.CPGF_MD_TEMPLATE _method("X8R8G8B8toA1R5G5B5", (u16 (*) (u32))&X8R8G8B8toA1R5G5B5);
    _d.CPGF_MD_TEMPLATE _method("A8R8G8B8toA1R5G5B5", (u16 (*) (u32))&A8R8G8B8toA1R5G5B5);
    _d.CPGF_MD_TEMPLATE _method("A8R8G8B8toR5G6B5", (u16 (*) (u32))&A8R8G8B8toR5G6B5);
    _d.CPGF_MD_TEMPLATE _method("A1R5G5B5toA8R8G8B8", (u32 (*) (u16))&A1R5G5B5toA8R8G8B8);
    _d.CPGF_MD_TEMPLATE _method("R5G6B5toA8R8G8B8", (u32 (*) (u16))&R5G6B5toA8R8G8B8);
    _d.CPGF_MD_TEMPLATE _method("R5G6B5toA1R5G5B5", (u16 (*) (u16))&R5G6B5toA1R5G5B5);
    _d.CPGF_MD_TEMPLATE _method("A1R5G5B5toR5G6B5", (u16 (*) (u16))&A1R5G5B5toR5G6B5);
    _d.CPGF_MD_TEMPLATE _method("getAlpha", (u32 (*) (u16))&getAlpha);
    _d.CPGF_MD_TEMPLATE _method("getRed", (u32 (*) (u16))&getRed);
    _d.CPGF_MD_TEMPLATE _method("getGreen", (u32 (*) (u16))&getGreen);
    _d.CPGF_MD_TEMPLATE _method("getBlue", (u32 (*) (u16))&getBlue);
    _d.CPGF_MD_TEMPLATE _method("getAverage", (s32 (*) (s16))&getAverage);
    _d.CPGF_MD_TEMPLATE _enum<ECOLOR_FORMAT>("ECOLOR_FORMAT")
        ._element("ECF_A1R5G5B5", irr::video::ECF_A1R5G5B5)
        ._element("ECF_R5G6B5", irr::video::ECF_R5G6B5)
        ._element("ECF_R8G8B8", irr::video::ECF_R8G8B8)
        ._element("ECF_A8R8G8B8", irr::video::ECF_A8R8G8B8)
        ._element("ECF_R16F", irr::video::ECF_R16F)
        ._element("ECF_G16R16F", irr::video::ECF_G16R16F)
        ._element("ECF_A16B16G16R16F", irr::video::ECF_A16B16G16R16F)
        ._element("ECF_R32F", irr::video::ECF_R32F)
        ._element("ECF_G32R32F", irr::video::ECF_G32R32F)
        ._element("ECF_A32B32G32R32F", irr::video::ECF_A32B32G32R32F)
        ._element("ECF_UNKNOWN", irr::video::ECF_UNKNOWN)
    ;
}


inline bool opErAToRWrapper_SColor__opEqual(const SColor * self, const SColor & other) {
    return (*self) == other;
}
inline bool opErAToRWrapper_SColor__opNotEqual(const SColor * self, const SColor & other) {
    return (*self) != other;
}
inline bool opErAToRWrapper_SColor__opLess(const SColor * self, const SColor & other) {
    return (*self) < other;
}
inline SColor opErAToRWrapper_SColor__opAdd(const SColor * self, const SColor & other) {
    return (*self) + other;
}


template <typename D>
void buildMetaClass_SColor(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32, u32, u32, u32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32)>();
    _d.CPGF_MD_TEMPLATE _field("color", &D::ClassType::color);
    _d.CPGF_MD_TEMPLATE _method("getAlpha", &D::ClassType::getAlpha);
    _d.CPGF_MD_TEMPLATE _method("getRed", &D::ClassType::getRed);
    _d.CPGF_MD_TEMPLATE _method("getGreen", &D::ClassType::getGreen);
    _d.CPGF_MD_TEMPLATE _method("getBlue", &D::ClassType::getBlue);
    _d.CPGF_MD_TEMPLATE _method("getLightness", &D::ClassType::getLightness);
    _d.CPGF_MD_TEMPLATE _method("getLuminance", &D::ClassType::getLuminance);
    _d.CPGF_MD_TEMPLATE _method("getAverage", &D::ClassType::getAverage);
    _d.CPGF_MD_TEMPLATE _method("setAlpha", &D::ClassType::setAlpha);
    _d.CPGF_MD_TEMPLATE _method("setRed", &D::ClassType::setRed);
    _d.CPGF_MD_TEMPLATE _method("setGreen", &D::ClassType::setGreen);
    _d.CPGF_MD_TEMPLATE _method("setBlue", &D::ClassType::setBlue);
    _d.CPGF_MD_TEMPLATE _method("toA1R5G5B5", &D::ClassType::toA1R5G5B5);
    _d.CPGF_MD_TEMPLATE _method("toOpenGLColor", &D::ClassType::toOpenGLColor);
    _d.CPGF_MD_TEMPLATE _method("set", (void (D::ClassType::*) (u32, u32, u32, u32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (void (D::ClassType::*) (u32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated_quadratic", &D::ClassType::getInterpolated_quadratic, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setData", &D::ClassType::setData);
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SColor &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SColor *, const SColor &))&opErAToRWrapper_SColor__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SColor &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const SColor *, const SColor &))&opErAToRWrapper_SColor__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SColor &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const SColor *, const SColor &))&opErAToRWrapper_SColor__opLess, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<SColor (*)(const cpgf::GMetaSelf &, const SColor &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (SColor (*) (const SColor *, const SColor &))&opErAToRWrapper_SColor__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


template <typename D>
void buildMetaClass_SColorHSL(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32)>()
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _field("Hue", &D::ClassType::Hue);
    _d.CPGF_MD_TEMPLATE _field("Saturation", &D::ClassType::Saturation);
    _d.CPGF_MD_TEMPLATE _field("Luminance", &D::ClassType::Luminance);
    _d.CPGF_MD_TEMPLATE _method("fromRGB", &D::ClassType::fromRGB, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("toRGB", &D::ClassType::toRGB);
}


template <typename D>
void buildMetaClass_SColorf(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32)>()
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (SColor)>();
    _d.CPGF_MD_TEMPLATE _field("r", &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _field("g", &D::ClassType::g);
    _d.CPGF_MD_TEMPLATE _field("b", &D::ClassType::b);
    _d.CPGF_MD_TEMPLATE _field("a", &D::ClassType::a);
    _d.CPGF_MD_TEMPLATE _method("toSColor", &D::ClassType::toSColor);
    _d.CPGF_MD_TEMPLATE _method("set", (void (D::ClassType::*) (f32, f32, f32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (void (D::ClassType::*) (f32, f32, f32, f32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated_quadratic", &D::ClassType::getInterpolated_quadratic, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setColorComponentValue", &D::ClassType::setColorComponentValue);
    _d.CPGF_MD_TEMPLATE _method("getAlpha", &D::ClassType::getAlpha);
    _d.CPGF_MD_TEMPLATE _method("getRed", &D::ClassType::getRed);
    _d.CPGF_MD_TEMPLATE _method("getGreen", &D::ClassType::getGreen);
    _d.CPGF_MD_TEMPLATE _method("getBlue", &D::ClassType::getBlue);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
