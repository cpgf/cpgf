// Auto generated file, don't modify.

#ifndef __META_SFML_SOCKETTCP_H
#define __META_SFML_SOCKETTCP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SocketTCP(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBlocking", _r), &D::ClassType::SetBlocking);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Connect", _r), &D::ClassType::Connect, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Listen", _r), &D::ClassType::Listen);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Accept", _r), &D::ClassType::Accept)
        ._default(copyVariantFromCopyable((IPAddress *)NULL))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Send", _r), (Socket::Status (D::ClassType::*) (const char *, std::size_t))&D::ClassType::Send);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Receive", _r), (Socket::Status (D::ClassType::*) (char *, std::size_t, std::size_t &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Send", _r), (Socket::Status (D::ClassType::*) (Packet &))&D::ClassType::Send);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Receive", _r), (Socket::Status (D::ClassType::*) (Packet &))&D::ClassType::Receive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Close", _r), &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SocketTCP &)>(mopHolder < mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
