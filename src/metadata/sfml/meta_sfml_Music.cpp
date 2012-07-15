// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/Music.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Music.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Music()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Music, sf::SoundStream> _nd = GDefineMetaClass<sf::Music, sf::SoundStream>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("Music");
        buildMetaClass_Music(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


