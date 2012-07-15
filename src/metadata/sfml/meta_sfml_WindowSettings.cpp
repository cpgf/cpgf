// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/WindowSettings.hpp"

#include "cpgf/metadata/sfml/meta_sfml_WindowSettings.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_WindowSettings()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::WindowSettings> _nd = GDefineMetaClass<sf::WindowSettings>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("WindowSettings");
        buildMetaClass_WindowSettings(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


