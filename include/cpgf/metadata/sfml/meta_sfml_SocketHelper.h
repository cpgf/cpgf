// Auto generated file, don't modify.

#ifndef __META_SFML_SOCKETHELPER_H
#define __META_SFML_SOCKETHELPER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Socket;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_sockethelper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Status>("Status")
        ._element("Done", sf::Socket::Done)
        ._element("NotReady", sf::Socket::NotReady)
        ._element("Disconnected", sf::Socket::Disconnected)
        ._element("Error", sf::Socket::Error)
    ;
}


template <typename D>
void buildMetaClass_SocketHelper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("InvalidSocket", &D::ClassType::InvalidSocket);
    _d.CPGF_MD_TEMPLATE _method("Close", &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method("SetBlocking", &D::ClassType::SetBlocking);
    _d.CPGF_MD_TEMPLATE _method("GetErrorStatus", &D::ClassType::GetErrorStatus);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
