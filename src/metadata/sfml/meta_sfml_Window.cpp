// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/Window.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Window.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Window()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Window, sf::WindowListener> _nd = GDefineMetaClass<sf::Window, sf::WindowListener>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Window");
        buildMetaClass_Window(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


