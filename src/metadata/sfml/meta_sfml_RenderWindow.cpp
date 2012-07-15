// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "cpgf/metadata/sfml/meta_sfml_RenderWindow.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_RenderWindow()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::RenderWindow, sf::Window, sf::RenderTarget> _nd = GDefineMetaClass<sf::RenderWindow, sf::Window, sf::RenderTarget>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("RenderWindow");
        buildMetaClass_RenderWindow(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


