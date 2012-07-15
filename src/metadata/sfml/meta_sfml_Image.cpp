// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics/Image.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Image.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Image()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Image, sf::Resource< Image >> _nd = GDefineMetaClass<sf::Image, sf::Resource< Image >>::declare("Image");
        buildMetaClass_Image(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


