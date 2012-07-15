// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include "cpgf/metadata/sfml/meta_sfml_SoundBuffer.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_SoundBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::SoundBuffer, sf::AudioResource, sf::Resource< SoundBuffer >> _nd = GDefineMetaClass<sf::SoundBuffer, sf::AudioResource, sf::Resource< SoundBuffer >>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("SoundBuffer");
        buildMetaClass_SoundBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


