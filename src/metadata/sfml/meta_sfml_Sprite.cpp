// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Sprite.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Sprite()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Sprite, sf::Drawable> _nd = GDefineMetaClass<sf::Sprite, sf::Drawable>::declare("Sprite");
        buildMetaClass_Sprite(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


