// Auto generated file, don't modify.

#ifndef __META_SFML_IMAGE_H
#define __META_SFML_IMAGE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


inline Image & opErAToRWrapper_Image__opAssign(Image * self, const Image & Other) {
    return (*self) = Other;
}


template <typename D>
void buildMetaClass_Image(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Image &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, const Color &)>()
        ._default(copyVariantFromCopyable(Color(0, 0, 0, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, const Uint8 *)>();
    _d.CPGF_MD_TEMPLATE _method("LoadFromFile", &D::ClassType::LoadFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("LoadFromMemory", &D::ClassType::LoadFromMemory);
    _d.CPGF_MD_TEMPLATE _method("LoadFromPixels", &D::ClassType::LoadFromPixels);
    _d.CPGF_MD_TEMPLATE _method("SaveToFile", &D::ClassType::SaveToFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("Create", &D::ClassType::Create)
        ._default(copyVariantFromCopyable(Color(0, 0, 0, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("CreateMaskFromColor", &D::ClassType::CreateMaskFromColor)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("Copy", &D::ClassType::Copy)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(IntRect(0, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("CopyScreen", &D::ClassType::CopyScreen, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >())
        ._default(copyVariantFromCopyable(IntRect(0, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("SetPixel", &D::ClassType::SetPixel);
    _d.CPGF_MD_TEMPLATE _method("GetPixel", &D::ClassType::GetPixel);
    _d.CPGF_MD_TEMPLATE _method("GetPixelsPtr", &D::ClassType::GetPixelsPtr);
    _d.CPGF_MD_TEMPLATE _method("Bind", &D::ClassType::Bind);
    _d.CPGF_MD_TEMPLATE _method("SetSmooth", &D::ClassType::SetSmooth);
    _d.CPGF_MD_TEMPLATE _method("GetWidth", &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method("GetHeight", &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method("IsSmooth", &D::ClassType::IsSmooth);
    _d.CPGF_MD_TEMPLATE _method("GetTexCoords", &D::ClassType::GetTexCoords);
    _d.CPGF_MD_TEMPLATE _method("GetValidTextureSize", &D::ClassType::GetValidTextureSize);
    _d.CPGF_MD_TEMPLATE _operator<Image & (*)(cpgf::GMetaSelf, const Image &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (Image & (*) (Image *, const Image &))&opErAToRWrapper_Image__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
