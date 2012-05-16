// Auto generated file, don't modify.

#ifndef __META_SFML_SOCKETHELPER_H
#define __META_SFML_SOCKETHELPER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Socket;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_sockethelper(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Status>(replaceName("Status", _r))
        ._element(replaceName("Done", _r), sf::Socket::Done)
        ._element(replaceName("NotReady", _r), sf::Socket::NotReady)
        ._element(replaceName("Disconnected", _r), sf::Socket::Disconnected)
        ._element(replaceName("Error", _r), sf::Socket::Error)
    ;
}


template <typename D>
void buildMetaClass_SocketHelper(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("InvalidSocket", _r), &D::ClassType::InvalidSocket);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Close", _r), &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetBlocking", _r), &D::ClassType::SetBlocking);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetErrorStatus", _r), &D::ClassType::GetErrorStatus);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
