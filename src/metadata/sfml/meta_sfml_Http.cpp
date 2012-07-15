// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network/Http.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Http.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Http()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Http> _nd = GDefineMetaClass<sf::Http>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Http");
        buildMetaClass_Http(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


