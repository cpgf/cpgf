// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SMATERIALLAYER_H
#define __META_IRRLICHT_SMATERIALLAYER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_smateriallayer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("aTextureClampNames", &aTextureClampNames);
    _d.CPGF_MD_TEMPLATE _enum<E_TEXTURE_CLAMP>("E_TEXTURE_CLAMP")
        ._element("ETC_REPEAT", irr::video::ETC_REPEAT)
        ._element("ETC_CLAMP", irr::video::ETC_CLAMP)
        ._element("ETC_CLAMP_TO_EDGE", irr::video::ETC_CLAMP_TO_EDGE)
        ._element("ETC_CLAMP_TO_BORDER", irr::video::ETC_CLAMP_TO_BORDER)
        ._element("ETC_MIRROR", irr::video::ETC_MIRROR)
        ._element("ETC_MIRROR_CLAMP", irr::video::ETC_MIRROR_CLAMP)
        ._element("ETC_MIRROR_CLAMP_TO_EDGE", irr::video::ETC_MIRROR_CLAMP_TO_EDGE)
        ._element("ETC_MIRROR_CLAMP_TO_BORDER", irr::video::ETC_MIRROR_CLAMP_TO_BORDER)
    ;
}


inline u8 bItFiEldWrapper_SMaterialLayer_TextureWrapU_getter(SMaterialLayer * self) {
    return self->TextureWrapU;
}

inline void bItFiEldWrapper_SMaterialLayer_TextureWrapU_setter(SMaterialLayer * self, u8 value) {
    self->TextureWrapU = value;
}
inline u8 bItFiEldWrapper_SMaterialLayer_TextureWrapV_getter(SMaterialLayer * self) {
    return self->TextureWrapV;
}

inline void bItFiEldWrapper_SMaterialLayer_TextureWrapV_setter(SMaterialLayer * self, u8 value) {
    self->TextureWrapV = value;
}
inline bool bItFiEldWrapper_SMaterialLayer_BilinearFilter_getter(SMaterialLayer * self) {
    return self->BilinearFilter;
}

inline void bItFiEldWrapper_SMaterialLayer_BilinearFilter_setter(SMaterialLayer * self, bool value) {
    self->BilinearFilter = value;
}
inline bool bItFiEldWrapper_SMaterialLayer_TrilinearFilter_getter(SMaterialLayer * self) {
    return self->TrilinearFilter;
}

inline void bItFiEldWrapper_SMaterialLayer_TrilinearFilter_setter(SMaterialLayer * self, bool value) {
    self->TrilinearFilter = value;
}


inline SMaterialLayer & opErAToRWrapper_SMaterialLayer__opAssign(SMaterialLayer * self, const SMaterialLayer & other) {
    return (*self) = other;
}
inline bool opErAToRWrapper_SMaterialLayer__opNotEqual(const SMaterialLayer * self, const SMaterialLayer & b) {
    return (*self) != b;
}
inline bool opErAToRWrapper_SMaterialLayer__opEqual(const SMaterialLayer * self, const SMaterialLayer & b) {
    return (*self) == b;
}


template <typename D>
void buildMetaClass_SMaterialLayer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SMaterialLayer &)>();
    _d.CPGF_MD_TEMPLATE _field("Texture", &D::ClassType::Texture);
    _d.CPGF_MD_TEMPLATE _property("TextureWrapU", &bItFiEldWrapper_SMaterialLayer_TextureWrapU_getter, &bItFiEldWrapper_SMaterialLayer_TextureWrapU_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("TextureWrapV", &bItFiEldWrapper_SMaterialLayer_TextureWrapV_getter, &bItFiEldWrapper_SMaterialLayer_TextureWrapV_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BilinearFilter", &bItFiEldWrapper_SMaterialLayer_BilinearFilter_getter, &bItFiEldWrapper_SMaterialLayer_BilinearFilter_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("TrilinearFilter", &bItFiEldWrapper_SMaterialLayer_TrilinearFilter_getter, &bItFiEldWrapper_SMaterialLayer_TrilinearFilter_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _field("AnisotropicFilter", &D::ClassType::AnisotropicFilter);
    _d.CPGF_MD_TEMPLATE _field("LODBias", &D::ClassType::LODBias);
    _d.CPGF_MD_TEMPLATE _method("getTextureMatrix", (core::matrix4 & (D::ClassType::*) ())&D::ClassType::getTextureMatrix);
    _d.CPGF_MD_TEMPLATE _method("getTextureMatrix", (const core::matrix4 & (D::ClassType::*) () const)&D::ClassType::getTextureMatrix);
    _d.CPGF_MD_TEMPLATE _method("setTextureMatrix", &D::ClassType::setTextureMatrix);
    _d.CPGF_MD_TEMPLATE _operator<SMaterialLayer & (*)(cpgf::GMetaSelf, const SMaterialLayer &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (SMaterialLayer & (*) (SMaterialLayer *, const SMaterialLayer &))&opErAToRWrapper_SMaterialLayer__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SMaterialLayer &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const SMaterialLayer *, const SMaterialLayer &))&opErAToRWrapper_SMaterialLayer__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SMaterialLayer &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SMaterialLayer *, const SMaterialLayer &))&opErAToRWrapper_SMaterialLayer__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
