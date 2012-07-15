// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/SoundBufferRecorder.hpp"

#include "cpgf/metadata/sfml/meta_sfml_SoundBufferRecorder.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_SoundBufferRecorder()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::SoundBufferRecorder, sf::SoundRecorder> _nd = GDefineMetaClass<sf::SoundBufferRecorder, sf::SoundRecorder>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("SoundBufferRecorder");
        buildMetaClass_SoundBufferRecorder(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


