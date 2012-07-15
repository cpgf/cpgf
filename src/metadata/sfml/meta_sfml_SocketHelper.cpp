// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network/SocketHelper.hpp"

#include "cpgf/metadata/sfml/meta_sfml_SocketHelper.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Global_sockethelper()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_sockethelper(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SocketHelper()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::SocketHelper> _nd = GDefineMetaClass<sf::SocketHelper>::declare("SocketHelper");
        buildMetaClass_SocketHelper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


