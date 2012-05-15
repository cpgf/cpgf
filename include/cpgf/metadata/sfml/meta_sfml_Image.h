// Auto generated file, don't modify.

#ifndef __META_SFML_IMAGE_H
#define __META_SFML_IMAGE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Image(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Image &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, const Color &)>()
        ._default(copyVariantFromCopyable(Color(0, 0, 0, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, const Uint8 *)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromFile", _r), &D::ClassType::LoadFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromMemory", _r), &D::ClassType::LoadFromMemory);
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromPixels", _r), &D::ClassType::LoadFromPixels);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SaveToFile", _r), &D::ClassType::SaveToFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("Create", _r), &D::ClassType::Create)
        ._default(copyVariantFromCopyable(Color(0, 0, 0, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateMaskFromColor", _r), &D::ClassType::CreateMaskFromColor)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Copy", _r), &D::ClassType::Copy)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(IntRect(0, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("CopyScreen", _r), &D::ClassType::CopyScreen, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >())
        ._default(copyVariantFromCopyable(IntRect(0, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPixel", _r), &D::ClassType::SetPixel);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPixel", _r), &D::ClassType::GetPixel);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPixelsPtr", _r), &D::ClassType::GetPixelsPtr);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Bind", _r), &D::ClassType::Bind);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSmooth", _r), &D::ClassType::SetSmooth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWidth", _r), &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetHeight", _r), &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsSmooth", _r), &D::ClassType::IsSmooth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetTexCoords", _r), &D::ClassType::GetTexCoords);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetValidTextureSize", _r), &D::ClassType::GetValidTextureSize);
    _d.CPGF_MD_TEMPLATE _operator<Image & (*)(cpgf::GMetaSelf, const Image &)>(mopHolder = mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
