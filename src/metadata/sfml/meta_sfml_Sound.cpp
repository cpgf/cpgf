// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/Sound.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Sound.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Sound()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Sound, sf::AudioResource> _nd = GDefineMetaClass<sf::Sound, sf::AudioResource>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Sound");
        buildMetaClass_Sound(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


