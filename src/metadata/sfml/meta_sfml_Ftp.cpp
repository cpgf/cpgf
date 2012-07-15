// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network/Ftp.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Ftp.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Ftp()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Ftp> _nd = GDefineMetaClass<sf::Ftp>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Ftp");
        buildMetaClass_Ftp(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


