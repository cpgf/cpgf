// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/Input.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Input.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Input()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Input, sf::WindowListener> _nd = GDefineMetaClass<sf::Input, sf::WindowListener>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Input");
        buildMetaClass_Input(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


