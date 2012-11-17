// Auto generated file, don't modify.

#ifndef __META_SFML_SOCKETTCP_H
#define __META_SFML_SOCKETTCP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


inline bool opErAToRWrapper_SocketTCP__opEqual(const SocketTCP * self, const SocketTCP & Other) {
    return (*self) == Other;
}
inline bool opErAToRWrapper_SocketTCP__opNotEqual(const SocketTCP * self, const SocketTCP & Other) {
    return (*self) != Other;
}
inline bool opErAToRWrapper_SocketTCP__opLess(const SocketTCP * self, const SocketTCP & Other) {
    return (*self) < Other;
}


template <typename D>
void buildMetaClass_SocketTCP(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("SetBlocking", &D::ClassType::SetBlocking);
    _d.CPGF_MD_TEMPLATE _method("Connect", &D::ClassType::Connect, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("Listen", &D::ClassType::Listen);
    _d.CPGF_MD_TEMPLATE _method("Accept", &D::ClassType::Accept)
        ._default(copyVariantFromCopyable((IPAddress *)NULL))
    ;
    _d.CPGF_MD_TEMPLATE _method("Send", (Socket::Status (D::ClassType::*) (const char *, std::size_t))&D::ClassType::Send);
    _d.CPGF_MD_TEMPLATE _method("Receive", (Socket::Status (D::ClassType::*) (char *, std::size_t, std::size_t &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method("Send", (Socket::Status (D::ClassType::*) (Packet &))&D::ClassType::Send);
    _d.CPGF_MD_TEMPLATE _method("Receive", (Socket::Status (D::ClassType::*) (Packet &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method("Close", &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method("IsValid", &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SocketTCP *, const SocketTCP &))&opErAToRWrapper_SocketTCP__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const SocketTCP *, const SocketTCP &))&opErAToRWrapper_SocketTCP__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder < mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const SocketTCP *, const SocketTCP &))&opErAToRWrapper_SocketTCP__opLess, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
