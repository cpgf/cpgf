// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/Context.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Context.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Context()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Context> _nd = GDefineMetaClass<sf::Context>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Context");
        buildMetaClass_Context(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


