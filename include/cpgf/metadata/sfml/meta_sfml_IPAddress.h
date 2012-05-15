// Auto generated file, don't modify.

#ifndef __META_SFML_IPADDRESS_H
#define __META_SFML_IPADDRESS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_ipaddress(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator< std::istream & (*)(std::istream &, IPAddress &)>(mopHolder >> mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >());
    _d.CPGF_MD_TEMPLATE _operator< std::ostream & (*)(std::ostream &, const IPAddress &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0>, cpgf::GMetaRuleCopyConstReference<1> >());
}


template <typename D>
void buildMetaClass_IPAddress(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const char *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint8, Uint8, Uint8, Uint8)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint32)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("LocalHost", _r), &D::ClassType::LocalHost);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ToString", _r), &D::ClassType::ToString);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ToInteger", _r), &D::ClassType::ToInteger);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLocalAddress", _r), &D::ClassType::GetLocalAddress);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPublicAddress", _r), &D::ClassType::GetPublicAddress)
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const IPAddress &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
