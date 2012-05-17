// Auto generated file, don't modify.

#ifndef __META_SFML_SOCKETUDP_H
#define __META_SFML_SOCKETUDP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SocketUDP(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBlocking", _r), &D::ClassType::SetBlocking);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Bind", _r), &D::ClassType::Bind);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Unbind", _r), &D::ClassType::Unbind);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Send", _r), (Socket::Status (D::ClassType::*) (const char *, std::size_t, const IPAddress &, unsigned short))&D::ClassType::Send, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("Receive", _r), (Socket::Status (D::ClassType::*) (char *, std::size_t, std::size_t &, IPAddress &, unsigned short &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Send", _r), (Socket::Status (D::ClassType::*) (Packet &, const IPAddress &, unsigned short))&D::ClassType::Send, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("Receive", _r), (Socket::Status (D::ClassType::*) (Packet &, IPAddress &, unsigned short &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Close", _r), &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetPort", _r), &D::ClassType::GetPort);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketUDP &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketUDP &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketUDP &)>(mopHolder < mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
