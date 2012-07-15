// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network/IPAddress.hpp"

#include "cpgf/metadata/sfml/meta_sfml_IPAddress.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Global_ipaddress()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ipaddress(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IPAddress()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::IPAddress> _nd = GDefineMetaClass<sf::IPAddress>::declare("IPAddress");
        buildMetaClass_IPAddress(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


