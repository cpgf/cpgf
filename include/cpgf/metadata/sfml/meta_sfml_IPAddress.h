// Auto generated file, don't modify.

#ifndef __META_SFML_IPADDRESS_H
#define __META_SFML_IPADDRESS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_ipaddress(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator< std::istream & (*)(std::istream &, IPAddress &)>(mopHolder >> mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >());
    _d.CPGF_MD_TEMPLATE _operator< std::ostream & (*)(std::ostream &, const IPAddress &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0>, cpgf::GMetaRuleCopyConstReference<1> >());
}


inline bool opErAToRWrapper_IPAddress__opEqual(const IPAddress * self, const IPAddress & Other) {
    return (*self) == Other;
}
inline bool opErAToRWrapper_IPAddress__opNotEqual(const IPAddress * self, const IPAddress & Other) {
    return (*self) != Other;
}
inline bool opErAToRWrapper_IPAddress__opLess(const IPAddress * self, const IPAddress & Other) {
    return (*self) < Other;
}
inline bool opErAToRWrapper_IPAddress__opGreater(const IPAddress * self, const IPAddress & Other) {
    return (*self) > Other;
}
inline bool opErAToRWrapper_IPAddress__opLessEqual(const IPAddress * self, const IPAddress & Other) {
    return (*self) <= Other;
}
inline bool opErAToRWrapper_IPAddress__opGreaterEqual(const IPAddress * self, const IPAddress & Other) {
    return (*self) >= Other;
}


template <typename D>
void buildMetaClass_IPAddress(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const char *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint8, Uint8, Uint8, Uint8)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint32)>();
//    _d.CPGF_MD_TEMPLATE _field("LocalHost", &D::ClassType::LocalHost);
    _d.CPGF_MD_TEMPLATE _method("IsValid", &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method("ToString", &D::ClassType::ToString);
    _d.CPGF_MD_TEMPLATE _method("ToInteger", &D::ClassType::ToInteger);
    _d.CPGF_MD_TEMPLATE _method("GetLocalAddress", &D::ClassType::GetLocalAddress);
    _d.CPGF_MD_TEMPLATE _method("GetPublicAddress", &D::ClassType::GetPublicAddress)
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opLess, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opGreater, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLessEqual", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opLessEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const IPAddress *, const IPAddress &))&opErAToRWrapper_IPAddress__opGreaterEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
