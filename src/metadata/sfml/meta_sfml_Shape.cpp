// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics/Shape.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Shape.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Shape()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Shape, sf::Drawable> _nd = GDefineMetaClass<sf::Shape, sf::Drawable>::declare("Shape");
        buildMetaClass_Shape(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


