// Auto generated file, don't modify.

#ifndef __META_SFML_VIDEOMODE_H
#define __META_SFML_VIDEOMODE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


inline bool opErAToRWrapper_VideoMode__opEqual(const VideoMode * self, const VideoMode & Other) {
    return (*self) == Other;
}
inline bool opErAToRWrapper_VideoMode__opNotEqual(const VideoMode * self, const VideoMode & Other) {
    return (*self) != Other;
}


template <typename D>
void buildMetaClass_VideoMode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int, unsigned int, unsigned int)>()
        ._default(copyVariantFromCopyable(32))
    ;
    _d.CPGF_MD_TEMPLATE _field("Width", &D::ClassType::Width);
    _d.CPGF_MD_TEMPLATE _field("Height", &D::ClassType::Height);
    _d.CPGF_MD_TEMPLATE _field("BitsPerPixel", &D::ClassType::BitsPerPixel);
    _d.CPGF_MD_TEMPLATE _method("IsValid", &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method("GetDesktopMode", &D::ClassType::GetDesktopMode);
    _d.CPGF_MD_TEMPLATE _method("GetMode", &D::ClassType::GetMode);
    _d.CPGF_MD_TEMPLATE _method("GetModesCount", &D::ClassType::GetModesCount);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const VideoMode &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const VideoMode *, const VideoMode &))&opErAToRWrapper_VideoMode__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const VideoMode &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const VideoMode *, const VideoMode &))&opErAToRWrapper_VideoMode__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
