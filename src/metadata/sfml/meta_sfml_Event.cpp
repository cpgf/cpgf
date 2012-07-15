// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/Event.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Event.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Global_event()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_event(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Event()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Event> _nd = GDefineMetaClass<sf::Event>::declare("Event");
        buildMetaClass_Event(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


